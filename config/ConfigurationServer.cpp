/* ************************************************************************** */
/*                                      */
/*                            :::     ::::::::   */
/*   ConfigurationServer.cpp                            :+:   :+:    :+:   */
/*                          +:+ +:+       +:+    */
/*   By: bbelen <bbelen@21-school.ru>            +#+  +:+       +#+    */
/*                        +#+#+#+#+#+   +#+     */
/*   Created: 2021/06/05 15:52:16 by bbelen   #+#  #+#          */
/*   Updated: 2021/07/07 23:15:27 by bbelen     ###   ########.fr    */
/*                                      */
/* ************************************************************************** */

#include "ConfigurationFile.hpp"

ConfigurationServer::ConfigurationServer()
{
    this->config = NULL;
}

ConfigurationServer::~ConfigurationServer()
{
    this->config = NULL;
}

ConfigurationServer::ConfigurationServer(const ConfigurationServer &config)
{
    *this = config;
}

ConfigurationServer &ConfigurationServer::operator=(const ConfigurationServer &config)
{
    this->autoIndex = config.autoIndex;
    this->listenVec = config.listenVec;
    this->methods = config.methods;
    this->root = config.root;
    this->serverNameVec = config.serverNameVec;
    this->locationVec = config.locationVec;
    this->indexVec = config.indexVec;
    this->returnAddr = config.returnAddr;
    this->errorPage = config.errorPage;

    return *this;
}

void    ConfigurationServer::addListen(t_listen  listen)
{
    this->listenVec.push_back(listen);
}

void    ConfigurationServer::setRoot(std::string root)
{
    this->root = root;
}

void    ConfigurationServer::addServerName(std::string serverName)
{
    this->serverNameVec.push_back(serverName);
}

void    ConfigurationServer::addLocation(location &location)
{
    this->locationVec.push_back(location);
}

void    ConfigurationServer::addMethod(std::string method)
{
    this->methods.insert(method);
}

void    ConfigurationServer::setAutoIndex(bool autoIndex)
{
    this->autoIndex = autoIndex;
}

void    ConfigurationServer::addReturnAddress(returnAddress returnAddr)
{
    this->returnAddr.errorCode = returnAddr.errorCode;
    this->returnAddr.address = returnAddr.address;
}

void    ConfigurationServer::addIndex(std::string index)
{
    this->indexVec.push_back(index);
}

void    ConfigurationServer::setConfig(ConfigurationFile *config)
{
    this->config = config;
}

void    ConfigurationServer::setErrorPage(std::string page)
{
    this->errorPage = page;
}

std::vector<t_listen>      &ConfigurationServer::getListenVec()
{
    return this->listenVec;
}

std::vector<std::string>    &ConfigurationServer::getServerNameVec()
{
    return this->serverNameVec;
}

std::string     ConfigurationServer::getRoot()
{
    return this->root;
}

std::string     ConfigurationServer::getErrorPage()
{
    return this->errorPage;
}

std::vector<location>      &ConfigurationServer::getLocationVec()
{
    return this->locationVec;
}

std::set<std::string>      ConfigurationServer::getMethods()
{
    return this->methods;
}

std::vector<std::string>    &ConfigurationServer::getIndexVec()
{
    return this->indexVec;
}

returnAddress            ConfigurationServer::getReturnAddress()
{
    return this->returnAddr;
}

ConfigurationFile           *ConfigurationServer::getServerConfig()
{
    return this->config;
}

bool    ConfigurationServer::checkPortsTaken(int possiblePort)
{
    if (this->listenVec.size() != 0)
    {
        for (unsigned long i = 0; i < this->listenVec.size(); i++)
        {
            if (this->listenVec[i].port == possiblePort)
                return (false);
        }
    }

    std::vector<ConfigurationServer> *servers = this->config->getServers();
    if (servers == NULL)
        return (true);
    
    std::vector<ConfigurationServer>::iterator it = servers->begin();
    std::vector<ConfigurationServer>::iterator itE = servers->end();

    while (it != itE)
    {
        std::vector<t_listen> &listenVec = it->getListenVec();
        
        std::vector<t_listen>::iterator itListen = listenVec.begin();
        std::vector<t_listen>::iterator itListenE = listenVec.end();
        while (itListen != itListenE)
        {
            if (itListen->port == possiblePort)
                return (false);
            itListen++;
        }

        it++;
    }
    return (true);
}

