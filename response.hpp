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
	size_t								_responseLen;
	RequestParsing						_parsedReq;
	ConfigurationServer					*_server;
	char 								**_env;
	std::string							_csMethod;
	std::string							_csRoot;
	std::string							_csRoute;

public:
	Response(RequestParsing req, ConfigurationServer *server, char **env)
	: _response("") ,_body(""), _errCodeStr("200 OK"), _errCode(200), 
	_version(req.getVersion()), _responseLen(0), _parsedReq(req), _server(server), _env(env)
	// { (void)_env; setVariables();chooseMethod();}
	{ (void)_env;chooseMethod(); generateContentType();}

	Response(Response const &copy): _parsedReq(copy._parsedReq) {*this = copy; return;};
	~Response() {};
	Response() {}
	Response		&operator=(Response const &equal_op);

// choose method
	void			chooseMethod();
	std::string cgi_process(std::string path, std::string filename, std::string params, char **env);

// generate variables
	std::string		CatGeneratePage(int code);
	// void			printConfigurationServer();
	std::string		generateContentType();
	// void			setVariables() {printConfigurationServer();generateContentType();}

// getters
	std::string		getResponse() const {return _response;}
	std::string		getBody() const {return _body;}
	size_t			getResponseLen() const {return _responseLen;}

// GET
	int				generateBody(const char* streamPath, int errCode);
	int				methodGetFormBody();
	std::string		generateResponse();

// METHODS
	void			methodGet();
	void			methodPost();
	void			methodDelete();



};

std::ostream &operator<<(std::ostream &out, Response &x);

#endif