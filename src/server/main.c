#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>

#define MAX_MSG_SIZE 1000


// Get IPv4 or IPv6
struct IpPort {
	char ipstr[INET6_ADDRSTRLEN];
	unsigned short port;
};

struct IpPort get_ipport(struct sockaddr *sa) {
	struct IpPort ipport;

	if (sa->sa_family == AF_INET) {
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)sa;
		inet_ntop(AF_INET, &(ipv4->sin_addr), ipport.ipstr, sizeof(ipport.ipstr));
		ipport.port = ntohs(ipv4->sin_port);
	} else {
		struct sockaddr_in *ipv6 = (struct sockaddr_in *)sa;
		inet_ntop(AF_INET, &(ipv6->sin_addr), ipport.ipstr, sizeof(ipport.ipstr));
		ipport.port = ntohs(ipv6->sin_port);
	}

	return ipport;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Invalid arguments: port\n");
		exit(1);
	}

	char *port = argv[1];
	struct addrinfo hints;
	struct addrinfo *service_info; 

	memset(&hints, 0, sizeof hints); 	// Make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     	// Don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; 	// TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     	// Fill in my IP for me

	if ((getaddrinfo(NULL, port, &hints, &service_info)) != 0) {
		perror("getaddrinfo: ");
		exit(1);
	}

	int listenfd = socket(service_info->ai_family, service_info->ai_socktype, service_info->ai_protocol);
	if (listenfd == -1) {
		perror("socket: ");
		exit(1);
	}

	if (bind(listenfd, service_info->ai_addr, service_info->ai_addrlen) == -1) {
		perror("bind: ");
		exit(1);
	}


	if (listen(listenfd, 10) == -1) {
		perror("listen: ");
		exit(1);
	}

	struct IpPort server_ipport = get_ipport(service_info->ai_addr);
	printf("%s listening on port %u\n", server_ipport.ipstr, server_ipport.port);

	freeaddrinfo(service_info);

	struct sockaddr_storage clientinfo;
	socklen_t clientinfo_size;
	printf("Waiting for connections...\n");

	int connfd = accept(listenfd, (struct sockaddr *)&clientinfo, &clientinfo_size);
	if (connfd == -1) {
		perror("accept: \n");
		exit(1);
	}

	struct IpPort client_ipport = get_ipport((struct sockaddr *)&clientinfo);
	printf("Connection from %s:%u\n", client_ipport.ipstr, client_ipport.port);

	char msg[MAX_MSG_SIZE];

	while (1) {
		int recv_status = recv(connfd, &msg, sizeof(msg), 0);
		if (recv_status == -1) {
			perror("recv: %s\n");
			exit(1);
		} else if (recv_status == 0) {
			printf("Client closed connection\n");
			break;
		} else {
			printf("Client: %s\n", msg);
		}

		memset(&msg, 0, sizeof(msg));
	}

	close(connfd);
	close(listenfd);

	return 0;
}

