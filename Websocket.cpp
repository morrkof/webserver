#include "Websocket.hpp"

Websocket::Websocket(int socket, socket_type type, ConfigurationServer *server, char **env)
: _socket(socket), _send_offset(0), _type(type), _server(server), _recv_buf(""), _env(env) {}
Websocket::~Websocket() { close(_socket);}

int Websocket::getSocket(void) const { return _socket; }
socket_type Websocket::getType(void) const { return _type; }
RequestParsing &Websocket::getRequest() { return _request; }
Response &Websocket::getResponse() { return _response; }
std::string Websocket::getResponseChars() { return _response.getResponse();}
size_t Websocket::getResponseLen() { return _response.getResponseLen(); }
int Websocket::getSendOffset() { return _send_offset; }
std::string Websocket::getRecvBuf() { return _recv_buf; }
void Websocket::setSendOffset(int bytes) { _send_offset += bytes; }
void Websocket::setRecvBuf(std::vector<char> buf, size_t size) { _recv_buf.append(buf.begin(),buf.begin() + size); }

void Websocket::setType(socket_type type) { _type = type; }
void Websocket::setRequest(std::string buf) 
{
	_type = WRITE; 
	_request = RequestParsing(buf);
	_response = Response(_request, _server, _env);
}

bool compare_ws(Websocket *lhs, Websocket *rhs)
{
	return (lhs->getSocket() < rhs->getSocket());
}