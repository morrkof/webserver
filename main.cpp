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
#include <errno.h>

#define PORT 8000 // это ждём из конфига

ConfigurationFile*	getConfig(std::string	fileName)
{
	ConfigurationFile	*configParser = new ConfigurationFile();

	std::cout << "Start parse config" << std::endl;
	configParser->parseFile(fileName);
	std::cout << "Finish parse config" << std::endl;
	std::vector<ConfigurationServer> *servers = configParser->getServers();
	std::cout << "Got servers: " << servers->size() << std::endl;
	for (unsigned long i = 0; i < servers->size(); i++)
	{
		std::cout << servers->at(i);
	}

	return configParser;
}

/* создаём слушающие сокеты в этой функции */
int	socket_init(int port)
{
	int listen_socket;
	int on = 1;

	/* открываем сокет и присваиваем его в listen_socket */
	if (!(listen_socket = socket(PF_INET, SOCK_STREAM, 0)))
	{
		std::cout << "socket() failed\n";
		exit(-1);
	}
	/* делаем наш сокет реюзабельным, для повторного использования без кулдауна */
	int rc = setsockopt(listen_socket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
	{
		std::cout << "setsockopt() failed\n";
		close(listen_socket);
		exit(-1);
	}
	/* делаем наш сокет неблокирующим, чтоб процесс не вис тут, если клиент не отвечает */
	fcntl(listen_socket, F_SETFL, O_NONBLOCK);
	/* подготавливаем стандартную структуру с настройками порта и привязываем сокет к порту */
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
	/* выставляем наш сокет в слушающий режим, он готов принимать соединения */
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
	ConfigurationFile *config;

	/* Пошел сюда конфиг */
	if (argc == 1)
		config = getConfig("basic_static.conf");
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

	/* переменные для селекта */
	fd_set fd_read, fd_write;
	int ready_events;
	while (1)
	{
		/* удаляем закрытые на прошлом проходе сокеты и очищаем наборы тех, где ловим события */
		sockets.remove(NULL);
		FD_ZERO(&fd_read);
		FD_ZERO(&fd_write);
		/* проходим по всем сокетам и добавляем их в наборы для ловли событий */
		for (std::list<Websocket *>::iterator it = sockets.begin(); it != sockets.end(); ++it)
		{
			if ((*it)->getType() == WRITE)
				FD_SET((*it)->getSocket(), &fd_write);
			else
				FD_SET((*it)->getSocket(), &fd_read);
		}
		/* сортируем, так как селект принимает максимальный + 1 */
		sockets.sort(compare_ws);
		/* непосредственно, селект. возвращает количество готовых событий */
		ready_events = select((*(sockets.rbegin()))->getSocket() + 1, &fd_read, &fd_write, NULL, NULL);
		/* если вернул < 0 = ошибка, всё зачищаем и закрываем и выходим */
		if (ready_events < 0)
		{
			int err = errno;
			std::cout << "select() failed. Error " << err << " " << strerror(err) << std::endl;
			for (std::list<Websocket *>::iterator it = sockets.begin(); it != sockets.end(); ++it)
				delete (*it);
			sockets.clear();
			exit(-1);
		}
		/* вернул 0 - таймаут, запустить по второму кругу */
		else if (ready_events == 0)
		{
			std::cout << "select() timeout\n";
			continue;
		}
		/* вернул > 0 = идём смотреть, что за события нам поймались */
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
					}
					/* если это сокет данных, то читаем запрос и формируем ответ, внутри класса пометка READ превратится во WRITE */
					else
					{
						int len = 20000; // TODO брать этот размер из конфига
						char buf[len];
						memset(buf, 0, len);
						recv((*it)->getSocket(), buf, len, 0);
						(*it)->setRequest(buf);
						// std::cout << buf << std::endl; /* тут печать реквеста ДО парсинга */
						// std::cout << (*it)->getRequest(); /* тут печать распарсенного пришедшего реквеста */
						// std::cout << (*it)->getResponse(); /* тут печать сформированного ответа */
					}
					/* удаляем обработанный сокет из селектового набора на чтение */
					FD_CLR((*it)->getSocket(),&fd_read);
					++done;
				}
				/* если сработал сокет на запись */
				else if (FD_ISSET((*it)->getSocket(), &fd_write))
				{
					/* отправляем сформированный ответ и удаляем обработанный сокет */
					send((*it)->getSocket(), ((*it)->getResponseChars()).c_str(), (*it)->getResponseLen(), 0);
					FD_CLR((*it)->getSocket(),&fd_write);
					++done;
					delete (*it);
					*it = NULL;
				}
			}
		}
	}
	return 0;
}
