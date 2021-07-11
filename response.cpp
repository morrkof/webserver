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

std::string	Response::cgiCatGeneratePage(int code)
{
	std::string result;
	result.append("<html>");
	result.append("<body style=\"background-color:#000000\">");
	result.append("<div style=\"display: flex; align-items: center; justify-content: center; height: 100%; flex-direction: column;\">");
	result.append("<div> <img src=\"https://http.cat/");
	result.append(static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << code ) ).str());
	result.append(".jpg\"></div></div></body></html>");
	return result; 
}

void		Response::printConfigurationServer() {
	std::vector<t_listen>::iterator		it1;
	std::vector<std::string>::iterator	it2;
	std::vector<location>::iterator		it3;
	std::set<std::string>::iterator		it4;
	std::vector<std::string>::iterator	it5;
	std::set<std::string>::iterator		it6;
	std::vector<std::string>::iterator	it7;
	std::set<std::string>::iterator		it99;
	std::set<std::string>::iterator		itEnd;

	std::cout << "🍦 START:: ConfigurationServer Data 🍦" << std::endl;
		for (std::vector<ConfigurationServer>::iterator it = _serversVec->begin() ; it != _serversVec->end(); ++it) {
			// getListenVec
			it1 = it->getListenVec().begin();
			int sizeListenVec = it->getListenVec().size();
			std::cout << "Main methods size - " << sizeListenVec << std::endl;
			while (sizeListenVec > 0) {
				std::cout << "port - " << it1->port << std::endl;
				it1++;
				sizeListenVec--;
			}
			// for (it1 = it->getListenVec().begin() ; it1 != it->getListenVec().end(); ++it1) {
			// 	std::cout << "port - " << it1->port << std::endl;
			// }
			// getServerNameVec
			for (it2 = it->getServerNameVec()->begin() ; it2 != it->getServerNameVec()->end(); ++it2)
				std::cout << "Server name - " << *it2 << std::endl;
			// getRoot
			_csRoot = it->getRoot();
			std::cout << "root - " << _csRoot << std::endl;
			// getLocationVec
			for (it3 = it->getLocationVec().begin() ; it3 != it->getLocationVec().end(); ++it3) {
				std::cout << "location : " << std::endl;
				std::cout << "route - " << it3->route << std::endl;
				std::cout << "fastcgi include -" << it3->fastcgi_include << std::endl;
				std::cout << "fastcgi_pass - " << it3->fastcgi_pass << std::endl;
				for (it4 = it3->methods.begin() ; it4 != it3->methods.end(); ++it4)
					std::cout << "methods - " << *it4 << std::endl;
				for (it5 = it3->try_files->begin() ; it5 != it3->try_files->end(); ++it5)
					std::cout << "try_files - " << *it5 << std::endl;
				_errCode = it3->errorCode;
				_errCodeStr = it3->errorCode;
				std::cout << "errorCode int - " << _errCode << std::endl;
				std::cout << "errorCode str - " << _errCodeStr << std::endl;
				std::cout << "client_body_size - " << it3->client_body_size << std::endl;
				std::cout << "autoindex - ";
				if (it3->autoindex)
					std::cout << "on";
				std::cout << std::endl;
				std::cout << "finished - ";
				if (it3->finished)
					std::cout << "yes";
				std::cout << std::endl;
			}
			// getMethods
			// for (it6 = it->getMethods().begin() ; it6 != it->getMethods().end(); ++it6)
			// 		std::cout << "methods - " << *it6 << std::endl;
			it99 = it->getMethods().begin();
			int sizeMethods = it->getMethods().size();
			std::cout << "Main methods size - " << sizeMethods << std::endl;
			while (sizeMethods > 0) {
				std::cout << *it99 << std::endl;
				it99++;
				sizeMethods--;
			}
			// // getIndexVec
			for (it7 = it->getIndexVec()->begin() ; it7 != it->getIndexVec()->end(); ++it7)
					std::cout << "IndexVec - " << *it7 << std::endl;
			// getReturnAddress
			std::cout << "Return Address - " << it->getReturnAddress().address << std::endl;
			std::cout << "Error Code - " << it->getReturnAddress().errorCode << std::endl;
			std::cout << "🍦" << std::endl;
		}
		std::cout << "🍦 END:: ConfigurationServer Data 🍦" << std::endl;
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
	// std::cout << "Content type: " << _contentType << std::endl;
	return _contentType;
}

