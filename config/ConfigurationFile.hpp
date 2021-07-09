/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:36:10 by bbelen            #+#    #+#             */
/*   Updated: 2021/07/09 19:45:37 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "ConfigurationServer.hpp"
#include "MapConfigFile.hpp"

class ConfigurationFile
{
    private:
        std::vector<ConfigurationServer>    *serverVec;
    
    public:
        ConfigurationFile();
        virtual ~ConfigurationFile();
        ConfigurationFile(const ConfigurationFile &file);

        ConfigurationFile &operator=(const ConfigurationFile &file);

        void                                addServer(ConfigurationServer *server);
        std::vector<ConfigurationServer>    *getServers();
        void                                parseFile(std::string filename);
        void                                checkConfigBlock(MapConfigFile &map, std::vector<std::string> &block);
        void		                        parseBlockLine(std::vector<std::string> line, ConfigurationServer *server);

    private:
        bool                                lineOnlySpacesOrTabs(std::string line);
    
    public:
        class ConfigFileNotFoundException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

        class ConfigFileParserException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

		class ConfigFileFormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

};