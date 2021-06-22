/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationServer.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:52:16 by bbelen            #+#    #+#             */
/*   Updated: 2021/06/05 15:52:16 by bbelen           ###   ########.fr       */
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

void    ConfigurationServer::addLocation(ConfigurationLocation location)
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

std::vector<t_listen>       ConfigurationServer::getListenVec()
{
    return this->listenVec;
}

std::vector<std::string>    ConfigurationServer::getServerNameVec()
{
    return this->serverNameVec;
}