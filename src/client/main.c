#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1000


int main() {
	char *hostname = getenv("HOSTNAME");
	char *port = getenv("PORT");
	printf("SERVER HOSTNAME: %s\n", hostname);
	printf("SERVER PORT: %s\n\n", port);

	printf("Press ENTER to connect to the server.");
	while (getchar() != '\n');

	struct addrinfo hints;
	struct addrinfo *service_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((getaddrinfo(hostname, port, &hints, &service_info)) != 0) {
		perror("getaddrinfo");
		exit(1);
	}

	int clientfd = socket(service_info->ai_family, service_info->ai_socktype, service_info->ai_protocol);
	if (clientfd == -1) {
		perror("socket");
	}

	if (connect(clientfd, service_info->ai_addr, service_info->ai_addrlen) == -1) {
		perror("connect");
		exit(1);
	}

	freeaddrinfo(service_info);
	printf("\nConnection established\n\n");

	char msg[MAX_MSG_SIZE];
	bool connected = true;

	while (connected) {
		printf("Enter command: ");
		fgets(msg, MAX_MSG_SIZE, stdin);
		msg[strcspn(msg, "\n")] = 0;

		if (strcmp(msg, "exit") == 0) {
			break;
		}

		ssize_t bytes_sent = send(clientfd, msg, strlen(msg), 0);
		if (bytes_sent == -1) {
			perror("send");
			exit(1);
		}

		printf("%zd bytes sent\n", bytes_sent);
		memset(&msg, 0, sizeof(msg));
		printf("\n");
	}

	close(clientfd);

	return 0;
}
