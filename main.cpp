#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "requestParsing.hpp"
#include "response.hpp"
#include <vector>
#include <algorithm>

#define PORT 3000

int	socket_init(int port)
{
	int listen_socket;
	int on = 1;
	sockaddr_in addr;

	if (!(listen_socket = socket(PF_INET, SOCK_STREAM, 0)))
	{
		std::cout << "socket() failed\n";
		exit(-1);
	}

	int rc = setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
	{
		std::cout << "setsockopt() failed\n";
		close(listen_socket);
		exit(-1);
	}
	addr.sin_family = PF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listen_socket, (sockaddr *) &addr, sizeof(addr)) == -1)
	{
		std::cout << "bind() failed\n";
		close(listen_socket);
		exit(-1);
	}
	if (listen(listen_socket, 128) == -1)
	{
		std::cout << "listen() failed\n";
		close(listen_socket);
		exit(-1);
	}
	return listen_socket;
}

int main()
{
	std::vector<int> sockets;
	sockets.push_back(socket_init(PORT)); // TODO: цикл
	sort(sockets.begin(), sockets.end());
	std::cout << "Waiting for connect\n";

	fd_set fd_read, fd_write;
	// timeval tv;
	// tv.tv_sec = 10;
	// tv.tv_usec = 0;
	int ready_events;

	// // 227-228 Столяров

	while (1)
	{
		FD_ZERO(&fd_read);
		FD_ZERO(&fd_write);
		for (std::vector<int>::iterator it = sockets.begin(); it != sockets.end(); ++it)
			FD_SET(*it, &fd_read);
		// TODO: функция поиска максимального среди двух массивов
		ready_events = select(sockets.back()+1, &fd_read, &fd_write, NULL, NULL);
		if (ready_events < 0)
		{
			std::cout << "select() failed\n";
			// close(listen_socket); // закрыть всё что в массивах
			exit(-1);
		}
		else if (ready_events == 0)
		{
			std::cout << "select() timeout\n";
			continue;
		}
		else
		{
			for (std::vector<int>::iterator it = sockets.begin(); it != sockets.end(); ++it)
			{
				if (FD_ISSET(*it, &fd_read))
				{
					sockaddr_storage client_addr;
					unsigned int address_size = sizeof(client_addr);
					int conn = accept(*it, (sockaddr *) &client_addr, &address_size);
					// TODO: new Connection class добавить в массив коннекшонов
					char buf[20000];
					int len = 20000;
					recv(conn, buf, len, 0); // тут принимаем запрос в buf
					RequestParsing		req1(buf); // added by bjebedia: creting a class to store data
					std::cout << req1; // added by bjebedia: print data
					Response			resp1("200 Ok", req1); // added by bjebedia: creating a response class
					std::cout << resp1;
					// buf не обнуляется
					std::cout << buf << std::endl;
					// pure classless send:
//					send(conn, "HTTP/1.1 200 Ok \n\n <Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html> ", strlen("HTTP/1.1 200 Ok \n\n <Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html> "), 0);
					send(conn, (resp1.getResponse()).c_str(), resp1.getResponseLen(), 0); // added by bjebedia: same but using reponse class
					close (conn);
				}
				if (FD_ISSET(*it, &fd_write))
				{
					// todo
				}
			}
		}
	}
	return 0;
}
