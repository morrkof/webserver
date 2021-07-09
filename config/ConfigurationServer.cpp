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
    this->serverNameVec = NULL;
    this->root = "";
    this->indexVec = NULL;
    //this->returnAddr = "";
    //this->locationVec = NULL;
}

ConfigurationServer::~ConfigurationServer()
{

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
    if (this->serverNameVec == NULL)
    {
        this->serverNameVec = new std::vector<std::string>(1, serverName);
        return;
    }
    this->serverNameVec->push_back(serverName);
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
        if (this->indexVec == NULL)
    {
        //std::cout << "No indeces before" << std::endl;
        this->indexVec = new std::vector<std::string>(1, index);
        //std::cout << *(this->indexVec->begin()) << std::endl;
        return;
    }
    this->indexVec->push_back(index);
}


std::vector<t_listen>      ConfigurationServer::getListenVec()
{
    return this->listenVec;
}

std::vector<std::string>    *ConfigurationServer::getServerNameVec()
{
    return this->serverNameVec;
}

std::string     ConfigurationServer::getRoot()
{
    return this->root;
}

std::vector<location>      &ConfigurationServer::getLocationVec()
{
    return this->locationVec;
}

std::set<std::string>      ConfigurationServer::getMethods()
{
    return this->methods;
}

std::vector<std::string>    *ConfigurationServer::getIndexVec()
{
    return this->indexVec;
}

returnAddress            ConfigurationServer::getReturnAddress()
{
    return this->returnAddr;
}

void    ConfigurationServer::parseListen(std::vector<std::string> &line)
{
    t_listen  currentListen;
    int possiblePort = std::atoi(line[1].c_str());
    if (possiblePort < 1 || possiblePort > 65535)
    {
        throw ConfigurationServer::ServerPortException();
    }
    currentListen.port = std::atoi(line[1].c_str());
    this->addListen(currentListen);
    //std::cout << "---------Success listen: " << currentListen.port << std::endl;
}

void    ConfigurationServer::parseServerName(std::vector<std::string> &line)
{
    if (line.size() == 1)
    {
        throw ConfigurationServer::ServerNameException();
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
                exit(SYNTAX_ERROR);
            }
        }
        this->addServerName(line[i]);
        //std::cout << "---------Success server_name: " << this->serverNameVec[i - 1] << std::endl;
    }
        
}

void    ConfigurationServer::parseRoot(std::vector<std::string> &line)
{
    if (line.size() == 2)
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
        exit(SYNTAX_ERROR);
    }
    //std::cout << "---------Success root: " << this->root << std::endl;
}

void    ConfigurationServer::parseIndex(std::vector<std::string> &line)
{
    if (line.size() == 1)
    {
        throw ConfigurationServer::ServerIndexException();
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
                exit(SYNTAX_ERROR);
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
        newLocation->try_files = NULL;
        newLocation->try_files = new std::vector<std::string>();
        if (line[2] != "{")
        {
            throw ConfigurationServer::ServerParserException();
            delete newLocation;
            exit(SYNTAX_ERROR);
        }
        newLocation->errorCode = -1;
        newLocation->fastcgi_include = "";
        newLocation->fastcgi_pass = "";
        newLocation->finished = false;
        this->addLocation(*newLocation); 
        // std::cout << "location added: " << this->getLocationVec().size() << std::endl;
    }
    else
    {
        throw ConfigurationServer::ServerParserException();
        exit(SYNTAX_ERROR);
    }
}

location&   ConfigurationServer::getLastLocation()
{
    return (this->locationVec.back());
}

void    ConfigurationServer::updateLocation(std::vector<std::string> &line)
{
    location &lastLocation = this->getLastLocation();
    if (line[0] == "autoindex")
    {
        if (line[1] == "on" && line[2] == ";")
            lastLocation.autoindex = true;
        else if (line[1] == "on;")
            lastLocation.autoindex = true;
        else if (line[1] == "off" && line[2] == ";")
            lastLocation.autoindex = false;
        else if (line[1] == "off;")
            lastLocation.autoindex = false;
        else
        {
            throw ConfigurationServer::ServerParserException();
            exit(SYNTAX_ERROR);
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
                        exit(SYNTAX_ERROR);
                    }
                    break;
                }
                else
                {
                    throw ConfigurationServer::ServerParserException();
                    exit(SYNTAX_ERROR);
                }
            }
            lastLocation.try_files->push_back(line[1]);
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
                    exit(SYNTAX_ERROR);
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
                    exit(SYNTAX_ERROR);     
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
            exit(SYNTAX_ERROR);
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
                        exit(SYNTAX_ERROR);
                    }
                    else
                    {
                        throw ConfigurationServer::ServerParserException();
                        exit(SYNTAX_ERROR);     
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
            exit(SYNTAX_ERROR); 
        }
        else
        {
            int size = std::atoi(line[1].c_str());
            if (size <= 0)
            {
                throw ConfigurationServer::ServerParserException();
                exit(SYNTAX_ERROR); 
            }
            lastLocation.client_body_size = size;
        }
    }
    else
    {
        throw ConfigurationServer::ServerParserException();
        exit(SYNTAX_ERROR);
    }
        
}

