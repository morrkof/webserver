#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	//первоначально нужно будет распарсить конфиг, сейчас эти переменные захардкожены:
	int temp_port = 80;

	// создание сокета, PF_INET это семейство адресации TCP/IP, SOCK_STREAM это потоковый
	// тип взаимодействия, 0 это протокол (не нужно выставлять, оно само определит)
	int sock = socket(PF_INET, SOCK_STREAM, 0);

	// заполняем структуру с адресом, она нужна для привязки сокета (bind)
	// sin_family - семейство адресации, то же что и выше (PF_INET)
	// sin_port - short int с big_endian порядком (сетевой стандарт), в случае совпадения
	// порядков с комплюктером - не сделает ничего
	// sin_addr.s_addr - IP-адрес в формате unsigned int big_endian (система сама определит айпи)
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(temp_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	
	// Снабжаем сокет адресом. В аргументах - фд сокета, заполненная структура, размер структуры
	// Обязательна проверка на ошибки - т.к. часто сокет бывает занят/отсутствуют права
	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		write(2, "Connection error", 17);
		exit(1);
	}

	// Перевод сокета в слушающий режим, аргументы - фд сокета и кол-во запросов в очереди, после
	// которых система начнёт отклонять все запросы, 128 максимальное на современных ОС
	listen(sock, 128);
	write(1, "Waiting for connect", 20);
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