void    ConfigurationServer::parseListen(std::vector<std::string> &line)
{
    if (line[line.size() - 1] != ";" && *(line[line.size() - 1].end() - 1) != ';')
    {
        throw ConfigurationServer::ServerParserException();
    }

    t_listen  currentListen;
    currentListen.host = "";

    std::string::iterator it = line[1].begin();
    std::string::iterator itE = line[1].end();

    while (it != itE && *it != ':')
        it++;
    
    if (it != itE)
    {
        std::string host;
        host.assign(line[1].begin(), it);
        currentListen.host += host;
        it++;
        std::string port;
        port.assign(it, line[1].end());
        int possiblePort = std::atoi(port.c_str());
        if (possiblePort < 1 || possiblePort > 65535 || !(this->checkPortsTaken(possiblePort)))
        {
            throw ConfigurationServer::ServerPortException();
        }
        currentListen.port = possiblePort;
    }
    else
    {
        int possiblePort = std::atoi(line[1].c_str());
        if (possiblePort < 1 || possiblePort > 65535 || !(this->checkPortsTaken(possiblePort)))
        {
            throw ConfigurationServer::ServerPortException();
        }
        currentListen.port = possiblePort;
    }
    this->addListen(currentListen);
}

void    ConfigurationServer::parseErrorPage(std::vector<std::string> &line)
{
    if (line.size() == 1)
    {
        throw ConfigurationServer::ServerIndexException();
    }
    for (unsigned long i = 1; i < line.size(); i++)
    {
        if (i + 1 == line.size())
        {
            // ';' может быть через пробел, тогда просто игнорируем, если приклеена
            // то обрезаем, иначе - ошибка синтаксиса, ибо ';'должна быть
            if (line[i] == ";")
                break;
            else if (line[i][line[i].size() - 1] == ';')
            {
                line[i].assign(line[i].begin(), line[i].end() - 1);
            }
            else
            {
                throw ConfigurationServer::ServerParserException();
            }
        }
        this->setErrorPage(line[i]);
    }
}

void    ConfigurationServer::parseServerName(std::vector<std::string> &line)
{
    if (line.size() == 1)
    {
        throw ConfigurationServer::ServerNameException();
    }
    for (unsigned long i = 1; i < line.size(); i++)
    {
        if (i + 1 == line.size())
        {
            // ';' может быть через пробел, тогда просто игнорируем, если приклеена
            // то обрезаем, иначе - ошибка синтаксиса, ибо ';'должна быть
            if (line[i] == ";")
                break;
            else if (line[i][line[i].size() - 1] == ';')
            {
                line[i].assign(line[i].begin(), line[i].end() - 1);
            }
            else
            {
                throw ConfigurationServer::ServerParserException();
            }
        }
        this->addServerName(line[i]);
    }
}

void    ConfigurationServer::parseRoot(std::vector<std::string> &line)
{
    if (line.size() == 2 && *(line[1].end() - 1) == ';')
    {
        line[1].assign(line[1].begin(), line[1].end() - 1);
        this->setRoot(line[1]);
    }
    else if (line.size() == 3 && line[line.size() - 1] == ";")
    {
        this->setRoot(line[1]);
    }
    else
    {
        throw ConfigurationServer::ServerParserException();
    }
}

void    ConfigurationServer::parseIndex(std::vector<std::string> &line)
{
    if (line.size() == 1)
    {
        throw ConfigurationServer::ServerIndexException();
    }
    for (unsigned long i = 1; i < line.size(); i++)
    {
        if (i + 1 == line.size())
        {
            // ';' может быть через пробел, тогда просто игнорируем, если приклеена
            // то обрезаем, иначе - ошибка синтаксиса, ибо ';'должна быть
            if (line[i] == ";")
                break;
            else if (line[i][line[i].size() - 1] == ';')
            {
                line[i].assign(line[i].begin(), line[i].end() - 1);
            }
            else
            {
                throw ConfigurationServer::ServerParserException();
            }
        }
        this->addIndex(line[i]);
    }
}

