#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main() {
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketfd == -1) {
		printf("Socket creation failed\n");
		exit(1);
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_port = htons(5050);

	if (bind(socketfd, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
		printf("Socket binding failed\n");
	}

	//if (connect(socketfd, (struct sockaddr*)&sin, sizeof(sin) == -1)) {
		//printf("Socket connection failed\n");
	//}

	if (listen(socketfd, 10) == -1) {
		printf("Listen failed\n");
	}

	char sendBuff[50] = "Hello world\n";

	while (1) {
		int connfd = accept(socketfd, (struct sockaddr*)NULL, NULL);
		send(connfd, sendBuff, sizeof(sendBuff), 0);
		shutdown(connfd, 2);
	}


	return 0;
}
