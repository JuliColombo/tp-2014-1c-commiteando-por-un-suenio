/*
 * socket.h
 *
 *  Created on: 25/05/2014
 *      Author: utnso
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#define SOCK_BUFFER 512

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <linux/types.h>
#include "paquete.h"

/*  MACROS DE TYPE  */

typedef enum {

	NUEVO 		=	'N',
	REANUDADO   =	'R',
	DEMORADO  	=	'D',
	CANCELADO   =	'C',
	SUSPENDIDO  =	'S',
	FINALIZADO  =	'F',
	IMPRESION	=	'I',

} paquete_type;


int nipc_abrirConexion(unsigned short puerto);
int nipc_aceptarConexion(int sockfd);
int nipc_conectar(char *dirIP, unsigned short puerto);
t_stream nipc_serializar(t_nipc paquete);
t_nipc nipc_deserializar(char* buffer);
int nipc_sendall(int socket, t_stream stream);
int nipc_enviar(int sockfd, t_nipc paquete);
int nipc_recibir(int sockfd, t_nipc* paquete);


#endif /* SOCKET_H_ */