void    ConfigurationServer::parseReturn(std::vector<std::string> &line)
{
    if (line.size() == 3 || (line.size() == 4 && line[line.size() - 1] == ";"))
    {
        returnAddress   returnAddress;
        
        //TODO: check if number
        returnAddress.errorCode = std::atoi(line[1].c_str());
        returnAddress.address = line[2];
    }
    else
    {
        throw ConfigurationServer::ServerParserException();
        exit(SYNTAX_ERROR);
    }
    //std::cout << "---------Success return: " << this->returnAddr.errorCode << std::endl;
}

void    ConfigurationServer::parseMethods(std::vector<std::string> &line)
{
    if (line.size() < 2 || (line.size() == 2 && line[1] == ";" ))
    {
        throw ConfigurationServer::ServerParserException();
        exit(SYNTAX_ERROR);
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
                    exit(SYNTAX_ERROR);
                }
                else
                {
                    throw ConfigurationServer::ServerParserException();
                    exit(SYNTAX_ERROR);     
                }
            }
            this->methods.insert(line[i]);
        }
    }
}

void	ConfigurationServer::checkFilledServer()
{
	if (this->serverNameVec->size() == 0 || this->indexVec->size() == 0 ||
			this->root == "" || this->locationVec.size() == 0)
	{
		throw ConfigurationServer::ServerNotEnoughParansException();
		exit(SYNTAX_ERROR);
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
    if (server.getServerNameVec() != NULL)
    {
        os << "server names size " << server.getServerNameVec()->size() << ": [";
        std::vector<std::string> *serverNames = server.getServerNameVec();
        for (unsigned long i = 0; i < serverNames->size(); i++)
        {
            os << serverNames->at(i);
            if (i + 1 < serverNames->size())
                os << "] [";
        }
        os << "]" << std::endl;
    }
    os << "root: [" << server.getRoot() << "]" << std::endl;
    os << "ports: [";
    std::vector<t_listen> listenVec = server.getListenVec();
    for (unsigned long i = 0; i < listenVec.size(); i++)
    {
        os << listenVec[i].port;
        if (i + 1 < listenVec.size())
            os << "] [";
    }
    os << "]" << std::endl;

    std::vector<std::string> *indexVec = server.getIndexVec();
    if (indexVec != NULL)
    {
        os << "index size " << indexVec->size() << ": [";
        for (unsigned long i = 0; i < indexVec->size(); i++)
        {
            os << indexVec->at(i);
            if (i + 1 < indexVec->size())
                os << "] [";
        }
        os << "]" << std::endl;
    }
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

    std::vector<location> &locationVec = server.getLocationVec();
    os << "locations: " << locationVec.size() << std::endl;
    for (unsigned long i = 0; i < locationVec.size(); i++)
    {
        os << "route: [";
        os << locationVec[i].route << "] with errorCode: [" << locationVec[i].errorCode;
        os << "] cgi: [" << locationVec[i].fastcgi_pass;
        os << "] cgi_include: [" << locationVec[i].fastcgi_include;
        if (locationVec[i].try_files != NULL)
        {
            os << "] try_files: [";
            for (unsigned long j = 0; j < locationVec[i].try_files->size(); j++)
            {
                os << locationVec[i].try_files->at(j) << "] [";
            }
        }
        os << "] methods: [";
        std::set<std::string>::iterator it = locationVec[i].methods.begin();
        std::set<std::string>::iterator itEnd = locationVec[i].methods.end();
        while (it != itEnd)
        {
            os << *it << " ";
            it++;
        }
        os << "]" << std::endl;
    }
	
	return (os);
}