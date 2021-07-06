#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <fstream>		// std::ifstream
#include <stdlib.h>
#include <vector>
#include "requestParsing.hpp"

class Response {
private:
	std::string							_response;
	std::string							_body;
	std::string							_code;
	std::string							_version;
	size_t								_responseLen;
	RequestParsing						_parsedReq;

public:
	Response(RequestParsing req): _response("") ,_body(""), _code("200 ok"), _version(req.getVersion()), _responseLen(0), _parsedReq(req) {parseResponse();};
	Response(Response const &copy): _parsedReq(copy._parsedReq) {*this = copy; return;};
	~Response() {};
	Response() {}
	Response		&operator=(Response const &equal_op);
	std::string		getResponse() const {return _response;}
	std::string		getBody() const {return _body;}
	size_t			getResponseLen() const {return _responseLen;}
	// std::string		generateBodyFile(std::string source);
	int				generateBody();
	std::string		generateResponse(std::string x);
	std::string		parseResponse();

};

std::ostream &operator<<(std::ostream &out, Response &x);

#endif