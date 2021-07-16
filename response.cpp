#include "response.hpp"

Response::Response(RequestParsing req, ConfigurationServer *server)
: _response("") ,_body(""), _errCodeStr("200 OK"), _errCode(200), _version(req.getVersion()), 
_contentType("text/html"), _responseLen(0), _parsedReq(req), _server(server)
{
	bool is_server = false;
	_location = _server->getLocationVec()[0];
	for (std::vector<location>::iterator it = _server->getLocationVec().begin(); it != _server->getLocationVec().end(); ++it)
	{
		if (_parsedReq.getLocation() == (*it).route)
			_location = (*it);
	}
	_hostname = _parsedReq.getMapHeaders()["Host"].substr(0, _parsedReq.getMapHeaders()["Host"].find(':'));
	for (std::vector<std::string>::iterator it = _server->getServerNameVec().begin(); it != _server->getServerNameVec().end(); ++it)
	{
		if (_hostname == (*it))
			is_server = true;
	}
	if (is_server)
		chooseMethod(); 
	else
	{
		_errCode = 404;
		_errCodeStr = "404 Not Found";
		_contentType = "text/html";
		generateResponse();
	}
}


void Response::chooseMethod() 
{
	generateContentType();
	_csMethod = _parsedReq.getMethod();
	if (_csMethod == "GET") methodGet();
	else if (_csMethod == "POST") methodPost();
	else if (_csMethod == "DELETE") methodDelete();
	else {
		_errCode = 400;
		_errCodeStr = "400 Bad Request";
		_contentType = "text/html";
		generateResponse();
	}
}


int Response::generateBody(const char* streamPath) 
{
	std::string		buf;
	std::ifstream	ifs(streamPath);

	if (ifs.is_open() == 0) 
		return 1;
	while (!ifs.eof()) 
	{
		std::getline(ifs, buf);
		_body.append(buf);
		_body.append("\n");
	}
	ifs.close();
	return 0;
}


void Response::methodGetFormBody() 
{
	if (_server->getLocationVec()[0].autoindex) 
	{
		AutoIndexPage page(_server->getRoot() + _parsedReq.getLocation());
		if (page.getIsDir())
			_body = page.getPage();
		else
		{
			if (generateBody((_server->getRoot() + _parsedReq.getLocation()).c_str()) == 1)
			{
				_errCode = 404;
				_errCodeStr = "404 Not Found";
				_contentType = "text/html";
			}
		}
	}
	else if (_server->getReturnAddress().address.size())
	{
		_errCode = 301;
		_errCodeStr = "301 Moved Permanently";
		_contentType = "text/html";
		_locationType = _server->getReturnAddress().address;
	}
	else
	{
		if (_parsedReq.getLocation() == "/") 
		{	
			if (_server->getIndexVec()[0].find(".php") != std::string::npos)
				_body = cgi_process(_location.fastcgi_pass, _server->getRoot() + "/" + _server->getIndexVec()[0], "");
			else if (generateBody((_server->getRoot() + "/" + _server->getIndexVec()[0]).c_str()) == 1)
			{
				_errCode = 404;
				_errCodeStr = "404 Not Found";
			}
		}
		else 
		{
			size_t i = _parsedReq.getLocation().find(".php?");
			if (_parsedReq.getLocation().find(".php") != std::string::npos)
			{
				std::string params("");
				if (i != std::string::npos)
					params = _parsedReq.getLocation().substr(i+5, _parsedReq.getLocation().length()-i-5);
				_body = cgi_process(_location.fastcgi_pass, _server->getRoot() + "/" + _server->getIndexVec()[0], params);
			}												
			else if (generateBody((_server->getRoot() + _parsedReq.getLocation()).c_str()) == 1) 
			{
				_errCode = 404;
				_errCodeStr = "404 Not Found";
			}
		}
	}
}


void Response::methodGet() 
{
	if (_server->getMethods().count("GET"))
		methodGetFormBody();
	else {
		_errCode = 405;
		_errCodeStr = "405 Method Not Allowed";
		_contentType = "text/html";
	}
	generateResponse();
}


void Response::methodPost() 
{
	if (_server->getMethods().count("POST"))
	{
		_contentType = "text/html";
		if (_location.client_body_size != -1)
		{
			if (_parsedReq.getBody().size() > (size_t)_location.client_body_size)
			{
				_errCode = 413;
				_errCodeStr = "413 Request Entity Too Large";
			}
		}
		else
		{
			if (_parsedReq.getMapHeaders().count("Content-Type"))
			{
				if (_parsedReq.getMapHeaders()["Content-Type"] == "application/x-www-form-urlencoded")
				{
					_body = cgi_process(_location.fastcgi_pass, _server->getRoot() + _parsedReq.getLocation(), _parsedReq.getBody());
					if (_body.size() == 0)
					{
						_errCode = 502;
						_errCodeStr = "502 Bad Gateway";
					}
				}
				else
				{
					std::ofstream ofs((_server->getRoot() + _parsedReq.getLocation()).c_str());
					if (!ofs)
					{
						_errCode = 500;
						_errCodeStr = "500 Internal Server Error";
					}
					else
					{
						ofs << _parsedReq.getBody();
						_errCode = 201;
						_errCodeStr = "201 Created";
					}
				}
			}
		}
	}
	else 
	{
		_errCode = 405;
		_errCodeStr = "405 Method Not Allowed";
		_contentType = "text/html";
	}
	generateResponse();
}


