/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:02:10 by bbelen            #+#    #+#             */
/*   Updated: 2021/07/07 08:33:01 by bbelen           ###   ########.fr       */
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
 * 		std::vector<t_listen>               listenVec; - для списка портов
 *      std::string                         root; - корневой каталог
 *      std::vector<std::string>            serverNameVec; - список имен серверов (может быть несколько)
 *      std::vector<location>               locationVec; - список рутов (маршрутов)
 *      std::set<std::string>               methods; - методы
 *      std::vector<std::string>            indexVec; - возможные стартовые страницы
 *      returnAddress                       returnAddr; - в случае перенаправления
 * 
 * Структура t_listen:
 * 
 * 		typedef struct  s_listen
 * 		{
 * 		   int         port;
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
