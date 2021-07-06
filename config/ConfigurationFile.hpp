/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:36:10 by bbelen            #+#    #+#             */
/*   Updated: 2021/06/27 19:08:23 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "ConfigurationServer.hpp"
#include "MapConfigFile.hpp"

class ConfigurationFile
{
    private:
        std::vector<ConfigurationServer>    serverVec;
    
    public:
        ConfigurationFile();
        virtual ~ConfigurationFile();
        ConfigurationFile(const ConfigurationFile &file);

        ConfigurationFile &operator=(const ConfigurationFile &file);

        void                                addServer(ConfigurationServer server);
        std::vector<ConfigurationServer>    getServers();
        void                                parseFile(std::string filename);
        void                                checkConfigBlock(MapConfigFile &map, std::vector<std::string> &block);
        void		                        parseBlockLine(std::vector<std::string> line, ConfigurationServer &server);

    private:
        bool                                lineOnlySpacesOrTabs(std::string line);
};