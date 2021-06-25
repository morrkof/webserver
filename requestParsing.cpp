#include "requestParsing.hpp"

RequestParsing	&RequestParsing::operator=(RequestParsing const &equal_op) {
	if (this != &equal_op) {
		this->_method = equal_op._method;
		this->_location = equal_op._location;
		this->_version = equal_op._version;
		this->_body = equal_op._body;
		this->_mapHeaders = equal_op._mapHeaders;
		this->_dataFull = equal_op._dataFull;
		this->_dataNext = equal_op._dataNext;
		this->_posNext = equal_op._posNext;
	}
	return (*this);
}

std::string	RequestParsing::getMethod() const {return _method;}

std::string	RequestParsing::getLocation() const {return _location;}

std::string	RequestParsing::getVersion() const {return _version;}

std::map<std::string,std::string>	&RequestParsing::getMapHeaders() {return _mapHeaders;}

std::string	RequestParsing::getBody() const {return _body;}

void	RequestParsing::_errorMethodDoesntExist() {std::cout << "Error: method doesn't exist" << std::endl;}

int		RequestParsing::chooseMethod() {
	std::string	methodsAll[3] = {"GET", "POST", "DELETE"};
	_method = _dataFull.substr(0, _dataFull.find(" "));
	size_t		i = 0;
	while (i < 3 && _method != methodsAll[i]) i++;
	if (i == 3) {
		_errorMethodDoesntExist();
		return 1;
	}
	_posNext = _dataFull.find(" ") + 1; // _posNext now equals the first symbol of the location

	// std::cout << std::endl << "chooseMethod" << std::endl;
	// std::cout << "posNext 1:	" << _posNext << std::endl;
	// std::cout << "method:		" << _method << std::endl;
	return 0;
}

int		RequestParsing::chooseLocation() {
	_dataNext = _dataFull.substr(_posNext, std::string::npos);
	_posNext = _dataNext.find(" ") + 1; // _posNext now equals the first symbol of the version
	_location = _dataNext.substr(0, _dataNext.find(" "));

	// std::cout << std::endl << "chooseLocation" << std::endl;
	// std::cout << "posNext 2:	" << _posNext << std::endl;
	// std::cout << "location:	" << _location << std::endl << std::endl;
	// std::cout << "dataNext:	" << _dataNext << std::endl;
	return 0;
}

int		RequestParsing::chooseVersion() {
	_dataNext = _dataNext.substr(_posNext, std::string::npos);
	_version = _dataNext.substr(0, _dataNext.find("\n"));
	_posNext = _dataNext.find("\n") + 1; // _posNext now is the symbol of the first header

	// std::cout << std::endl << "chooseVersion" << std::endl;
	// std::cout << "posNext 3:	" << _posNext << std::endl;
	// std::cout << "location:	" << _location << std::endl << std::endl;
	// std::cout << "dataNext:	" << _dataNext << std::endl;
	// std::cout << "version:	" << _version << std::endl << std::endl;
	return 0;
}

int		RequestParsing::fillMapHeaders() {
	std::string	headerKey;
	std::string	headerValue;
	size_t		doubleNewline = 0;
	// std::cout << std::endl << "fillMapHeaders" << std::endl;
	while (_posNext != doubleNewline) {
		_dataNext = _dataNext.substr(_posNext, std::string::npos);
		headerKey = _dataNext.substr(0, _dataNext.find(":"));
		_posNext = _dataNext.find(": ") + 1;
		_dataNext = _dataNext.substr(_posNext, std::string::npos);
		headerValue = _dataNext.substr(0, _dataNext.find("\n"));
		_posNext = _dataNext.find("\n") + 1;
		doubleNewline = _dataNext.find("\r\n\r\n") + 1;
		_mapHeaders[headerKey] = headerValue;

		// std::cout << "headerKey:	" << headerKey << std::endl;
		// std::cout << "headerValue:	" << headerValue << std::endl;
		// std::cout << "posNext:		" << _posNext << std::endl;
		// std::cout << "doubleNewline:	" << doubleNewline << std::endl;
	}
	// for (std::map<std::string,std::string>::iterator it = _mapHeaders.begin(); it != _mapHeaders.end(); ++it)
	// 	std::cout << it->first << ": " << it->second << '\n';

	_posNext = _dataNext.find("\r\n\r\n") + 1;
	return 0;
}

int		RequestParsing::chooseBody() {
	if (_mapHeaders.find("Content-Length") == _mapHeaders.end()) {
		std::cout << "The body is not found" << std::endl;
		return 0;
	}
	_dataNext = _dataNext.substr(_posNext + 1, std::string::npos);
	size_t		doubleNewline = _dataNext.find("\r\n\r\n");
	_dataNext = _dataNext.substr(0, doubleNewline); // _dataNext now equals content
	_body = _dataNext;
	// std::cout << std::endl << "content" << std::endl;
	// std::cout << "posNext:	" << _posNext << std::endl;
	// std::cout << "body:	" << _body << std::endl;
	return 0;
}

int		RequestParsing::parseMeLikeYouDo() {
	if (chooseMethod() == 1)
		return 1;
	if (chooseLocation() == 1)
		return 1;
	if (chooseVersion() == 1)
		return 1;
	if (fillMapHeaders() == 1)
		return 1;
	if (chooseBody() == 1)
		return 1;
	return 0;
}

std::ostream&	operator<<(std::ostream	&out, RequestParsing &x) {
	out << std::endl << "🍄 START:: show parsed data 🍄" << std::endl;
	out << "Method:		" << x.getMethod() << std::endl;
	out << "Location:	" << x.getLocation() << std::endl;
	out << "Version:	" << x.getVersion() << std::endl;
	for (std::map<std::string,std::string>::iterator it = x.getMapHeaders().begin(); it != x.getMapHeaders().end(); ++it)
		std::cout << it->first << ":	" << it->second << '\n';
	out << "Body:		" << x.getBody() << std::endl;
	out << "🍄 END:: show parsed data 🍄" << std::endl << std::endl;

	return (out);
}