#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


std::string generate_page(int code)
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

/*
**	params = "a=b";
**	path = "/usr/bin/php-cgi";
**	filename = "/home/anastasia/Desktop/webserv/sites/dynamic/index.php";
*/
std::string cgi_process(std::string path, std::string filename, std::string params, char **env)
{
	std::string result;
	pid_t pid;
	int status;
	char *args[4];
	char buf[20000];
	memset(buf, 0, 20000);
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

// int main(int argc, char **argv, char **env)
// {
// 	std::string body;
// 	std::string result;

// 	body = "a=b";
// 	std::string path = "/usr/bin/php-cgi";
// 	std::string filename("/home/anastasia/Desktop/webserv/sites/dynamic/index.php");
// 	result = cgi_process(path, filename, body, env);
// 	std::cout << result;
// }