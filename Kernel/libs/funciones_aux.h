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

enum{
	LIBRE,
	USADA
};


int escribir_log(log_t *log, const char *program_name, e_message_type type,	const char* format);
int* vector_num(char**, char**);
int validarSemaforo(char* semaforo);
int validarVarGlobal(char* var_global);
int cant_identificadores(char**);
void log_error_socket(void);
int posicion_Variable_Global(char*);
int valor_Variable_Global(char* variable);
int posicion_Semaforo(char* semaforo);
void liberarCPU(int fd);
int agregarNuevoPrograma(char* codigo, int fd);
void inicializarMutex(void);
void crearSemaforos(void);
void cerrarSemaforos(void);
void bloquearPrograma(int pid);
void actualizarPCB(t_programa* programa, t_struct_pcb* pcb);
void* buscarPrograma(int pid, t_list* lista,pthread_mutex_t *mutex);
void mandarAOtraCola(t_programa* programa, t_list* listaActual, pthread_mutex_t* mutexActual,
		t_list* listaNueva, pthread_mutex_t* mutexNuevo);
int buscar_cpu_libre(void);
int buscar_cpu_por_fd(int fd);
void finalizarPrograma(t_programa* programa, char*);


/************************* FUNCIONES AUXILIARES PARA EL MANEJO DE EPOLL *************************/
void manejar_ConexionNueva_Programas(epoll_data_t);
void manejar_ConexionNueva_CPU(epoll_data_t data);
void handler_conexion_cpu(epoll_data_t data);
void desconexion_cpu(epoll_data_t data);


#endif /* FUNCIONES_AUX_H_ */
