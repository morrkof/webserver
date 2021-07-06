/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationServer.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:52:16 by bbelen            #+#    #+#             */
/*   Updated: 2021/07/06 09:53:53 by bbelen           ###   ########.fr       */
/*                                                                            */
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


std::vector<t_listen>       ConfigurationServer::getListenVec()
{
    return this->listenVec;
}

std::vector<std::string>    ConfigurationServer::getServerNameVec()
{
    return this->serverNameVec;
}

std::string                 ConfigurationServer::getRoot()
{
    return this->root;
}

std::vector<location>       ConfigurationServer::getLocationVec()
{
    return this->locationVec;
}

std::set<std::string>       ConfigurationServer::getMethods()
{
    return this->methods;
}

std::vector<std::string>    ConfigurationServer::getIndexVec()
{
    return this->indexVec;
}

returnAddress               ConfigurationServer::getReturnAddress()
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
    }
    //std::cout << "---------Success root: " << this->root << std::endl;
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
        //std::cout << "---------Success server_name: " << this->indexVec[i - 1] << std::endl;
    }
}

void    ConfigurationServer::parseLocation(std::vector<std::string> &line)
{
    //TODO: finish
    std::cout << "line is: " << line[0] << std::endl;
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
    }
    //std::cout << "---------Success return: " << this->returnAddr.errorCode << std::endl;
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