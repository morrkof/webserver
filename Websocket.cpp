#include "Websocket.hpp"

Websocket::Websocket(int socket, socket_type type)
: _socket(socket), _type(type) {}
Websocket::~Websocket() { close(_socket);}

int Websocket::getSocket(void) const { return _socket; }
socket_type Websocket::getType(void) const { return _type; }
RequestParsing &Websocket::getRequest() { return _request; }
Response &Websocket::getResponse() { return _response; }
std::string Websocket::getResponseChars() { return _response.getResponse();}
size_t Websocket::getResponseLen() { return _response.getResponseLen(); }

void Websocket::setType(socket_type type) { _type = type; }
void Websocket::setRequest(std::string buf) 
{ 
	_type = WRITE; 
	_request = RequestParsing(buf); 
	_response = Response("200 Ok", _request); 
}

bool compare_ws(Websocket *lhs, Websocket *rhs)
{
	return (lhs->getSocket() < rhs->getSocket());
}