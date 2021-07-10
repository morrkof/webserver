/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndexPage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/10 15:19:22 by bbelen            #+#    #+#             */
/*   Updated: 2021/07/10 15:39:34 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndexPage.hpp"

AutoIndexPage::AutoIndexPage()
{

}

AutoIndexPage::AutoIndexPage(std::vector<std::string> files)
{
    std::string preBody = "<html lang=\"en\"><body style=\"background-color:#ece8df; color:#0e0803\">";
    std::string postBody = "</body></html>";
    this->page = "";
    
    std::vector<std::string> filesVec;
    std::vector<std::string>::iterator it = files.begin();
    std::vector<std::string>::iterator itE = files.end();

    for (; it != itE; it++)
    {
        filesVec.push_back("<a href=\"" + *it + "\">" + *it + "</a><br>");
    }

    this->page = this->page + preBody;
    for (it = filesVec.begin(); it != filesVec.end(); it++)
    {
        this->page += *it;
    }

    this->page += postBody;
}

AutoIndexPage::AutoIndexPage(AutoIndexPage const &page)
{
	this->page = page.page;
}

AutoIndexPage::~AutoIndexPage()
{

}

AutoIndexPage &AutoIndexPage::operator=(AutoIndexPage const &page)
{
    this->page = page.page;

	return *this;
}

std::string    AutoIndexPage::getPage()
{
    return this->page;
}