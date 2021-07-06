#include "response.hpp"

Response	&Response::operator=(Response const &equal_op) {
	if (this != &equal_op) {
		this->_response = equal_op._response;
		this->_body = equal_op._body;
		this->_code = equal_op._code;
		this->_version = equal_op._version;
		this->_responseLen = equal_op._responseLen;
		this->_parsedReq = equal_op._parsedReq;
	}
	return (*this);
}

int		Response::generateBody() {
	if (_parsedReq.getLocation() == "/") {						// root
		_code = "200 ok";
		_body = "<Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html>";
	}
	else if (_parsedReq.getLocation() == "/unicorn.jpg") {		// error 404 asks for its unicorn
		_code = "200 ok";
		std::string		line;
		std::string		source("sites/unicorn.jpg");
		std::ifstream	ifs(source.c_str(), std::ios::binary);
		if (!ifs) {
		_code = "404 Not Found";
		std::cout << "Error: cannot open this file" << std::endl;
		exit (1);
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
		_code = "404 Not Found";
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

std::string	Response::generateResponse(std::string x) {
//	example: "HTTP/1.1 200 Ok \n\n <Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html> "
		_response.append(_version);		// HTTP/1.1
		_response.append(" ");
		_response.append(_code);		// 200 Ok
		_response.append(x);			// обязательное, без него пытается скачать
		_response.append("\n\n");		// один \n в конце предыдущего блока и ещё одна пустая строка чтоб отделить тело
		_response.append(_body);
		_responseLen = _response.length();
	return _response;
}

std::string	Response::parseResponse() {
	generateBody();
	if (_parsedReq.getLocation() == "/unicorn.jpg")
		generateResponse("Content-Type: image/jpeg");
	else
	generateResponse("");
	return _response;
}

std::ostream&	operator<<(std::ostream	&out, Response &x) {
	out << std::endl << "🌠 START:: show response 🌠" << std::endl;
	out << x.getResponse() << std::endl;
	out << "🌠 END:: show response 🌠" << std::endl << std::endl;
	return (out);
}
