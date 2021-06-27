/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MapConfigFile.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/26 19:38:19 by bbelen            #+#    #+#             */
/*   Updated: 2021/06/27 17:47:02 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"

class MapConfigFile
{
	private:
		int			bracketOpen;
		int			bracketClose;
		std::string	blockName;
	
	public:
		MapConfigFile();
		MapConfigFile(MapConfigFile const &map);
		virtual ~MapConfigFile();

		MapConfigFile &operator=(MapConfigFile const &map);

		void		addBracketOpen(int value);
		void		addBracketClose(int value);
		void		setBlockName(std::string blockName);
		void		resetMap();

		int			getBracketsOpen();
		int			getBracketClose();
		std::string	getBlockName();

		bool		checkBrackets();
		bool		checkBlockName();
		
};