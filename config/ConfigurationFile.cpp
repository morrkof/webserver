/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:51:41 by bbelen            #+#    #+#             */
/*   Updated: 2021/07/17 13:09:48 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigurationFile.hpp"

ConfigurationFile::ConfigurationFile()
{
    this->serverVec = NULL;
}

ConfigurationFile::~ConfigurationFile()
{
    delete this->serverVec;
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

void    ConfigurationFile::addServer(ConfigurationServer *server)
{
    if (server->getServerConfig() == NULL)
        server->setConfig(this);
    if (this->serverVec == NULL)
    {
        this->serverVec = new std::vector<ConfigurationServer>(1, *server);
    }
    else
    {
        this->serverVec->push_back(*server);
    }
}

std::vector<ConfigurationServer>    *ConfigurationFile::getServers()
{
    return (this->serverVec);
}

void    ConfigurationFile::parseFile(std::string filename)
{
	if (filename.find(".conf") == std::string::npos)
	{
		throw ConfigurationFile::ConfigFileFormatException();
		exit(FILE_ERROR);
	}
    
    std::ifstream   config;

    config.open(filename.c_str());
    if (!(config.is_open()))
    {
        throw ConfigurationFile::ConfigFileNotFoundException();
        exit(FILE_ERROR);
    }
    
    std::string         line; 
    MapConfigFile       map;
    std::vector<std::string> blockBody;
    while (std::getline(config, line))
    {        
        if (line[0] == '#' || line == "" || this->lineOnlySpacesOrTabs(line))
            continue;
        
        std::istringstream iss(line);
        std::vector<std::string> LineParts((std::istream_iterator<std::string>(iss)),
            std::istream_iterator<std::string>());
        if (LineParts.size() == 2 && LineParts[1] == "{")
        {
            map.addBracketOpen(1);
            map.setBlockName(LineParts[0]);
        }
        else if (LineParts.size() > 1 && LineParts[LineParts.size() - 1] == "{")
        {
            map.addBracketOpen(1);
            blockBody.push_back(line);
        }
        else if (LineParts[0] == "}")
        {
            map.addBracketClose(1);
            if (map.checkBrackets())
            {
                this->checkConfigBlock(map, blockBody);
                map.resetMap();
                blockBody.clear();
            }
            else
                blockBody.push_back(line);
        }
        else
        {
            blockBody.push_back(line);
        }
    }
    config.close();
}

void    ConfigurationFile::checkConfigBlock(MapConfigFile &map, std::vector<std::string> &block)
{
    if (!map.checkBrackets())
    {
        throw ConfigurationFile::ConfigFileParserException();
        exit(SYNTAX_ERROR);
    }
    if (!map.checkBlockName())
    {
        throw ConfigurationFile::ConfigFileParserException();
        exit(SYNTAX_ERROR);
    }
    
    std::vector<std::string>::iterator  it = block.begin();
    std::vector<std::string>::iterator  itEnd = block.end();
    
    ConfigurationServer *server = new ConfigurationServer();
    server->setConfig(this);

    while (it != itEnd)
    {
        std::istringstream iss(*it);
        std::vector<std::string> lineParts((std::istream_iterator<std::string>(iss)),
            std::istream_iterator<std::string>());
        if (map.getBlockName() == "server")
        {
            this->parseBlockLine(lineParts, server);
        }
        it++;
    }

	server->checkFilledServer();
    this->addServer(server);
    delete server;
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

void    ConfigurationFile::parseBlockLine(std::vector<std::string> line, ConfigurationServer *server)
{
    std::cout << "line: " << line[0] << std::endl;
    if (line[0] == "listen")
        server->parseListen(line);
    else if (line[0] == "server_name")
        server->parseServerName(line);
    else if (line[0] == "root" && server->getLocationVec().size() > 0 && server->getLastLocation().finished == false)
        server->updateLocation(line);
    else if (line[0] == "root")
        server->parseRoot(line);
    else if (line[0] == "index" && server->getLocationVec().size() > 0 && server->getLastLocation().finished == false)
        server->updateLocation(line);
    else if (line[0] == "index")
        server->parseIndex(line);
    else if (line[0] == "location")
        server->parseLocation(line);
    else if (line[0] == "return")
        server->parseReturn(line);
    else if (line[0] == "autoindex" && server->getLocationVec().size() > 0 && server->getLastLocation().finished == false)
        server->updateLocation(line);
    else if (line[0] == "try_files" && server->getLocationVec().size() > 0 && server->getLastLocation().finished == false)
        server->updateLocation(line);
    else if (line[0] == "include" && server->getLocationVec().size() > 0 && server->getLastLocation().finished == false)
        server->updateLocation(line);
    else if (line[0] == "fastcgi_pass" && server->getLocationVec().size() > 0 && server->getLastLocation().finished == false)
        server->updateLocation(line);
    else if (line[0] == "client_body_size" && server->getLocationVec().size() > 0 && server->getLastLocation().finished == false)
        server->updateLocation(line);
    else if (line[0] == "allow_methods" && server->getLocationVec().size() > 0 && server->getLastLocation().finished == false)
        server->updateLocation(line);
    else if (line[0] == "allow_methods")
        server->parseMethods(line);
    else if (line[0] == "}" && server->getLocationVec().size() > 0 && server->getLastLocation().finished == false)
    {
        server->getLastLocation().finished = true;
    }
    else
    {
        std::cout << "Error config line: |" << line[0] << "|" << std::endl;
        throw ConfigurationFile::ConfigFileParserException();
        exit(SYNTAX_ERROR);
    }
    
}

const char* ConfigurationFile::ConfigFileNotFoundException::what() const throw()
{
    return ("ConfigFileException: file not found\n");
}

const char* ConfigurationFile::ConfigFileParserException::what() const throw()
{
    return ("ConfigFileException: syntax error\n");
}

const char* ConfigurationFile::ConfigFileFormatException::what() const throw()
{
    return ("ConfigFileException: wrong file format\n");
}