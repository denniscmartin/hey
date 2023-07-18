#include <asm-generic/socket.h>
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


int listenfd;

void sigint_handler(int sig_num) {
	printf("\nClosing socket\n");
	close(listenfd);
	exit(0);
}

void sigchld_handler(int s) {
	int saved_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
}

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

int main() {
	signal(SIGINT, sigint_handler);

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

	listenfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (listenfd == -1) {
		perror("socket: ");
		exit(1);
	}

	// Avoid "Address already in use"
	int yes = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt: \n");
	}

	if (bind(listenfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		perror("bind: ");
		exit(1);
	}

	freeaddrinfo(servinfo);

	if (listen(listenfd, 10) == -1) {
		perror("listen: ");
		exit(1);
	}

	// Kill zombie child processes
	struct sigaction sig;
	sig.sa_handler = sigchld_handler;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sig, NULL) == -1) {
		perror("sigaction: \n");
		exit(1);
	}

	struct IpPort server_ipport = get_ipport(servinfo->ai_addr);
	printf("Server IP: %s\n", server_ipport.ipstr);
	printf("Listening on port: %u\n", server_ipport.port);

	char send_buffer[100];
	struct sockaddr_storage clientinfo;
	socklen_t clientinfo_size;
	printf("Waiting for connections...\n");

	while (1) {
		int connfd = accept(listenfd, (struct sockaddr *)&clientinfo, &clientinfo_size);
		if (connfd == -1) {
			perror("accept: \n");
			exit(1);
		}

		struct IpPort client_ipport = get_ipport((struct sockaddr *)&clientinfo);
		printf("Client IP: %s\n", client_ipport.ipstr);
		printf("Client port: %u\n", client_ipport.port);

		int pid = fork();
		if (pid == 0) {
			// This is child
			close(listenfd);
			time_t now = time(NULL);
			snprintf(send_buffer, sizeof(send_buffer), "Server time: %s\n", ctime(&now));

			if (send(connfd, send_buffer, sizeof(send_buffer), 0) == -1) {
				perror("send: \n");
			}

			close(connfd);
			exit(0);
		}

		close(connfd);
	}

	return 0;
}

