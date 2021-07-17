/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:02:10 by bbelen            #+#    #+#             */
/*   Updated: 2021/07/17 18:35:58 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**
 * Основной хэдер для всех конфигов и парсеров
 * 
 * Общий гайд:
 * 
 * Два основных класса: ConfigurationFile, ConfigurationServer
 * Основное взаимодействие через ConfigurationFile. Его интересные поля:
 * 
 * std::vector<ConfigurationServer>    serverVec;
 * 
 * Это сервер векторов. Егоможно получить через соответствующий геттер:
 * 
 * std::vector<ConfigurationServer>    getServers();
 * 
 * Соответственно, класс ConfigurationServer состоит из:
 * 
 * 		std::string                 route;
 * 		std::string                 root;
 *    	bool                        autoindex;
 *   	std::vector<std::string>    try_files;
 *   	bool                        finished;
 *   	int                         errorCode;
 *   	std::string                 fastcgi_include;
 *   	std::string                 fastcgi_pass;
 *   	std::set<std::string>       methods;
 *   	int                         client_body_size;
 * 
 * Структура t_listen:
 * 
 * 		typedef struct  s_listen
 * 		{
 * 			std::string         host;
 * 		    int         port;
 * 		}       t_listen; (там могут понадобиться доп поля, так что структура)
 * 
 * Структура location:
 * 
 * 		struct location
 * 		{
 * 	    	std::string                 route; - сам рут для маршрута
 * 	    	bool                        autoindex;
 * 	    	std::vector<std::string>    try_files; - какие файлы перебирает
 * 	    	int                         errorCode; - код ошибки, если файлов нет
 * 			std::string                 		fastcgi_include;
 * 			std::string                 		fastcgi_pass;
 * 	    	std::set<std::string>       		methods;
 * 	    	int                         		client_body_size;
 * 		};
 * 
 */

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <dirent.h>

#define SYNTAX_ERROR	1
#define FILE_ERROR		2