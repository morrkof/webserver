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
	if (_parsedReq.getLocation() == "/")						// if location exists
		_body = "<Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html>";
	else if (_parsedReq.getLocation() == "/unicorn.jpg") {		// error 404 asks for its unicorn
		std::ifstream	ifs("sites/unicorn.jpg");
		std::string		buf;
		_picLen = 10;
		if (ifs.is_open() == 0) {
			std::cout << "file doesn't exist" << std::endl;
			return 1;
		}
		while (!ifs.eof()) {
			std::getline(ifs, buf);
			_body.append(buf);
			_body.append("\n");
			_picLen = _picLen + _body.length();
		}
		ifs.close();
	}
	else {														// error 404
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
	if (_parsedReq.getLocation() == "/unicorn.jpg") {
		_body = getFileStr("unicorn.jpg"); // не разобралась где тут делается тело, сделала свою функцию (переделай по-своему)
		_response.append(_version); // HTTP/1.1
		_response.append(" ");
		_response.append(_code); // 200 Ok
		_response.append("Content-Type: image/jpeg\n"); // обязательное, без него пытается скачать
		// _response.append("Content-Length: 13887\n"); // не обязательное
		// _response.append("Content-Transfer-Encoding: binary\n"); // не обязательное
		_response.append("\n"); // один \n в конце предыдущего блока и ещё одна пустая строка чтоб отделить тело
		_response.append(_body);
		_responseLen = _response.length();
	}
	else
	generateBody();
	_response.append(_version); // HTTP/1.1
	_response.append(" ");
	_response.append(_code); // 200 Ok
	_response.append(" \n\n ");
	_response.append(_body);
	_responseLen = _response.length();
	return _response;
}

std::ostream&	operator<<(std::ostream	&out, Response &x) {
	out << std::endl << "🌠 START:: show response 🌠" << std::endl;
	out << x.getResponse() << std::endl;
	out << "🌠 END:: show response 🌠" << std::endl << std::endl;
	return (out);
}

std::string Response::getFileStr(std::string source) // моя функция, переделать по своему и удалить, из hpp тоже удалить
{
	std::string result;
	std::ifstream ifs(source.c_str(), std::ios::binary);
	if (!ifs) {
		std::cout << "Error: cannot open this file" << std::endl;
		exit (1);
	}

	std::string line;
	while (getline(ifs, line)) {
		result += line;
		if (ifs.eof())
			break;
		result += "\n";
	}
	ifs.close();
	return result;
}