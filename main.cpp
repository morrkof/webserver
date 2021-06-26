#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "requestParsing.hpp"

#define PORT 8080

int main()
{
	int listen_socket;
	int on = 1;

	if (!(listen_socket = socket(PF_INET, SOCK_STREAM, 0)))
	{
		perror("socket");
		exit(1);
	}

	int rc = setsockopt(listen_socket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
	{
		perror("setsockopt() failed");
		close(listen_socket);
		exit(-1);
	}

	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listen_socket, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		write(2, "Connection error\n", 18);
		exit(1);
	}
	listen(listen_socket, 128);
	std::cout << "Waiting for connect\n";

	// fd_set fd_in, fd_out;
	// // struct timeval tv;
	// FD_ZERO(&fd_in);
	// FD_ZERO(&fd_out);
	// // 227-228 Столяров

	while (1)
	{
		struct sockaddr_storage client_addr;
		unsigned int address_size = sizeof(client_addr);
		// TODO:  добавить неблокирующий ввод и селект
		int conn = accept(listen_socket, (struct sockaddr *) &client_addr, &address_size);
		char buf[20000];
		int len = 20000;
		recv(conn, buf, len, 0); // тут принимаем запрос в buf
		RequestParsing	ex1(buf); // added by bjebedia: creting a class to store data
		std::cout << ex1; // added by bjebedia: print data
		std::cout << buf << std::endl;
		// тут в send передаём ответ
		send(conn, "HTTP/1.1 200 Ok \n\n <Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html> ", strlen("HTTP/1.1 200 Ok \n\n <Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html> "), 0);
		close (conn);
	}
	return 0;
}