void Response::methodDelete() 
{
	if (_server->getMethods().count("DELETE"))
	{
		_csRoot = _server->getRoot();
		std::string		fileToRemove = _csRoot + _parsedReq.getLocation();
		if (remove(fileToRemove.c_str()) != 0) {
			_errCode = 204;
			_errCodeStr = "204 No Content";
		}
		else {
			_errCode = 202;
			_errCodeStr = "202 Accepted";
		}
	}
	else {
		_errCode = 405;
		_errCodeStr = "405 Method Not Allowed";
	}
	_contentType = "text/html";
	generateResponse();
}


std::string	Response::generateResponse() 
{
	if (_errCode != 200)
		_body = CatGeneratePage(_errCode);

	_response.append(_version);
	_response.append(" ");
	_response.append(_errCodeStr);
	_response.append("\n");

	_response.append("Server: ");
	_response.append("Troglodits 4.2 Salty Salami");
	_response.append("\n");

	_response.append("Content-Type: ");	
	_response.append(_contentType);
	_response.append("\n");

	_response.append("Content-Length: ");	
	_response.append(static_cast< std::ostringstream & >((std::ostringstream() << std::dec << _body.length())).str());
	_response.append("\n");

	if (_errCode == 301)
	{
		_response.append("Location: ");
		_response.append(_locationType);
		_response.append("\n");
	}

	if (_errCode == 405)
	{
		std::string _allowMethodsServ;
		std::set<std::string> tmp = _server->getMethods();
		for (std::set<std::string>::iterator it = tmp.begin(); it != tmp.end(); ++it)
		{
			_allowMethodsServ += (*it);
			_allowMethodsServ += " ";
		}
		_response.append("Public: ");
		_response.append(_allowMethodsServ);
		_response.append("\n");

		std::string _allowMethods;
		for (std::set<std::string>::iterator it = _location.methods.begin(); it != _location.methods.end(); ++it)
		{
			_allowMethods += (*it);
			_allowMethods += " ";
		}
		_response.append("Allow: ");
		_response.append(_allowMethods);
		_response.append("\n");
	}

	_response.append("\n");
	_response.append(_body);
	_responseLen = _response.length();
	return _response;
}


std::string	Response::generateContentType()
{
	std::string	extension(_parsedReq.getLocation());
	std::map<std::string, std::string>	types;
	types[".txt"] = "text/plain";
	types[".bin"] = "application/octet-stream";
	types[".jpeg"] = "image/jpeg";
	types[".jpg"] = "image/jpeg";
	types[".html"] = "text/html";
	types[".htm"] = "text/html";
	types[".png"] = "image/png";
	types[".bmp"] = "image/bmp";
	types[".pdf"] = "application/pdf";
	types[".tar"] = "application/x-tar";
	types[".json"] = "application/json";
	types[".css"] = "text/css";
	types[".js"] = "application/javascript";
	types[".mp3"] = "audio/mpeg";
	types[".avi"] = "video/x-msvideo";
	types[".otf"] =  "font/opentype";
	types[".svg"] =  "image/svg+xml";
	if (extension.rfind(".") == std::string::npos)
		return _contentType;
	extension = extension.substr(extension.rfind("."), extension.npos);
	for(std::map<std::string,std::string>::iterator it = types.begin(); it != types.end(); ++it) {
		if (extension.compare(it->first) == 0)
			_contentType = it->second;
	}
	return _contentType;
}


std::string	Response::CatGeneratePage(int code) 
{
	std::string result;
	result.append("<html>");
	result.append("<body style=\"background-color:#000000\">");
	result.append("<div style=\"display: flex; align-items: center; justify-content: center; height: 100%; flex-direction: column;\">");
	result.append("<div> <img src=\"https://http.cat/");
	result.append(static_cast< std::ostringstream & >((std::ostringstream() << std::dec << code)).str());
	result.append(".jpg\"></div></div></body></html>");
	return result; 
}


std::string Response::cgi_process(std::string path, std::string filename, std::string params)
{
	std::string result;
	pid_t pid;
	int status;
	char buf[20000];
	memset(buf, 0, 20000);
	int piped[2];
	pipe(piped);

	pid = fork();

	if (pid < 0)
		std::cout << "fork() failed\n";
	else if (pid == 0)
	{
		int error = dup(2);
		(void)error;
		close (2);
		dup2(piped[1], 1);
		close (piped[0]);
		close (piped[1]);
		execl(path.c_str(), path.c_str(), "-q", filename.c_str(), params.c_str(), (char *)NULL);
		exit (1);
	}
	else
	{
		waitpid(pid, &status, 0);
		close(piped[1]);
		read(piped[0], buf, 20000);
	}
	result = buf;
	return result;
}


Response	&Response::operator=(Response const &equal_op) 
{
	if (this != &equal_op) {
		this->_response = equal_op._response;
		this->_body = equal_op._body;
		this->_errCodeStr = equal_op._errCodeStr;
		this->_errCode = equal_op._errCode;
		this->_version = equal_op._version;
		this->_contentType = equal_op._contentType;
		this->_responseLen = equal_op._responseLen;
		this->_parsedReq = equal_op._parsedReq;
		this->_server = equal_op._server;
		this->_csMethod = equal_op._csMethod;
		this->_csRoot = equal_op._csRoot;
		this->_csRoute = equal_op._csRoute;
	}
	return (*this);
}


std::ostream&	operator<<(std::ostream	&out, Response &x) 
{
	out << std::endl << "🌠 START:: show response 🌠" << std::endl;
	out << x.getResponse() << std::endl;
	out << "🌠 END:: show response 🌠" << std::endl << std::endl;
	return (out);
}
