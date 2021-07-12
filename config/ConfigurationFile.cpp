/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:51:41 by bbelen            #+#    #+#             */
/*   Updated: 2021/07/12 10:39:11 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigurationFile.hpp"

ConfigurationFile::ConfigurationFile()
{
    this->serverVec = NULL;
}

ConfigurationFile::~ConfigurationFile()
{
    std::cout << "---Cleaning---" << std::endl;
    // for (unsigned long i = 0; i < this->serverVec->size(); i++)
    // {
    //     //delete &this->serverVec[i];
    //     delete this->serverVec[i].
    // }
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
    // std::cout << "adding server: " << std::endl;
    // std::cout << *server;
    // std::cout << "adding server end" << std::endl;
    if (this->serverVec == NULL)
    {
        this->serverVec = new std::vector<ConfigurationServer>(1, *server);
        //this->serverVec->assign(1, *server);
    }
    else
    {
        // std::cout << "pushing back to the size of " << this->serverVec->size() << std::endl;
        this->serverVec->push_back(*server);
        // std::cout << "pushing success" << std::endl;
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
        
        // std::cout << "line: " << line << std::endl;
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
                //check block and get server params
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
    // std::cout << "--------finished reading config" << std::endl;
    config.close();
}

void    ConfigurationFile::checkConfigBlock(MapConfigFile &map, std::vector<std::string> &block)
{
    // std::cout << "--------Start check block" << std::endl;
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
    //std::cout << "--------Ok block OK brackets" << std::endl;
    
    std::vector<std::string>::iterator  it = block.begin();
    std::vector<std::string>::iterator  itEnd = block.end();
    
    ConfigurationServer *server = new ConfigurationServer();

    // std::cout << "----new server created" << std::endl; 

    while (it != itEnd)
    {
        std::istringstream iss(*it);
        std::vector<std::string> lineParts((std::istream_iterator<std::string>(iss)),
            std::istream_iterator<std::string>());
        if (map.getBlockName() == "server")
        {
			//get iter here as well
            this->parseBlockLine(lineParts, server);
        }
        it++;
    }

    //std::cout << "Config block ready." << std::endl;
    // std::cout << "----checking server" << std::endl;
	server->checkFilledServer();
    // std::cout << "----adding server" << std::endl;
    this->addServer(server);
    // std::cout << "----server added" << std::endl << std::endl;
    // std::cout << "---CHECK PARSING-----" << std::endl << *(this->getServers()->begin());
    // std::cout << "---CHECK PARSING END-" << std::endl;
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
    // std::cout << "line: ";
    // for (unsigned long i = 0; i < line.size(); i++)
    // {
    //     std::cout << line[i] << " ";
    // }
    // std::cout << std::endl;
    // if (server->getIndexVec() != NULL)
    // {
    //     std::cout << "has indeces" << std::endl;
    // }
    
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