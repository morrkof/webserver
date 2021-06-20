#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#define PORT 80

int main()
{
	int listen_socket;
	int client_socket;
	long read_value;
	struct sockaddr_in address;

	if (!(listen_socket = socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("socket");
		exit(1);
	}

	// fd_set fd_in, fd_out;
	// // struct timeval tv;
	// FD_ZERO(&fd_in);
	// FD_ZERO(&fd_out);
	// // 227-228 Столяров

	// int sock = socket(AF_INET, SOCK_STREAM, 0);
	// struct sockaddr_in addr;
	// addr.sin_family = AF_INET;
	// addr.sin_port = htons(temp_port);
	// addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	// {
	// 	write(2, "Connection error", 17);
	// 	exit(1);
	// }
	// listen(sock, 128);
	// write(1, "Waiting for connect", 20);
	// while (1)
	// {
	// 	struct sockaddr_storage client_addr;
	// 	unsigned int address_size = sizeof(client_addr);
	// 	// TODO:  добавить неблокирующий ввод, стримы и селект
	// 	int conn = accept(sock, (struct sockaddr *) &client_addr, &address_size);
	// 	char buf[50];
	// 	int len = 50;
	// 	recv(conn, buf, len, 0);
	// 	send(conn, "HTTP/1.1 200 Ok \n<Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html> ", strlen("HTTP/1.1 200 Ok \n<Html> <Head> <title> Example </title>  </Head>  <Body> Hello </Body> </Html> "), 0);
	// 	write(1, buf, 50);
	// 	close (conn);
	// }
	return 0;
}
