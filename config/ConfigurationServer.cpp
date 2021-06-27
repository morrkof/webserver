/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationServer.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:52:16 by bbelen            #+#    #+#             */
/*   Updated: 2021/06/27 18:46:05 by bbelen           ###   ########.fr       */
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

void    ConfigurationServer::addListen(listen  listen)
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


std::vector<listen>       ConfigurationServer::getListenVec()
{
    return this->listenVec;
}

std::vector<std::string>    ConfigurationServer::getServerNameVec()
{
    return this->serverNameVec;
}

void    ConfigurationServer::parseListen(std::vector<std::string> &line)
{
    listen  currentListen;
    //TODO: add check that port is ok, limits and if is number at all
    currentListen.port = std::atoi(line[1].c_str());
    std::cout << "---------Success listen: " << currentListen.port << std::endl;
}

void    ConfigurationServer::parseServerName(std::vector<std::string> &line)
{
    if (line.size() == 1)
    {
        //throw exception no server names
        std::cout << "------no server names" << std::endl;
    }
    for (int i = 1; i < line.size(); i++)
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
                std::cout << "ERROR: no ';' at the end of the line" << std::endl;
                //TODO: error syntax exception
            }
        }
        this->addServerName(line[i]);
        std::cout << "---------Success server_name: " << this->serverNameVec[i - 1] << std::endl;
    }
    
}

void    ConfigurationServer::parseRoot(std::vector<std::string> &line)
{

}

void    ConfigurationServer::parseIndex(std::vector<std::string> &line)
{

}

void    ConfigurationServer::parseLocation(std::vector<std::string> &line)
{

}

void    ConfigurationServer::parseReturn(std::vector<std::string> &line)
{
    if (line.size() == 3)
    {
        returnAddress   returnAddress;
        
        //TODO: check if number
        returnAddress.errorCode = atoi(line[1].c_str());
        returnAddress.address = line[2];
    }
    else
    {
        //TODO: syntax exception
        std::cout << "ERROR: wrong return line" << std::endl;
    }
    
}