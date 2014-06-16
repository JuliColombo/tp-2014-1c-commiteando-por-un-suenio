/*
 * FuncionesPLP.h
 *
 *  Created on: 25/04/2014
 *      Author: utnso
 */

#ifndef FUNCIONESPLP_H_
#define FUNCIONESPLP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "estructuras_kernel.h"
#include "parser/metadata_program.h"
#include "commons/config.h"
#include "commons/collections/list.h"
#include "log.h"
#include "Sockets/socket.h"
#include "funciones_aux.h"
#include <semaphore.h>
#include <sys/epoll.h>

/************************* VARIABLES GLOBALES *************************/
extern t_config_kernel configuracion_kernel;
extern int* valor_var_globales;
extern log_t* archLog;
extern char* PATH;
extern t_thread_io io;
extern pthread_t plp, conexion_plp_programas, conexion_plp_umv, conexion_plp_cpu;
extern pthread_t pcp, conexion_pcp_cpu, hilo_pcp_ready, hilo_pcp_new;
extern cola_procesos cola;
extern pthread_mutex_t* mutex_cola_new;
extern pthread_mutex_t* mutex_cola_ready;
extern pthread_mutex_t* mutex_cola_exec;
extern pthread_mutex_t* mutex_cola_block;
extern pthread_mutex_t* mutex_cola_exit;
extern int sock_programas;
extern int sock_cpu;
extern int sock_umv;
extern int* fds_conectados_programas, fds_conectados_cpu;
extern sem_t sem_new_programas, sem_new_multip, sem_ready, sem_cpu;


/************************* PROTOTIPOS DE FUNCIONES *************************/

void calcularPeso(t_programa); //Calcula el peso del programa
void agregarAColaSegunPeso(t_programa, t_list*); //Agrega ordenadamente en la cola de new
void mostrarNodosPorPantalla(t_list*); //Muestra los programas que estan en New por pantalla
void inicializarConfiguracion(void); //Crea el Log. Si el archivo no existe escribe el log con el error correspondiente
void leerConfiguracion(void); //Lee la config del archivo y la asigna la struct correspondiente
void imprimirConfiguracion(void);
int cantidadProgramasEnEjecucion(void); //Retorna la cantidad de programas que estan en el pcp actualmente
void completarGradoMultip(void); //Completa grado de multiprogramacion del sistema
void inicializarColas(void); //Inicializa las colas de new, ready, block, exec y exit
void crearPCB(t_programa); //Recive un programa y crea su PCB
void esperarYCerrarConexiones(void); //Espera a que terminen las conexiones y cierra sus hilos
t_pcb crearPcb(char*);

/************************* HILOS *************************/
void core_plp(void);
void core_pcp(void);
void core_io(int);
void core_conexion_plp_programas(void);
void core_conexion_umv(void);
void core_conexion_pcp_cpu(void);
void core_pcp_ready(void);
void core_pcp_new(void);


#endif /* FUNCIONESPLP_H_ */