void    ConfigurationServer::parseLocation(std::vector<std::string> &line)
{
    if (line.size() == 3)
    {
        location *newLocation = new location();

        newLocation->route = std::string(line[1]);
        newLocation->autoindex = false;
        newLocation->client_body_size = -1;
        if (line[2] != "{")
        {
            throw ConfigurationServer::ServerParserException();
            delete newLocation;
        }
        newLocation->errorCode = -1;
        newLocation->fastcgi_include = "";
        newLocation->fastcgi_pass = "";
        newLocation->finished = false;
        this->addLocation(*newLocation); 
        delete newLocation;
    }
    else
    {
        throw ConfigurationServer::ServerParserException();
    }
}

location&   ConfigurationServer::getLastLocation()
{
    return (this->locationVec.back());
}

bool    ConfigurationServer::findFileInDirectory(location &lastLocation, std::string fileName)
{
    std::vector<std::string>::iterator it = lastLocation.try_files.begin();
    std::vector<std::string>::iterator itE = lastLocation.try_files.end();

    while (it != itE)
    {
        if (*it == fileName)
            return true;
        it++;
    }
    return false;
}

void    ConfigurationServer::updateLocation(std::vector<std::string> &line)
{
    location &lastLocation = this->getLastLocation();
    if (line[0] == "autoindex")
    {
        if ((line[1] == "on" && line.size() >= 3 && line[2] == ";") || (line[1] == "on;"))
        {
            lastLocation.autoindex = true;
        }
        else if (line[1] == "off" && line.size() >= 3 && line[2] == ";")
            lastLocation.autoindex = false;
        else if (line[1] == "off;")
            lastLocation.autoindex = false;
        else
        {
            throw ConfigurationServer::ServerParserException();
        }
    }
    else if (line[0] == "root")
    {
        if (line.size() > 3)
        {
            throw ConfigurationServer::ServerParserException();
        }
        for (unsigned long i = 1; i < line.size(); i++)
        {
            if (i + 1 == line.size())
            {
                // ';' может быть через пробел, тогда просто игнорируем, если приклеена
                // то обрезаем, иначе - ошибка синтаксиса, ибо ';'должна быть
                if (line[i] == ";")
                    break;
                else if (line[i][line[i].size() - 1] == ';')
                {
                    line[i].assign(line[i].begin(), line[i].end() - 1);
                }
                else
                {
                    throw ConfigurationServer::ServerParserException();
                }
            }
            lastLocation.root = line[1];
        }
    }
    else if (line[0] == "index")
    {
        if (line.size() > 3)
        {
            throw ConfigurationServer::ServerParserException();
        }
        for (unsigned long i = 1; i < line.size(); i++)
        {
            if (i + 1 == line.size())
            {
                // ';' может быть через пробел, тогда просто игнорируем, если приклеена
                // то обрезаем, иначе - ошибка синтаксиса, ибо ';'должна быть
                if (line[i] == ";")
                    break;
                else if (line[i][line[i].size() - 1] == ';')
                {
                    line[i].assign(line[i].begin(), line[i].end() - 1);
                }
                else
                {
                    throw ConfigurationServer::ServerParserException();
                }
            }
            lastLocation.try_files.insert(lastLocation.try_files.begin(), line[i]);
        }
    }
    else if (line[0] == "try_files")
    {
        for (unsigned long i = 1; i < line.size(); i++)
        {
            if (i + 1 == line.size())
            {
                // ';' может быть через пробел, тогда просто игнорируем, если приклеена
                // то обрезаем, иначе - ошибка синтаксиса, ибо ';'должна быть
                if (line[i] == ";")
                    break;
                else if (line[i][line[i].size() - 1] == ';' && line[i][0] == '=')
                {
                    line[i].assign(line[i].begin() + 1, line[i].end() - 1);
                    int errorCode = std::atoi(line[i].c_str());
                    if (errorCode > 0)
                    {
                        lastLocation.errorCode = errorCode;
                    }
                    else
                    {
                        throw ConfigurationServer::ServerParserException();
                    }
                    break;
                }
                else
                {
                    throw ConfigurationServer::ServerParserException();
                }
            }
            lastLocation.try_files.push_back(line[1]);
        }
    }
    else if (line[0] == "include")
    {
        if (line.size() > 3)
        {
            throw ConfigurationServer::ServerParserException();
            exit(SYNTAX_ERROR);
        }
        for (unsigned long i = 1; i < line.size(); i++)
        {
            if (i + 1 == line.size())
            {
                // ';' может быть через пробел, тогда просто игнорируем, если приклеена
                // то обрезаем, иначе - ошибка синтаксиса, ибо ';'должна быть
                if (line[i] == ";")
                    break;
                else if (line[i][line[i].size() - 1] == ';')
                {
                    line[i].assign(line[i].begin(), line[i].end() - 1);
                }
                else
                {
                    throw ConfigurationServer::ServerParserException();
                }
            }
            lastLocation.fastcgi_include = line[i];
        }
    }
    else if (line[0] == "fastcgi_pass")
    {
        if (line.size() > 3)
        {
            throw ConfigurationServer::ServerParserException();
        }
        for (unsigned long i = 1; i < line.size(); i++)
        {
            if (i + 1 == line.size())
            {
                // ';' может быть через пробел, тогда просто игнорируем, если приклеена
                // то обрезаем, иначе - ошибка синтаксиса, ибо ';'должна быть
                if (line[i] == ";")
                    break;
                else if (line[i][line[i].size() - 1] == ';')
                {
                    line[i].assign(line[i].begin(), line[i].end() - 1);
                }
                else
                {
                    throw ConfigurationServer::ServerParserException();
                }
            }
            lastLocation.fastcgi_pass = line[i];
        }
    }
    else if (line[0] == "allow_methods")
    {
        if (line.size() < 2 || (line.size() == 2 && line[1] == ";" ))
        {
            throw ConfigurationServer::ServerParserException();
        }
        else
        {
            for (unsigned long i = 1; i < line.size(); i++)
            {
                if (i + 1 == line.size())
                {
                    // ';' может быть через пробел, тогда просто игнорируем, если приклеена
                    // то обрезаем, иначе - ошибка синтаксиса, ибо ';'должна быть
                    if (line[i] == ";")
                        break;
                    else if (line[i][line[i].size() - 1] == ';')
                    {
                        line[i].assign(line[i].begin(), line[i].end() - 1);
                    }
                    else if (line[i] != "GET" && line[i] != "PUT" && line[i] != "POST" && line[i] != "DELETE")
                    {
                        throw ConfigurationServer::ServerParserException();
                    }
                    else
                    {
                        throw ConfigurationServer::ServerParserException();
                    }
                }
                lastLocation.methods.insert(line[i]);
            }
        }
    }
    else if (line[0] == "client_body_size")
    {
        if (line.size() < 2)
        {
            throw ConfigurationServer::ServerParserException();
        }
        else
        {
            int size = std::atoi(line[1].c_str());
            if (size <= 0)
            {
                throw ConfigurationServer::ServerParserException();
            }
            lastLocation.client_body_size = size;
        }
    }
    else
    {
        throw ConfigurationServer::ServerParserException();
    }
        
}

