#ifndef WEBSOCKET_HPP
#define WEBSOCKET_HPP

#include <unistd.h>
#include <iostream>
#include "requestParsing.hpp"
#include "response.hpp"

enum socket_type {
	LISTEN,	// 0
	READ,	// 1
	WRITE	// 2
};

class Websocket 
{
private:
	int _socket;
	socket_type _type;
	RequestParsing _request;
	Response _response;

public:
	Websocket(int socket, socket_type type)
	: _socket(socket), _type(type) {}
	~Websocket() { close(_socket);}

	int getSocket(void) const { return _socket; }
	socket_type getType(void) const { return _type; }
	RequestParsing &getRequest() { return _request; }
	std::string getResponseChars() { return _response.getResponse();}
	size_t getResponseLen() { return _response.getResponseLen(); }

	void setType(socket_type type) { _type = type; }
	void setRequest(std::string buf) { _type = WRITE; _request = RequestParsing(buf); _response = Response("200 Ok", _request); }


	
};

bool compare_ws(Websocket *lhs, Websocket *rhs)
{
	return (lhs->getSocket() < rhs->getSocket());
}

#endif