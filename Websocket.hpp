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
	std::vector<ConfigurationServer>	*_serversVec;
	char **_env;

public:
	Websocket(int socket, socket_type type, std::vector<ConfigurationServer> *servers, char **env);
	~Websocket();
	Websocket();

	int getSocket(void) const;
	socket_type getType(void) const;
	RequestParsing &getRequest();
	Response &getResponse();
	std::string getResponseChars();
	size_t getResponseLen();

	void setType(socket_type type);
	void setRequest(std::string buf);
};

bool compare_ws(Websocket *lhs, Websocket *rhs);

#endif