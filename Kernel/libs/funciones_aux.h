/*
 * funciones_aux.h
 *
 *  Created on: 18/05/2014
 *      Author: utnso
 */

#ifndef FUNCIONES_AUX_H_
#define FUNCIONES_AUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "log.h"
#include <sys/epoll.h>
#include "funciones_PLP.h"
#include "Sockets/socket.h"

int* vector_num(char**, char**);
int cant_identificadores(char**);
void log_error_socket(void);
int posicion_Variable_Global(char*);

/************************* FUNCIONES AUXILIARES PARA EL MANEJO DE EPOLL *************************/
void aceptarConexionEntrante (struct epoll_event);
void* manejar_ConexionNueva_Programas(struct epoll_event);


#endif /* FUNCIONES_AUX_H_ */
