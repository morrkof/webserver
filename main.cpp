#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>
#include "requestParsing.hpp"
#include "config/ConfigurationFile.hpp"
#include "response.hpp"
#include "Websocket.hpp"
#include <list>
#include <vector>
#include <set>
#include <algorithm>


ConfigurationFile*	getConfig(std::string	fileName)
{
	ConfigurationFile	*configParser = new ConfigurationFile();

	configParser->parseFile(fileName);
	std::vector<ConfigurationServer> *servers = configParser->getServers();
	std::cout << "____________________________________________________________" << std::endl;
	std::cout << "Got servers: " << servers->size() << std::endl;
	for (unsigned long i = 0; i < servers->size(); i++)
	{
		std::cout << servers->at(i);
		std::cout << "____________________________________________________________" << std::endl;
	}

	return configParser;
}

int	socket_init(int port)
{
	int listen_socket;
	int on = 1;

	if (!(listen_socket = socket(PF_INET, SOCK_STREAM, 0)))
	{
		std::cout << "socket() failed\n";
		exit(-1);
	}
	int rc = setsockopt(listen_socket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
	{
		std::cout << "setsockopt() failed\n";
		close(listen_socket);
		exit(-1);
	}
	fcntl(listen_socket, F_SETFL, O_NONBLOCK);
	struct sockaddr_in addr;
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

int main(int argc, char **argv, char **env)
{
	int status;
	ConfigurationFile *config;

	if (argc == 1)
		config = getConfig("basic_dynamic.conf");
	else
	{
		std::string filename(argv[1]);
		config = getConfig(filename);
	}

	std::cout << "Got config in main. Servers: " << config->getServers()->size() << std::endl;

	/* инициализируем сокеты и создаём массив слушающих сокетов на всех доступных портах  */
	std::list<Websocket *> sockets;
	Websocket *s;
	for (std::vector<ConfigurationServer>::iterator it = config->getServers()->begin(); it != config->getServers()->end(); ++it)
	{
		ConfigurationServer *temp = &(*it);
		std::vector<t_listen> ports = temp->getListenVec();
		for (std::vector<t_listen>::iterator it2 = ports.begin(); it2 != ports.end(); it2++)
		{
			s = new Websocket(socket_init((*it2).port), LISTEN, temp, env);
			sockets.push_back(s);
		}
	}
	
	std::cout << "🦄 Waiting for connect\n";

	fd_set fd_read, fd_write;
	int ready_events;
	while (1)
	{
		/* удаляем закрытые на прошлом проходе сокеты и очищаем наборы тех, где ловим события */
		sockets.remove(NULL);
		FD_ZERO(&fd_read);
		FD_ZERO(&fd_write);
		for (std::list<Websocket *>::iterator it = sockets.begin(); it != sockets.end(); ++it)
		{
			if ((*it)->getType() == WRITE)
				FD_SET((*it)->getSocket(), &fd_write);
			else
				FD_SET((*it)->getSocket(), &fd_read);
		}
		sockets.sort(compare_ws);
		/* непосредственно, селект. возвращает количество готовых событий */
		ready_events = select((*(sockets.rbegin()))->getSocket() + 1, &fd_read, &fd_write, NULL, NULL);
		if (ready_events < 0)
		{
			std::cout << "select() failed." << std::endl;
			for (std::list<Websocket *>::iterator it = sockets.begin(); it != sockets.end(); ++it)
				delete (*it);
			sockets.clear();
			exit(-1);
		}
		else if (ready_events == 0)
		{
			std::cout << "select() timeout\n";
			continue;
		}
		else
		{
			/* заводим счётчик отработанных событий и перебираем все наши сокеты */
			int done = 0;
			for (std::list<Websocket *>::iterator it = sockets.begin(); it != sockets.end() && done < ready_events; ++it)
			{
				/* если сработал сокет на чтение */
				if (FD_ISSET((*it)->getSocket(), &fd_read))
				{
					/* если это слушающий сокет, то "считывать" будем запрос на соединение */
					if ((*it)->getType() == LISTEN)
					{
						/* готовим структуру, принимаем соединение, появляется новый сокет для данных, добавляем его в общий массив с пометкой READ */
						sockaddr_storage client_addr;
						unsigned int address_size = sizeof(client_addr);
						int conn = accept((*it)->getSocket(), (sockaddr *) &client_addr, &address_size);
						fcntl(conn, F_SETFL, O_NONBLOCK);
						Websocket *s = new Websocket(conn, READ, (*it)->getServer(), env);
						sockets.push_back(s);
						FD_CLR((*it)->getSocket(),&fd_read);
					}
					/* если это сокет данных, то читаем запрос и формируем ответ, внутри класса пометка READ превратится во WRITE */
					else
					{
						int len = 512;
						char buf[len+1];
						memset(buf, 0, len+1);
						status = recv((*it)->getSocket(), buf, len, 0);
						if (status == -1)
						{
							std::cout << "recieve() failed." << std::endl;
							for (std::list<Websocket *>::iterator it = sockets.begin(); it != sockets.end(); ++it)
								delete (*it);
							sockets.clear(); 
							exit(-1);
						}
						else if (status < len)
						{
							(*it)->setRecvBuf(buf);
							(*it)->setRequest((*it)->getRecvBuf());
							FD_CLR((*it)->getSocket(),&fd_read);
							// std::cout << (*it)->getRecvBuf() << std::endl; /* тут печать реквеста ДО парсинга */
							// std::cout << (*it)->getRequest(); /* тут печать распарсенного пришедшего реквеста */
							// std::cout << (*it)->getResponse(); /* тут печать сформированного ответа */
						}
						else
							(*it)->setRecvBuf(buf);
					}
					++done;
				}
				/* если сработал сокет на запись */
				else if (FD_ISSET((*it)->getSocket(), &fd_write))
				{
					/* отправляем сформированный ответ и удаляем обработанный сокет */
					status = send((*it)->getSocket(), ((*it)->getResponseChars()).c_str() + (*it)->getSendOffset(), 
					(*it)->getResponseLen() - (*it)->getSendOffset(), 0);
					if (status == -1)
					{
						std::cout << "send() failed." << std::endl;
						for (std::list<Websocket *>::iterator it = sockets.begin(); it != sockets.end(); ++it)
							delete (*it);
						sockets.clear();
						exit(-1);
					}
					else if (status != (int)((*it)->getResponseLen() - (*it)->getSendOffset()))
					{
						(*it)->setSendOffset(status);
					}
					else
					{
						FD_CLR((*it)->getSocket(),&fd_write);
						++done;
						delete (*it);
						*it = NULL;
					}
				}
			}
		}
	}
	return 0;
}
