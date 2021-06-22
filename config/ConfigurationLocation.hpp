/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationLocation.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:01:38 by bbelen            #+#    #+#             */
/*   Updated: 2021/06/05 15:01:38 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "ConfigurationServer.hpp"

class ConfigurationLocation
{
    private:
        std::string         route;
        ConfigurationServer server;
        
    public:
        ConfigurationLocation();
        virtual ~ConfigurationLocation();
        ConfigurationLocation(const ConfigurationLocation &configLoc);

        ConfigurationLocation &operator=(const ConfigurationLocation &configLoc);

        void    setRoute(std::string root);
        void    setServer(ConfigurationServer server);

        std::string getRoot();
        ConfigurationServer getServer();
};