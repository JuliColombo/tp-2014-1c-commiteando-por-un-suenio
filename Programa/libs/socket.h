/*
 * socket.h
 *
 *  Created on: 28/05/2014
 *      Author: utnso
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "estructuras_socket.h"
#include "commons/string.h"


#define MAX_EVENTS_EPOLL 60
#define MAX_CONNECTION_SERVER 60 //VAMOS A ATENDER DE A 10 CONEXIONES COMO MAXIMO A LA VEZ

// Estructura para paquetizar datos a enviar/recibir
typedef struct {
	uint8_t tipo;
	uint16_t length;
} t_socketHeader;

typedef uint8_t t_tipoEstructura;
typedef unsigned int t_signal;

//FUNCIONES PARA EL CLIENTE
int socket_crearCliente(void);
int socket_conectarCliente(int sockfd,char *serverIp, int serverPort);
int socket_crearYConectarCliente(char *serverIp, int serverPort);

//FUNCIONES PARA EL SERVIDOR
int socket_crearServidor(char *ip, int port);
int socket_crearServidorPuertoRandom(char *ip, int * port);
int socket_aceptarCliente(int socketEscucha);

//FUNCIONES EPOLL
int epoll_crear(void);
int epoll_agregarSocketServidor(int epollfd, int socketCliente);
int epoll_agregarSocketCliente(int epollfd, int socketCliente);
int epoll_agregarFdInotify(int epollfd, int fdInotify);
int epoll_escucharBloqueante(int epollfd, struct epoll_event * events);
int epoll_escucharTimeOut(int epollfd, struct epoll_event * events, int timeout);
int epoll_escucharGeneral(int epollfd, int uniqueSocket, void(*uniqueHandler)(epoll_data_t), void(*normalHandler)(epoll_data_t), void(*closeHandler)(epoll_data_t));
int epoll_escucharGeneralTimeOut(int epollfd, int uniqueSocket, void(*uniqueHandler)(epoll_data_t), void(*normalHandler)(epoll_data_t), void(*closeHandler)(epoll_data_t), int timeOut);

//FUNCIONES COMUNES
int socket_enviar(int socketReceptor, t_tipoEstructura tipoEstructura, void* estructura);
int socket_recibir(int socketEmisor, t_tipoEstructura * tipoEstructura, void** estructura);

int socket_enviarSignal(int socketReceptor, t_signal signal);
int socket_recibirSignal(int socketEmisor, t_signal *signal);

int socket_cerrarConexion(int socket);

//FUNCIONES DE MANIPULACION DE DIRECCIONES IP y PUERTO
char* socket_ip(char* direccionCompleta);
int socket_puerto(char* direccionCompleta);

char* socket_unirDireccion(char* ip, int puerto);


#endif /* SOCKET_H_ */
