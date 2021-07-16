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
	size_t _send_offset;
	socket_type _type;
	RequestParsing _request;
	Response _response;
	ConfigurationServer	*_server;
	std::string _recv_buf;
	char **_env;

public:
	Websocket(int socket, socket_type type, ConfigurationServer *server, char **env);
	~Websocket();
	Websocket();

	int getSocket(void) const;
	socket_type getType(void) const;
	RequestParsing &getRequest();
	Response &getResponse();
	std::string getResponseChars();
	size_t getResponseLen();
	ConfigurationServer *getServer() { return _server; }
	int getSendOffset();
	std::string getRecvBuf();
	void setSendOffset(int bytes);
	void setRecvBuf(std::vector<char> buf, size_t size);

	void setType(socket_type type);
	void setRequest(std::string buf);
};

bool compare_ws(Websocket *lhs, Websocket *rhs);

#endif