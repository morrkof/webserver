/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndexPage.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/10 14:57:15 by bbelen            #+#    #+#             */
/*   Updated: 2021/07/12 16:16:59 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <dirent.h>


class AutoIndexPage
{
	private:
		std::string                 page;
		bool						isDir;

        AutoIndexPage();
	
	public:
        AutoIndexPage(std::string path);
		AutoIndexPage(AutoIndexPage const &page);
		virtual ~AutoIndexPage();

		AutoIndexPage &operator=(AutoIndexPage const &page);

		std::string    getPage();
};