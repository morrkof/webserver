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
	std::string							_errCodeStr;
	int									_errCode;
	std::string							_version;
	std::string							_contentType;
	size_t								_responseLen;
	RequestParsing						_parsedReq;
	std::vector<ConfigurationServer>	*_serversVec;
	char 								**_env;
	std::string							_csMethod;
	std::string							_csRoot;
	std::string							_csRoute;

public:
	Response(RequestParsing req, std::vector<ConfigurationServer> *server, char **env): _response("") ,_body(""), _errCodeStr("200 OK"), _errCode(200), _version(req.getVersion()), _responseLen(0), _parsedReq(req), _serversVec(server), _env(env)
	{ (void)_env; setVariables();chooseMethod();};
	Response(Response const &copy): _parsedReq(copy._parsedReq) {*this = copy; return;};
	~Response() {};
	Response() {}
	Response		&operator=(Response const &equal_op);
// cat errors
	std::string		cgiCatGeneratePage(int code);
// generate variables
	void			printConfigurationServer();
	std::string		generateContentType();
	void			setVariables() {printConfigurationServer();generateContentType();}
// getters
	std::string		getResponse() const {return _response;}
	std::string		getBody() const {return _body;}
	size_t			getResponseLen() const {return _responseLen;}
// GET
	int				generateBody(const char* streamPath, int errCode);
	int				methodGetFormBody();
	std::string		generateResponse();
// DELETE
	int				methodDelete();
// choose method
	int				chooseMethod();

};

std::ostream &operator<<(std::ostream &out, Response &x);

#endif