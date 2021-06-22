/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:51:41 by bbelen            #+#    #+#             */
/*   Updated: 2021/06/22 13:49:09 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigurationFile.hpp"

ConfigurationFile::ConfigurationFile()
{

}

ConfigurationFile::~ConfigurationFile()
{
    
}

ConfigurationFile::ConfigurationFile(const ConfigurationFile &file)
{
    *this = file;
}

ConfigurationFile &ConfigurationFile::operator=(const ConfigurationFile &file)
{
    this->serverVec = file.serverVec;
    return (*this);
}

void    ConfigurationFile::addServer(ConfigurationServer &server)
{
    this->serverVec.push_back(server);
}

std::vector<ConfigurationServer>    ConfigurationFile::getServers()
{
    return (this->serverVec);
}

void    ConfigurationFile::parseFile(std::string filename)
{

}