void    ConfigurationServer::parseReturn(std::vector<std::string> &line)
{
    if (line.size() == 3 || (line.size() == 4 && line[line.size() - 1] == ";"))
    {
        returnAddress   returnAddress;
        
        int errorCode = std::atoi(line[1].c_str());
        if (errorCode < 1 || errorCode > 1000)
        {
            throw ConfigurationServer::ServerParserException();
        }
        this->returnAddr.errorCode = errorCode;
        if (line[2][line[2].size() - 1] == ';')
            line[2].assign(line[2].begin(), line[2].end() - 1);
        this->returnAddr.address = line[2];
    }
    else
    {
        throw ConfigurationServer::ServerParserException();
    }
}

void    ConfigurationServer::parseMethods(std::vector<std::string> &line)
{
    if (line.size() < 2 || (line.size() == 2 && line[1] == ";" ))
    {
        throw ConfigurationServer::ServerParserException();
    }
    else
    {
        for (unsigned long i = 1; i < line.size(); i++)
        {
            if (i + 1 == line.size())
            {
                // ';' может быть через пробел, тогда просто игнорируем, если приклеена
                // то обрезаем, иначе - ошибка синтаксиса, ибо ';'должна быть
                if (line[i] == ";")
                        break;
                else if (line[i][line[i].size() - 1] == ';')
                {
                    line[i].assign(line[i].begin(), line[i].end() - 1);
                }
                else if (line[i] != "GET" && line[i] != "PUT" && line[i] != "POST" && line[i] != "DELETE")
                {
                    throw ConfigurationServer::ServerParserException();
                }
                else
                {
                    throw ConfigurationServer::ServerParserException();
                }
            }
            this->methods.insert(line[i]);
        }
    }
}

