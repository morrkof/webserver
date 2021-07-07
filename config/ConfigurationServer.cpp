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

void    ConfigurationServer::addLocation(location location)
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


std::vector<t_listen>      ConfigurationServer::getListenVec()
{
    return this->listenVec;
}

std::vector<std::string>    ConfigurationServer::getServerNameVec()
{
    return this->serverNameVec;
}

std::string     ConfigurationServer::getRoot()
{
    return this->root;
}

std::vector<location>      ConfigurationServer::getLocationVec()
{
    return this->locationVec;
}

std::set<std::string>      ConfigurationServer::getMethods()
{
    return this->methods;
}

std::vector<std::string>    ConfigurationServer::getIndexVec()
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
        //std::cout << "---------Success server_name: " << this->indexVec[i - 1] << std::endl;
    }
}

void    ConfigurationServer::parseLocation(std::vector<std::string> &line)
{
    if (line.size() == 3)
    {
        location newLocation;

        newLocation.route = line[1];
        newLocation.autoindex = false;
        newLocation.client_body_size = -1;
        if (line[2] != "{")
        {
            throw ConfigurationServer::ServerParserException();
            exit(SYNTAX_ERROR);
        }
        newLocation.finished = false;
        this->addLocation(newLocation); 
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

std::ostream &operator<<(std::ostream &os, ConfigurationServer &server)
{
	os << "----Server name: ";
    std::vector<std::string> serverNames = server.getServerNameVec();
    for (unsigned long i = 0; i < serverNames.size(); i++)
    {
        os << serverNames[i] << " ";
    }
    os << std::endl;
    os << "Root: " << server.getRoot() << std::endl;
    os << "Ports: ";
    std::vector<t_listen> listenVec = server.getListenVec();
    for (unsigned long i = 0; i < listenVec.size(); i++)
    {
        os << listenVec[i].port << " ";
    }
    os << std::endl;
    std::vector<location> locationVec = server.getLocationVec();
    os << "Locations: " << locationVec.size() << std::endl;
    for (unsigned long i = 0; i < locationVec.size(); i++)
    {
        os << locationVec[i].route << " " << locationVec[i].errorCode << " ";
    }
    os << std::endl;
	
	return (os);
}