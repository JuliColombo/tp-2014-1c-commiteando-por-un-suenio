/*
 * socket.c
 *
 *  Created on: 25/05/2014
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


/*t_stream nipc_serializar(t_nipc paquete) {

	t_stream stream;
	stream.length = sizeof(paquete.Type) + sizeof(paquete.Lenght)
			+ paquete.Lenght;
	stream.buffer = malloc(stream.length);
	int offset = 0;
	int temp_size = 0;

	memcpy(stream.buffer, &paquete.Type, temp_size = sizeof(paquete.Type));
	offset += temp_size;

	memcpy(stream.buffer + offset, &paquete.Lenght, temp_size= sizeof (paquete.Lenght) );
	offset += temp_size;

	if (paquete.Lenght > 0)
		memcpy(stream.buffer + offset, paquete.Payload, paquete.Lenght);

	return stream;

}

t_nipc nipc_deserializar(char* buffer) {

	t_nipc paquete;
	int offset = 0;

	paquete.Type = (*buffer);
	offset += sizeof paquete.Type;

	paquete.Lenght = *((int*) (buffer + offset));
	offset += sizeof paquete.Lenght;

	if (paquete.Lenght > 0) {

		paquete.Payload = malloc(paquete.Lenght);
		memcpy(paquete.Payload, buffer + offset, paquete.Lenght);
	}
	return paquete;
}*/


int nipc_sendall(int socket, t_stream stream) {
	int total = 0; // cuántos bytes hemos enviado
	int bytesleft = stream.length; // cuántos se han quedado pendientes
	int n;

	while (total < bytesleft) {
		n = send(socket, stream.buffer + total, bytesleft, MSG_NOSIGNAL);
		if (n == -1) {
			break;
		}
		total += n;
		bytesleft -= n;
	}

	if (n < 0)
		total = -1;

	return total;
}

int nipc_enviar(int sockfd, t_nipc paquete) {

	t_stream stream;
	int total_enviado;

	stream = nipc_serializar(paquete);
	total_enviado = nipc_sendall(sockfd, stream);

	free(stream.buffer);

	return total_enviado;
}

int nipc_recibir(int sockfd, t_nipc* paquete) {

	int tam_paquete = 0, max_buf = SOCK_BUFFER;
	char *buffer = (char *) malloc(SOCK_BUFFER);

	/* Intenta recibir el paquete entero. Si no hay espacio en el buffer, pide más memoria y vuelve a intentar
	 * hasta que haya espacio */
	while ((tam_paquete = recv(sockfd, buffer, max_buf, MSG_PEEK)) == max_buf) {
		max_buf += SOCK_BUFFER;
		buffer = realloc(buffer, max_buf);
	}

	recv(sockfd, buffer, tam_paquete, 0);

	if (tam_paquete > 0) {
		*paquete = nipc_deserializar(buffer);
	}

	free(buffer);

	return tam_paquete;

}

