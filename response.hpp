#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <fstream>		// std::ifstream
#include <stdlib.h>
#include <vector>
#include <map>
#include "requestParsing.hpp"
#include "config/ConfigurationServer.hpp"

class Response {
private:
	std::string							_response;
	std::string							_body;
	std::string							_errCode;
	std::string							_version;
	std::string							_contentType;
	size_t								_responseLen;
	RequestParsing						_parsedReq;
	std::vector<ConfigurationServer>	*_serversVec;
	char 								**_env;

public:
	Response(RequestParsing req, std::vector<ConfigurationServer> *server, char **env): _response("") ,_body(""), _errCode("200 ok"), _version(req.getVersion()), _responseLen(0), _parsedReq(req), _serversVec(server), _env(env)
	{ (void)_env; prr();generateContentType();parseResponse();};
	Response(Response const &copy): _parsedReq(copy._parsedReq) {*this = copy; return;};
	~Response() {};
	Response() {}
	Response		&operator=(Response const &equal_op);
	void prr();
	std::string		getResponse() const {return _response;}
	std::string		getBody() const {return _body;}
	size_t			getResponseLen() const {return _responseLen;}
	std::string		generateContentType();
	int				generateBody(const char* streamPath, std::string errCode);
	int				parseBody();
	std::string		generateResponse();
	std::string		parseResponse();

};

std::ostream &operator<<(std::ostream &out, Response &x);

#endif