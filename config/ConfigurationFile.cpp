/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:51:41 by bbelen            #+#    #+#             */
/*   Updated: 2021/06/27 09:57:28 by bbelen           ###   ########.fr       */
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
    std::ifstream   config;

    try
    {
        config.open(filename);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    //std::stringstream   ss;
    std::string         line; 
    MapConfigFile       map;
    while (std::getline(config, line))
    {
        if (line[0] == '#')
            continue;
        std::istringstream iss(line);
        std::vector<std::string> LineParts((std::istream_iterator<std::string>(iss)),
            std::istream_iterator<std::string>());
        if (LineParts[1] == "{")
        {
            map.addBracketOpen(1);
            map.setBlockName(LineParts[0]);
        }
        else if (LineParts[0] == "}")
        {
            map.addBracketClose(1);
        }
    }
    
    config.close();
}