/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MapConfigFile.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/26 19:45:39 by bbelen            #+#    #+#             */
/*   Updated: 2021/06/27 17:46:50 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MapConfigFile.hpp"

MapConfigFile::MapConfigFile() : bracketOpen(0), bracketClose(0), blockName("") 
{

}

MapConfigFile::MapConfigFile(MapConfigFile const &map)
: bracketOpen(map.bracketOpen), bracketClose(map.bracketClose), blockName(map.blockName)
{
	
}

MapConfigFile::~MapConfigFile()
{

}

MapConfigFile &MapConfigFile::operator=(MapConfigFile const &map)
{
	this->bracketOpen = map.bracketOpen;
	this->bracketClose = map.bracketClose;
	this->blockName = map.blockName;

	return *this;
}

void		MapConfigFile::addBracketOpen(int value)
{
	this->bracketOpen += value;
}

void		MapConfigFile::addBracketClose(int value)
{
	this->bracketClose += value;
}

void		MapConfigFile::setBlockName(std::string blockName)
{
	this->blockName = blockName;
}

void		MapConfigFile::resetMap()
{
	this->bracketOpen = 0;
	this->bracketClose = 0;
	this->blockName = "";
}

int			MapConfigFile::getBracketsOpen()
{
	return this->bracketOpen;
}

int			MapConfigFile::getBracketClose()
{
	return this->bracketClose;
}

std::string	MapConfigFile::getBlockName()
{
	return this->blockName;
}

bool		MapConfigFile::checkBrackets()
{
	return this->bracketOpen == this->bracketClose;
}

bool		MapConfigFile::checkBlockName()
{
	if (this->blockName == "server")
	{
		return true;
	}
	return false;
}