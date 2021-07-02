#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <cstring>
#include <string>
#include <iostream>
#include <map>
#include "requestParsing.hpp"

class Response {
private:
	std::string							_response;
	std::string							_code;
	size_t								_responseLen;
	RequestParsing						_parsedReq;

public:
	Response(std::string code, RequestParsing req): _response(""), _code(code), _responseLen(0), _parsedReq(req) {};
	Response(Response const &copy): _parsedReq(copy._parsedReq) {*this = copy; return;};
	~Response() {};
	Response		&operator=(Response const &equal_op);
	std::string		getResponse() const {return _response;}
	size_t			getResponseLen() const {return _responseLen;}
	std::string		generateResponse();

private:
	Response();
};

std::ostream &operator<<(std::ostream &out, Response &x);

#endif