int			Response::generateBody(const char* streamPath, int errCode) {
	std::string		buf;
	std::ifstream	ifs(streamPath);
	_errCode = errCode;
	_errCodeStr = errCode;

	std::cout <<  "🐝" << streamPath << std::endl;

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

int		Response::methodGetFormBody() {
	if (_parsedReq.getLocation() == "/") {						// root
		if (generateBody("sites/static/index.html", 200) == 1)
		return 1;
	}
	else {														// error 404
		if (generateBody((_serversVec->front().getRoot() + _parsedReq.getLocation()).c_str(), 200) == 1) {
			_errCode = 404;
			_errCodeStr = "404 Not Found";
			return 1;
		}
	}
	// else if (_parsedReq.getLocation() == "/root_unicorn.jpg") {		// error 404 asks for its unicorn
	// 	if (generateBody("sites/pics/root_unicorn.jpg", "200 ok") == 1)
	// 		return 1;
	// }
	// else if (_parsedReq.getLocation() == "/unicorn.jpg") {		// error 404 asks for its unicorn
	// 	if (generateBody("sites/pics/unicorn.jpg", "200 ok") == 1)
	// 		return 1;
	// }
	// else {														// error 404
	// 	if (generateBody("sites/error404.html", "404 Not Found") == 1)
	// 		return 1;
	// }
	return 0; 
}

std::string	Response::generateResponse() {
//	example: "HTTP/1.1 200 Ok \n\n <Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html> "
		_response.append(_version);		// HTTP/1.1
		_response.append(" ");
		_response.append(_errCodeStr);		// 200 ok
		_response.append(" ");
		_response.append("Content-Type: ");	// обязательное, без него пытается скачать
		_response.append(_contentType);
		_response.append("\n\n");		// один \n в конце предыдущего блока и ещё одна пустая строка чтоб отделить тело
		if (_errCode != 200)
			_body = cgiCatGeneratePage(_errCode);
		_response.append(_body);
		_responseLen = _response.length();
	return _response;
}

void		Response::methodDelete() {
	std::string		fileToRemove = _csRoot + _parsedReq.getLocation();
	std::cout << "---" << fileToRemove.c_str() << "---" << std::endl;
	if (remove(fileToRemove.c_str()) != 0) {
		std::cout << "DELETE: Cannot delete file" << std::endl;
		_errCode = 204;
		_errCodeStr = "204 No Content";
	}
	else {
		std::cout << "DELETE: successful" << std::endl;
		_errCode = 202;
		_errCodeStr = "202 Accepted";
	}
}
void			Response::methodPost() {

}

void		Response::chooseMethod() {
	_csMethod = _parsedReq.getMethod();
	if (_csMethod == "GET") {
		methodGetFormBody();
		generateContentType();
		generateResponse();
	}
	if (_csMethod == "POST") {
		methodGetFormBody();
		methodPost();
		generateResponse();
	}
	else if (_csMethod == "DELETE") {
		methodDelete();
		generateResponse();
	}
	else {
		std::cout << "Unknown method: " << _csMethod << " is not possible" << std::endl;
	}
}

std::ostream&	operator<<(std::ostream	&out, Response &x) {
	out << std::endl << "🌠 START:: show response 🌠" << std::endl;
	out << x.getResponse() << std::endl;
	out << "🌠 END:: show response 🌠" << std::endl << std::endl;
	return (out);
}
