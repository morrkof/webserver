#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in name;
	name.sin_family = PF_INET;
	name.sin_port = (in_port_t)htons(80);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *) &name, sizeof(name)) == -1)
	{
		write(2, "Ошибка подключения", 36);
	}
	listen(sock, 10);
	write(1, "Ожидание подключения", 40);
	while (1)
	{
		pid_t pid;
		struct sockaddr_storage client_addr;
		unsigned int address_size = sizeof(client_addr);
		int conn = accept(sock, (struct sockaddr *) &client_addr, &address_size);
		char buf[50];
		int len = 50;
		int c = recv(conn, buf, len, 0);
		send(conn, "PRIVET", strlen("PRIVET"), 0);
		write(1, buf, 50);
		close (conn);
		// if ((pid = fork()) < 0) 
		// 	close(sock);
		// if (pid == 0)
		// {
		// 	
		// 	close(conn);
		// 	exit (0);
		// }
	}
	return 0;
}
