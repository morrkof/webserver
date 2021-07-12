#include "response.hpp"

void		Response::chooseMethod() {
	_csMethod = _parsedReq.getMethod();
	if (_csMethod == "GET") methodGet();
	else if (_csMethod == "POST") methodPost();
	else if (_csMethod == "DELETE") methodDelete();
	else {
		_errCode = 400;
		_errCodeStr = "400 Bad Request";
		_contentType = "text/html";
		generateResponse();
		// std::cout << "Unknown method: " << _csMethod << " is not possible" << std::endl;
	}
}

void Response::methodGet() {
	if (_server->getMethods().count("GET"))
	{
		methodGetFormBody();
		generateContentType();
		_errCodeStr = "200 Ok"; // лучше ставить внутри
	}
	else {
		_errCode = 405;
		_errCodeStr = "405 Method Not Allowed";
	}
	generateResponse();
}

void Response::methodPost() {
	if (_server->getMethods().count("POST"))
	{
		/* Размер тела превышает допустимый? 413 ошибка
		** Content-type = multipart/form-data bla-bla-bla ? загружаем файл на сервер
		** Норм? - 201
		** Уже есть? 303 + Location header 
		** остальные ошибки - 500 Internal Server Error
		** Content-type = application/x-www-form-urlencoded ? запускаем CGI
		** CGI с ошибкой? - 502 Bad Gateway
		*/
		if (!_server->getReturnAddress().address.empty()) { // если строка заполнена
			_parsedReq.getMapHeaders().insert(_parsedReq.getMapHeaders().begin(), std::pair<std::string, std::string>("Location :", _server->getReturnAddress().address));
			_errCode = 301;
			_errCodeStr = "301 Moved Permanently";
			_body = CatGeneratePage(_errCode);
		}
	}
	else {
		_errCode = 405;
		_errCodeStr = "405 Method Not Allowed";
	}
	generateResponse();
}

void Response::methodDelete() {
	if (_server->getMethods().count("DELETE"))
	{
		_csRoot = _server->getRoot();
		std::string		fileToRemove = _csRoot + _parsedReq.getLocation();
		// std::cout << "---" << fileToRemove.c_str() << "---" << std::endl;
		if (remove(fileToRemove.c_str()) != 0) {
			// std::cout << "DELETE: Cannot delete file" << std::endl;
			_errCode = 204;
			_errCodeStr = "204 No Content";
		}
		else {
			// std::cout << "DELETE: successful" << std::endl;
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
			_body = CatGeneratePage(_errCode);
		_response.append(_body);
		_responseLen = _response.length();
	return _response;
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

	// std::cout <<  "🐝" << streamPath << std::endl;
	// + если нет прав на открытие файла - 403 error
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

std::string	Response::CatGeneratePage(int code) {
	std::string result;
	result.append("<html>");
	result.append("<body style=\"background-color:#000000\">");
	result.append("<div style=\"display: flex; align-items: center; justify-content: center; height: 100%; flex-direction: column;\">");
	result.append("<div> <img src=\"https://http.cat/");
	result.append(static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << code ) ).str());
	result.append(".jpg\"></div></div></body></html>");
	return result; 
}

int		Response::methodGetFormBody() {
	_errCode = 200;
	_errCodeStr = "200 Ok";
	if (_server->getLocationVec()[0].autoindex)
	{
		AutoIndexPage page(_server->getRoot()); // + что-то, надо подумать
		_body = page.getPage();
		return 1;
	}	
	// + if redirect - 301 err + Location header
	// if оканчивается на .php - CGI 
	if (_parsedReq.getLocation() == "/") {						// root + try_files в цикле
		if (generateBody("sites/static/index.html", 200) == 1)
		return 1;
	}
	else {														// error 404
		if (generateBody((_server->getRoot() + _parsedReq.getLocation()).c_str(), 200) == 1) {
			_errCode = 404;
			_errCodeStr = "404 Not Found";
			return 1;
		}
	}
	return 0; 
}

/*
**	params = "a=b";
**	path = "/usr/bin/php-cgi";
**	filename = "/home/anastasia/Desktop/webserv/sites/dynamic/index.php";
*/
std::string Response::cgi_process(std::string path, std::string filename, std::string params, char **env)
{
	std::string result;
	pid_t pid;
	int status;
	char *args[4];
	char buf[20000];
	memset(buf, 0, 20000);
	(void)params;
	// TODO распиливать params по пробелам, убрать захардкоженное
	args[0] = new char[std::string("php-cgi").size()];
	args[0] = (char *)"php-cgi";
	args[1] = (char *)filename.c_str();
	args[2] = (char *)"page=shop";
	args[3] = NULL;

	int piped[2];
	pipe(piped);

	pid = fork();

	if (pid < 0)
	{
		std::cout << "fork() failed\n";
		exit(-1);
	}
	else if (pid == 0)
	{
		dup2(piped[1], 1);
		close (piped[0]);
		close (piped[1]);
		execve(path.c_str(), args, env);
		// execl(path.c_str(), filename.c_str(), params.c_str(), (char *)NULL); // попробовать этот вариант без env
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

Response	&Response::operator=(Response const &equal_op) {
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
		this->_env = equal_op._env;
		this->_csMethod = equal_op._csMethod;
		this->_csRoot = equal_op._csRoot;
		this->_csRoute = equal_op._csRoute;
	}
	return (*this);
}

std::ostream&	operator<<(std::ostream	&out, Response &x) {
	out << std::endl << "🌠 START:: show response 🌠" << std::endl;
	out << x.getResponse() << std::endl;
	out << "🌠 END:: show response 🌠" << std::endl << std::endl;
	return (out);
}
