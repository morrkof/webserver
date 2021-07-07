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

typedef struct  s_listen
{
    //int         host;
    int         port;
}       t_listen;

struct location
{
    std::string                 route;
    bool                        autoindex;
    std::vector<std::string>    try_files;
    bool                        finished;
    int                         errorCode;
    std::string                 fastcgi_include;
    std::string                 fastcgi_pass;
    std::set<std::string>       methods;
    int                         client_body_size;
};

struct returnAddress
{
    int         errorCode;
    std::string address;
};


class ConfigurationServer
{
    private:
        std::vector<t_listen>               listenVec;
        std::string                         root;
        std::vector<std::string>            serverNameVec;
        std::vector<location>               locationVec;
        std::set<std::string>               methods;
        std::vector<std::string>            indexVec;
        bool                                autoIndex;
        returnAddress                       returnAddr;
    
    public:
        ConfigurationServer();
        virtual ~ConfigurationServer();
        ConfigurationServer(const ConfigurationServer &config);

        ConfigurationServer &operator=(const ConfigurationServer &config);

        void    addListen(t_listen  listen);
        void    setRoot(std::string root);
        void    addServerName(std::string serverName);
        void    addLocation(location location);
        void    addMethod(std::string method);
        void    setAutoIndex(bool autoIndex);
        void    addReturnAddress(returnAddress returnAddr);
        void    addIndex(std::string index);

        std::vector<t_listen>         getListenVec();
        std::vector<std::string>    getServerNameVec();
        std::string                 getRoot();
        std::vector<location>       getLocationVec();
        location&                   getLastLocation();
        std::set<std::string>       getMethods();
        std::vector<std::string>    getIndexVec();
        returnAddress               getReturnAddress();

        void    parseListen(std::vector<std::string> &line);
        void    parseServerName(std::vector<std::string> &line);
        void    parseRoot(std::vector<std::string> &line);
        void    parseIndex(std::vector<std::string> &line);
        void    parseLocation(std::vector<std::string> &line);
        void    parseReturn(std::vector<std::string> &line);
        void    parseMethods(std::vector<std::string> &line);
        void    updateLocation(std::vector<std::string> &line);

        class ServerParserException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

        class ServerPortException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

        class ServerNameException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

        class ServerIndexException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

std::ostream &operator<<(std::ostream &os, ConfigurationServer &server);