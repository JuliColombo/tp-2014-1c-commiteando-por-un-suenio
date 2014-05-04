/*
 * socket.c
 *
 *  Created on: 04/05/2014
 *      Author: utnso
 */


#include "socket.h"

int nipc_abrirConexion(unsigned short puerto) {

	int sockfd;
	struct sockaddr_in dirlocal;
	int resp = 1;

	dirlocal.sin_family = AF_INET;
	dirlocal.sin_port = htons(puerto);
	dirlocal.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return sockfd;
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &resp, sizeof(int)) < 0) {
		perror("setsockopt");
		return -1;
	}

	if (bind(sockfd, (struct sockaddr*) &dirlocal, sizeof(dirlocal)) < 0) {
		perror("bind");
		return -1;
	}

	if (listen(sockfd, 5) < 0) {
		perror("listen");
		return -1;
	}

	return sockfd;
}

int nipc_aceptarConexion(int sockfd) {

	int fd;
	unsigned int tam;
	struct sockaddr cliente;

	tam = sizeof(struct sockaddr);

	if ((fd = accept(sockfd, &cliente, &tam)) < 0) {
		perror("accept");
		return fd;
	}

	return fd;

}
