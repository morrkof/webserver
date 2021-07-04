#ifndef REQUEST_PARSING_HPP
#define REQUEST_PARSING_HPP
#include <cstring>
#include <string>
#include <iostream>
#include <map>

class RequestParsing {
private:
	std::string							_method;
	std::string							_location;
	std::string							_version;
	std::string							_body;
	std::map<std::string,std::string>	_mapHeaders;

	std::string							_dataFull;
	std::string							_dataNext;			// shows an unparsed piece of data
	size_t								_posNext;			// shows the place of iterator for the next argument

public:
	RequestParsing(std::string str): _dataFull(str) {parseMeLikeYouDo();}
	RequestParsing(RequestParsing const &copy) {*this = copy; return;}
	~RequestParsing() {}
	RequestParsing() {}
	RequestParsing	&operator=(RequestParsing const &equal_op);
	std::string		getMethod() const;
	std::string		getLocation() const;
	std::string		getVersion() const;
	std::map<std::string,std::string>	&getMapHeaders();
	std::string		getBody() const;

private:
	
	int			parseMeLikeYouDo();
	void		_errorMethodDoesntExist();
	int			chooseMethod();
	int			chooseLocation();
	int			chooseVersion();
	int			fillMapHeaders();
	int			chooseBody();
};

std::ostream &operator<<(std::ostream &out, RequestParsing &x);

#endif