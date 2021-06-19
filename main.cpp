#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// #include <poll.h>
// POLL
// // два события
// struct pollfd fds[2];
 
// // от sock1 мы будем ожидать входящих данных
// fds[0].fd = sock1;
// fds[0].events = POLLIN;
 
// // а от sock2 - исходящих
// fds[1].fd = sock2;
// fds[1].events = POLLOUT;
 
// // ждём до 10 секунд
// int ret = poll( &fds, 2, 10000 );
// // проверяем успешность вызова
// if ( ret == -1 )
//     // ошибка
// else if ( ret == 0 )
//     // таймаут, событий не произошло
// else
// {
//     // обнаружили событие, обнулим revents чтобы можно было переиспользовать структуру
//     if ( pfd[0].revents & POLLIN )
//         pfd[0].revents = 0;
//         // обработка входных данных от sock1

//     if ( pfd[1].revents & POLLOUT )
//         pfd[1].revents = 0;
//         // обработка исходящих данных от sock2
// }


int main(int argc, char *argv[])
{
	int temp_port = 80;
	fd_set fd_in, fd_out;
	struct timeval tv;
	FD_ZERO(&fd_in);
	FD_ZERO(&fd_out);
	// 227-228 Столяров

	int sock = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(temp_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		write(2, "Connection error", 17);
		exit(1);
	}
	listen(sock, 128);
	write(1, "Waiting for connect", 20);
	while (1)
	{
		struct sockaddr_storage client_addr;
		unsigned int address_size = sizeof(client_addr);
		// TODO:  добавить неблокирующий ввод, стримы и селект
		int conn = accept(sock, (struct sockaddr *) &client_addr, &address_size);
		char buf[50];
		int len = 50;
		int c = recv(conn, buf, len, 0);


		send(conn, "PRIVET", strlen("PRIVET"), 0);
		write(1, buf, 50);
		close (conn);
	}
	return 0;
}
