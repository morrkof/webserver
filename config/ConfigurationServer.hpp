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

struct  listen
{
    //int         host;
    int         port;
};

struct location
{
    std::string         route;
};

struct returnAddress
{
    int         errorCode;
    std::string address;
};


class ConfigurationServer
{
    private:
        std::vector<listen>                 listenVec;
        std::string                         root;
        std::vector<std::string>            serverNameVec;
        std::vector<location>               locationVec;
        std::set<std::string>               methods;
        bool                                autoIndex;
        returnAddress                       returnAddr;
    
    public:
        ConfigurationServer();
        virtual ~ConfigurationServer();
        ConfigurationServer(const ConfigurationServer &config);

        ConfigurationServer &operator=(const ConfigurationServer &config);

        void    addListen(listen  listen);
        void    setRoot(std::string root);
        void    addServerName(std::string serverName);
        void    addLocation(location location);
        void    addMethod(std::string method);
        void    setAutoIndex(bool autoIndex);
        void    addReturnAddress(returnAddress returnAddr);

        std::vector<listen>       getListenVec();
        std::vector<std::string>    getServerNameVec();

        void    parseListen(std::vector<std::string> &line);
        void    parseServerName(std::vector<std::string> &line);
        void    parseRoot(std::vector<std::string> &line);
        void    parseIndex(std::vector<std::string> &line);
        void    parseLocation(std::vector<std::string> &line);
        void    parseReturn(std::vector<std::string> &line);

        
};