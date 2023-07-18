#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>


int socketfd;


void sig_handler(int sig_num) {
	if (sig_num == 2) {
		printf("\nClosing socket\n");
		close(socketfd);
		exit(0);
	}
}


int main() {

	signal(SIGINT, sig_handler);

	struct addrinfo hints;
	struct addrinfo *servinfo; 

	memset(&hints, 0, sizeof hints); 	// Make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     	// Don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; 	// TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     	// Fill in my IP for me

	if ((getaddrinfo(NULL, "5050", &hints, &servinfo)) != 0) {
		perror("getaddrinfo: ");
		exit(1);
	}

	socketfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	if (socketfd == -1) {
		perror("socket: ");
		exit(1);
	}

	if (bind(socketfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		perror("bind: ");
		exit(1);
	}

	freeaddrinfo(servinfo);

	if (listen(socketfd, 10) == -1) {
		perror("listen: ");
		exit(1);
	}

	char send_buffer[100];
	struct sockaddr_storage clientinfo;
	socklen_t clientinfo_size;

	printf("Waiting for connections...\n");

	while (1) {
		int connfd = accept(socketfd, (struct sockaddr *)&clientinfo, &clientinfo_size);

		if (connfd == -1) {
			perror("accept: \n");
			exit(1);
		}

		time_t now = time(NULL);
		snprintf(send_buffer, sizeof(send_buffer), "Server time: %s\n", ctime(&now));
		send(connfd, send_buffer, sizeof(send_buffer), 0);
		close(connfd);
	}

	return 0;
}
