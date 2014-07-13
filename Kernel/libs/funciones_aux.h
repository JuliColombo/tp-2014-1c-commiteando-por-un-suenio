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
void agregarNuevoPrograma(char* codigo, int fd);
void inicializarSemaforos(void);
void crearSemaforos(void);
void cerrarSemaforos(void);
void bloquearPrograma(int pid);

/************************* FUNCIONES AUXILIARES PARA EL MANEJO DE EPOLL *************************/
void manejar_ConexionNueva_Programas(epoll_data_t);
void manejar_ConexionNueva_CPU(epoll_data_t data);
void desconexion_cpu(epoll_data_t data);


#endif /* FUNCIONES_AUX_H_ */
