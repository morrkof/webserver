/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndexPage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppipes <ppipes@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/10 15:19:22 by bbelen            #+#    #+#             */
/*   Updated: 2021/07/14 01:18:12 by ppipes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndexPage.hpp"

AutoIndexPage::AutoIndexPage(){}

AutoIndexPage::AutoIndexPage(std::string path)
{
	std::vector<std::string> files;
	DIR *dir;
	dirent *file;

	dir = opendir(path.c_str());
    if (dir)
    {
        while ((file = readdir(dir)) != 0)
        {
            files.push_back(file->d_name);
        }
        closedir(dir);
        std::sort(files.begin(), files.end());

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
        this->isDir = true;
    }
    else
		this->isDir = false;
    
}

AutoIndexPage::AutoIndexPage(AutoIndexPage const &page)
{
	this->page = page.page;
    this->isDir = page.isDir;
}

AutoIndexPage::~AutoIndexPage(){}

AutoIndexPage &AutoIndexPage::operator=(AutoIndexPage const &page)
{
    this->page = page.page;
    this->isDir = page.isDir;

	return *this;
}

std::string    AutoIndexPage::getPage() { return this->page; }

bool AutoIndexPage::getIsDir() { return this->isDir; }