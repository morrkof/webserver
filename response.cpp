#include "response.hpp"

Response	&Response::operator=(Response const &equal_op) {
	if (this != &equal_op) {
		this->_response = equal_op._response;
		this->_body = equal_op._body;
		this->_errCode = equal_op._errCode;
		this->_version = equal_op._version;
		this->_responseLen = equal_op._responseLen;
		this->_parsedReq = equal_op._parsedReq;
	}
	return (*this);
}

void		Response::prr() {
	std::vector<std::string>::iterator it2;
	std::cout << "@@@@@@@@@@@@@@ 1";
	for (std::vector<ConfigurationServer>::iterator it = _serversVec.begin() ; it != _serversVec.end(); ++it) {
		for (it2 = it->getServerNameVec().begin() ; it2 != it->getServerNameVec().end(); ++it2)
			std::cout << ' ' << *it2;
		std::cout << "root" << it->getRoot() << std::endl;
	}
}

std::string	Response::generateContentType() {
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
	if (extension.rfind(".") == std::string::npos) {
		_contentType = "";
		return _contentType;
	}
	extension = extension.substr(extension.rfind("."), extension.npos);
	for(std::map<std::string,std::string>::iterator it = types.begin(); it != types.end(); ++it) {
		if (extension.compare(it->first) == 0)
			_contentType = it->second;
	}
	std::cout << "Content type: " << _contentType << std::endl;
	return _contentType;
}

int			Response::generateBody(const char* streamPath, std::string errCode) {
	std::string		buf;
	std::ifstream	ifs(streamPath);
	_errCode = errCode;
	if (ifs.is_open() == 0) {
			std::cout << "file doesn't exist" << std::endl;
			return 1;
		}
		while (!ifs.eof()) {
			std::getline(ifs, buf);
			_body.append(buf);
			_body.append("\n");
		}
	ifs.close();
	return 0;
}

int			Response::parseBody() {
	if (_parsedReq.getLocation() == "/") {						// root
		if (generateBody("sites/static/index.html", "200 ok") == 1)
			return 1;
	}
	else if (_parsedReq.getLocation() == "/root_unicorn.jpg") {		// error 404 asks for its unicorn
		if (generateBody("sites/pics/root_unicorn.jpg", "200 ok") == 1)
			return 1;
	}
	else if (_parsedReq.getLocation() == "/unicorn.jpg") {		// error 404 asks for its unicorn
		if (generateBody("sites/pics/unicorn.jpg", "200 ok") == 1)
			return 1;
	}
	else {														// error 404
		if (generateBody("sites/error404.html", "404 Not Found") == 1)
			return 1;
	}
	return 0;
}

std::string	Response::generateResponse() {
//	example: "HTTP/1.1 200 Ok \n\n <Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html> "
		_response.append(_version);		// HTTP/1.1
		_response.append(" ");
		_response.append(_errCode);		// 200 Ok
		_response.append(" ");
		_response.append("Content-Type: ");	// обязательное, без него пытается скачать
		_response.append(_contentType);
		_response.append("\n\n");		// один \n в конце предыдущего блока и ещё одна пустая строка чтоб отделить тело
		_response.append(_body);
		_responseLen = _response.length();
	return _response;
}

std::string	Response::parseResponse() {
	parseBody();
	generateContentType();
	if (_parsedReq.getLocation() == "/unicorn.jpg")
		generateResponse();
	else
	generateResponse();
	return _response;
}

std::ostream&	operator<<(std::ostream	&out, Response &x) {
	out << std::endl << "🌠 START:: show response 🌠" << std::endl;
	out << x.getResponse() << std::endl;
	out << "🌠 END:: show response 🌠" << std::endl << std::endl;
	return (out);
}
