#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>


int socketfd;


void sig_handler(int sig_num) {
	if (sig_num == 2) {
		printf("\nClosing socket\n");
		shutdown(socketfd, 2);
		exit(0);
	}
}


int main() {
	signal(SIGINT, sig_handler);
	socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketfd == -1) {
		perror("socket: ");
		exit(1);
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_port = htons(5050);

	if (bind(socketfd, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
		perror("bind: ");
		exit(1);
	}

	if (listen(socketfd, 10) == -1) {
		perror("listen: ");
		exit(1);
	}

	printf("Waiting for connections...\n");
	char send_buffer[100];

	while (1) {
		int connfd = accept(socketfd, (struct sockaddr*)NULL, NULL);

		if (connfd == -1) {
			perror("accept: \n");
			exit(1);
		}

		time_t now = time(NULL);
		snprintf(send_buffer, sizeof(send_buffer), "Server time: %s\n", ctime(&now));
		send(connfd, send_buffer, sizeof(send_buffer), 0);
		shutdown(connfd, 2);
	}

	return 0;
}
