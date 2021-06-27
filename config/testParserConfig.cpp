/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testParserConfig.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbelen <bbelen@21-school.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/26 20:10:20 by bbelen            #+#    #+#             */
/*   Updated: 2021/06/26 20:11:45 by bbelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigurationFile.hpp"

int main(void)
{
	ConfigurationFile	parserFile;

	parserFile.parseFile("./configs/basic_config.conf");

	return 0;
}