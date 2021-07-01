#include "response.hpp"

Response::Response(Response const &copy) {*this = copy; return;}

Response	&Response::operator=(Response const &equal_op) {
	if (this != &equal_op) {
		this->_parsedReq = equal_op._parsedReq;
		this->_response = equal_op._response;
		this->_responseLen = equal_op._responseLen;
	}
	return (*this);
}

std::string	Response::generateResponse() {
//	"HTTP/1.1 200 Ok \n\n <Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html> "
	_response = "";
	_response.append(_parsedReq.getVersion());
	_response.append(" ");
	_responseLen = _response.length();
	return _response;
}

std::ostream&	operator<<(std::ostream	&out, Response &x) {
	out << std::endl << "🌠 START:: show response 🌠" << std::endl;
	out << x.getResponse() << std::endl;
	out << "🌠 END:: show response 🌠" << std::endl << std::endl;
	return (out);
}