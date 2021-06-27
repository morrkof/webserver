/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:51:41 by bbelen            #+#    #+#             */
/*   Updated: 2021/06/27 19:55:36 by bbelen           ###   ########.fr       */
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

void    ConfigurationFile::addServer(ConfigurationServer server)
{
    this->serverVec.push_back(server);
}

std::vector<ConfigurationServer>    ConfigurationFile::getServers()
{
    return (this->serverVec);
}

void    ConfigurationFile::parseFile(std::string filename)
{
    //TODO: check name to have .conf
    
    std::ifstream   config;

    config.open(filename.c_str());
    if (!(config.is_open()))
    {
        // TODO: exception file not found
        std::cout << "ERROR: config not found" << std::endl;
        return;
    }
    

    //std::stringstream   ss;
    std::string         line; 
    MapConfigFile       map;
    std::vector<std::string> blockBody;
    while (std::getline(config, line))
    {        
        if (line[0] == '#' || line == "" || this->lineOnlySpacesOrTabs(line))
            continue;

        // line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");
        // std::string::size_type begin = line.find_first_not_of("\t");
        // std::string::size_type end   = line.find_last_not_of("\t");
        // line = line.substr(begin, end-begin + 1);
        
        std::istringstream iss(line);
        std::vector<std::string> LineParts((std::istream_iterator<std::string>(iss)),
            std::istream_iterator<std::string>());
        if (LineParts.size() == 2 && LineParts[1] == "{")
        {
            map.addBracketOpen(1);
            map.setBlockName(LineParts[0]);
        }
        else if (LineParts[0] == "}")
        {
            map.addBracketClose(1);
            if (map.checkBrackets())
            {
                //check block and get server params
                this->checkConfigBlock(map, blockBody);
                map.resetMap();
                blockBody.clear();
            }
        }
        else
        {
            blockBody.push_back(line);
        }
    }
    std::cout << "--------finished reading config" << std::endl;
    config.close();
}

void    ConfigurationFile::checkConfigBlock(MapConfigFile &map, std::vector<std::string> &block)
{
    //std::cout << "--------Start check block" << std::endl;
    if (!map.checkBrackets())
    {
        //TODO: exception brackets
        return ;
    }
    if (!map.checkBlockName())
    {
        //TODO: exception block name
        return ;
    }
    //std::cout << "--------Ok block OK brackets" << std::endl;
    
    std::vector<std::string>::iterator  it = block.begin();
    std::vector<std::string>::iterator  itEnd = block.end();
    ConfigurationServer server;

    while (it != itEnd)
    {
        //std::cout << "line: " << *it << std::endl;
        std::istringstream iss(*it);
        std::vector<std::string> lineParts((std::istream_iterator<std::string>(iss)),
            std::istream_iterator<std::string>());
        // for (int i = 0; i < lineParts.size(); i++)
        // {
        //     //поехал разбор строки
        //     std::cout << "Part: " << lineParts[i] << std::endl;
        // }

        //поехал разбор строки
        if (map.getBlockName() == "server")
        {
            this->parseBlockLine(lineParts, server);
        }
        it++;
    }

    //std::cout << "Config block ready." << std::endl;
    this->addServer(server);

}

bool    ConfigurationFile::lineOnlySpacesOrTabs(std::string line)
{
    std::string::iterator it = line.begin();
    std::string::iterator itEnd = line.end();

    while (it != itEnd)
    {
        if (*it != ' ' && *it != '\t')
        {
            return false;
        }
        it++;
    }
    return true;
}

void    ConfigurationFile::parseBlockLine(std::vector<std::string> line, ConfigurationServer &server)
{
    if (line[0] == "listen")
        server.parseListen(line);
    else if (line[0] == "server_name")
        server.parseServerName(line);
    else if (line[0] == "root")
        server.parseRoot(line);
    else if (line[0] == "index")
        server.parseIndex(line);
    else if (line[0] == "location")
        server.parseLocation(line);
    else if (line[0] == "return")
        server.parseReturn(line);
    else
    {
        std::cout << "Error config line: |" << line[0] << "|" << std::endl;
        //TODO exception error config wrong line
    }
    
}