void	ConfigurationServer::checkFilledServer()
{
	if (this->serverNameVec.size() == 0 || this->indexVec.size() == 0 ||
			this->root == "" || this->listenVec.size() == 0)
	{
		throw ConfigurationServer::ServerNotEnoughParansException();
	}
}

const char* ConfigurationServer::ServerParserException::what() const throw()
{
    return ("ServerParserException: syntax error\n");
}

const char* ConfigurationServer::ServerPortException::what() const throw()
{
    return ("ServerPortException: port number out of bounds\n");
}

const char* ConfigurationServer::ServerNameException::what() const throw()
{
    return ("ServerNameException: no server name\n");
}

const char* ConfigurationServer::ServerIndexException::what() const throw()
{
    return ("ServerNameException: no index pages\n");
}

const char* ConfigurationServer::ServerNotEnoughParansException::what() const throw()
{
    return ("ServerNotEnoughParansException: not enough parameters\n");
}

std::ostream &operator<<(std::ostream &os, ConfigurationServer &server)
{
    if (server.getServerNameVec().size() != 0)
    {
        os << "server names size " << server.getServerNameVec().size() << ": [";
        std::vector<std::string> &serverNames = server.getServerNameVec();
        for (unsigned long i = 0; i < serverNames.size(); i++)
        {
            os << serverNames.at(i);
            if (i + 1 < serverNames.size())
                os << "] [";
        }
        os << "]" << std::endl;
    }
    os << "root: [" << server.getRoot() << "]" << std::endl;
    os << "PORTS: [";
    std::vector<t_listen> &listenVec = server.getListenVec();
    for (unsigned long i = 0; i < listenVec.size(); i++)
    {
        os << listenVec[i].port;
        if (i + 1 < listenVec.size())
            os << "] [";
    }
    os << "]" << std::endl;

    std::vector<std::string> &indexVec = server.getIndexVec();
    if (indexVec.size() != 0)
    {
        os << "index size " << indexVec.size() << ": [";
        for (unsigned long i = 0; i < indexVec.size(); i++)
        {
            os << indexVec.at(i);
            if (i + 1 < indexVec.size())
                os << "] [";
        }
        os << "]" << std::endl;
    }
    os << "error_page: [" << server.getErrorPage() << "]" << std::endl;
    os << "methods: [";
    std::set<std::string> methodsSet = server.getMethods();
    std::set<std::string>::iterator it = methodsSet.begin();
    std::set<std::string>::iterator itEnd = methodsSet.end();
    int size = methodsSet.size();
    while (it != itEnd)
    {
        os << *it;
        if (size - 1 > 0)
            os << "] [";
        it++;
        size--;
    }
    os << "]" << std::endl;
    if (server.getReturnAddress().address != "")
    {
        os << "return address: [" << server.getReturnAddress().address
            << "] code: [" << server.getReturnAddress().errorCode << "]" << std::endl;
    }
    std::vector<location> &locationVec = server.getLocationVec();
    os << "locations: " << locationVec.size() << std::endl;
    for (unsigned long i = 0; i < locationVec.size(); i++)
    {
        os << "route: [";
        os << locationVec[i].route << "] with errorCode: [" << locationVec[i].errorCode;
        os << "] cgi: [" << locationVec[i].fastcgi_pass;
        os << "] cgi_include: [" << locationVec[i].fastcgi_include;
        os << "] autoindex: [" << locationVec[i].autoindex;
        if (locationVec[i].try_files.size() != 0)
        {
            os << "] try_files: [";
            for (unsigned long j = 0; j < locationVec[i].try_files.size(); j++)
            {
                os << locationVec[i].try_files.at(j) << "] [";
            }
        }
        os << "] methods: [";
        std::set<std::string>::iterator it = locationVec[i].methods.begin();
        std::set<std::string>::iterator itEnd = locationVec[i].methods.end();
        int size = locationVec[i].methods.size();
        while (it != itEnd)
        {
            os << *it << " ";
            if (size - 1 > 0)
                os << "] [";
            it++;
            size--;
        }
        os << "]" << std::endl;
    }
	
	return (os);
}