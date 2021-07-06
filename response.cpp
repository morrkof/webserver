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

int			Response::generateBody() {
	if (_parsedReq.getLocation() == "/") {						// root
		_errCode = "200 ok";
		std::ifstream	ifs("sites/static/index.html");
		std::string		buf;
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
		// _body = "<Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html>";
	}
	else if (_parsedReq.getLocation() == "/root_unicorn.jpg") {		// error 404 asks for its unicorn
		_errCode = "200 ok";
		std::string		line;
		std::string		source("sites/pics/root_unicorn.jpg");
		std::ifstream	ifs(source.c_str(), std::ios::binary);
		if (!ifs) {
			_errCode = "404 Not Found";
			std::cout << "Error: cannot open this file" << std::endl;
			return 1;
		}
		while (getline(ifs, line)) {
			_body += line;
			if (ifs.eof())
				break;
			_body += "\n";
		}
		ifs.close();
	}
	else if (_parsedReq.getLocation() == "/unicorn.jpg") {		// error 404 asks for its unicorn
		_errCode = "200 ok";
		std::string		line;
		std::string		source("sites/pics/unicorn.jpg");
		std::ifstream	ifs(source.c_str(), std::ios::binary);
		if (!ifs) {
			_errCode = "404 Not Found";
			std::cout << "Error: cannot open this file" << std::endl;
			return 1;
		}
		while (getline(ifs, line)) {
			_body += line;
			if (ifs.eof())
				break;
			_body += "\n";
		}
		ifs.close();
	}
	else {														// error 404
		_errCode = "404 Not Found";
		std::ifstream	ifs("sites/error404.html");
		std::string		buf;
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
	}
	// std::cout << _body << std::endl;
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
	generateBody();
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
