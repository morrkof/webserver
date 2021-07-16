#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <fstream>		// std::ifstream
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <algorithm>
#include "requestParsing.hpp"
#include "AutoIndexPage.hpp"
#include "config/ConfigurationServer.hpp"

class Response {
private:
	std::string							_response;
	std::string							_body;
	std::string							_errCodeStr;
	int									_errCode;
	std::string							_version;
	std::string							_contentType;
	std::string							_locationType;
	size_t								_responseLen;
	RequestParsing						_parsedReq;
	ConfigurationServer					*_server;
	std::string							_csMethod;
	std::string							_csRoot;
	std::string							_csRoute;
	location							_location;
	std::string							_hostname;

public:
	Response(RequestParsing req, ConfigurationServer *server);
	Response(Response const &copy): _parsedReq(copy._parsedReq) {*this = copy; return;};
	~Response() {};
	Response() {}
	Response		&operator=(Response const &equal_op);

// choose method
	void			chooseMethod();
	std::string		cgi_process(std::string path, std::string filename, std::string params);

// generate variables
	std::string		CatGeneratePage(int code);
	std::string		generateContentType();

// getters
	std::string		getResponse() const {return _response;}
	std::string		getBody() const {return _body;}
	size_t			getResponseLen() const {return _responseLen;}

// body and response
	int				generateBody(const char* streamPath);
	void			methodGetFormBody();
	std::string		generateResponse();
	
// METHODS
	void			methodGet();
	void			methodPost();
	void			methodDelete();
};

std::ostream &operator<<(std::ostream &out, Response &x);

#endif