/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndexPage.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/10 14:57:15 by bbelen            #+#    #+#             */
/*   Updated: 2021/07/10 15:38:31 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>

class AutoIndexPage
{
	private:
		std::string                 page;

        AutoIndexPage();
	
	public:
        AutoIndexPage(std::vector<std::string> files);
		AutoIndexPage(AutoIndexPage const &page);
		virtual ~AutoIndexPage();

		AutoIndexPage &operator=(AutoIndexPage const &page);

		std::string    getPage();
};