#include <string>
#include <iostream>
#include <fstream>

std::string cgi_proceed(std::string body)
{
	std::string result;
	return result;
}

int main()
{
	std::string body;
	std::string buf;
	std::ifstream	ifs("/home/anastasia/Desktop/webserv/sites/dynamic/index.php");
	if (ifs.is_open() == 0) {
			std::cout << "file doesn't exist" << std::endl;
			return 1;
		}
	while (getline(ifs, buf)) {

		body += buf;
		if (ifs.eof())
			break;
		body += "\n";
	}
	ifs.close();
	
	// std::cout << "!!!!!!!!!!!!!  BODY !!!!!!!!!!!\n";
	// std::cout << body << std::endl;;
	// std::cout << "!!!!!!!!!!!!!  BODY !!!!!!!!!!!\n";
	cgi_proceed(body);
}