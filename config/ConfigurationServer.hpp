/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 14:46:43 by bbelen            #+#    #+#             */
/*   Updated: 2021/06/05 14:46:43 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "ConfigurationLocation.hpp"

typedef struct  s_listen
{
    int         host;
    int         port;
}               t_listen;

class ConfigurationServer
{
    private:
        std::vector<t_listen>               listenVec;
        std::string                         root;
        std::vector<std::string>            serverNameVec;
        std::vector<ConfigurationLocation>  locationVec;
        std::set<std::string>               methods;
        bool                                autoIndex;
    
    public:
        ConfigurationServer();
        virtual ~ConfigurationServer();
        ConfigurationServer(const ConfigurationServer &config);

        ConfigurationServer &operator=(const ConfigurationServer &config);

        void    addListen(t_listen  listen);
        void    setRoot(std::string root);
        void    addServerName(std::string serverName);
        void    addLocation(ConfigurationLocation location);
        void    addMethod(std::string method);
        void    setAutoIndex(bool autoIndex);

        std::vector<t_listen>       getListenVec();
        std::vector<std::string>    getServerNameVec();
};