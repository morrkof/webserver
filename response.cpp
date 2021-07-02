#include "response.hpp"

Response	&Response::operator=(Response const &equal_op) {
	if (this != &equal_op) {
		this->_code = equal_op._code;
		this->_parsedReq = equal_op._parsedReq;
		this->_response = equal_op._response;
		this->_responseLen = equal_op._responseLen;
	}
	return (*this);
}

std::string	Response::generateResponse() {
//	example: "HTTP/1.1 200 Ok \n\n <Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html> "
	_response.append(_version); // HTTP/1.1
	_response.append(" ");
	_response.append(_code); // 200 Ok
	_response.append(" \n\n <Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html> ");
	_responseLen = _response.length();
	return _response;
}

std::ostream&	operator<<(std::ostream	&out, Response &x) {
	out << std::endl << "🌠 START:: show response 🌠" << std::endl;
	out << x.getResponse() << std::endl;
	out << "🌠 END:: show response 🌠" << std::endl << std::endl;
	return (out);
}