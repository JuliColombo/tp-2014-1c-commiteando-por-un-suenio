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
extern log_t* archLog;
extern char* PATH_config;
extern pthread_t io;
extern pthread_t plp, conexion_plp_programas, conexion_plp_umv, conexion_plp_cpu;
extern cola_procesos cola;
extern pthread_mutex_t* mutex_cola_new;
extern pthread_mutex_t* mutex_cola_ready;
extern pthread_mutex_t* mutex_cola_exec;
extern pthread_mutex_t* mutex_cola_block_io;
extern pthread_mutex_t* mutex_cola_block_sem;
extern pthread_mutex_t* mutex_cola_exit;
extern pthread_mutex_t* mutex_pid;
extern pthread_mutex_t* mutex_solicitarMemoria;
extern pthread_mutex_t* mutex_array;
extern pthread_mutex_t* mutex_semaforos;
extern pthread_mutex_t* mutex_var_compartidas;
extern pthread_mutex_t* mutex_log;
extern int sock_programas;
extern int sock_cpu;
extern int sock_umv;
extern int efd_cpu;
extern int efd_programas;
extern int* fds_conectados_cpu;
extern int* estado_cpu;
extern sem_t sem_multiProg, sem_ready, sem_new, sem_cpu;
extern int program_pid;


/************************* PROTOTIPOS DE FUNCIONES *************************/

int calcularPeso(t_medatada_program*); //Calcula el peso del programa
void agregarAColaSegunPeso(t_programa*, t_list*); //Agrega ordenadamente en la cola de new
void mostrarColasPorPantalla(t_list*, char*); //Muestra los programas que estan en New por pantalla
void inicializarConfiguracion(void); //Crea el Log. Si el archivo no existe escribe el log con el error correspondiente
void leerConfiguracion(void); //Lee la config del archivo y la asigna la struct correspondiente
void imprimirConfiguracion(void);
int cantidadProgramasEnPCP(void); //Retorna la cantidad de programas que estan en el pcp actualmente
void inicializarColas(void); //Inicializa las colas de new, ready, block, exec y exit
void crearPCB(t_programa); //Recive un programa y crea su PCB
void esperarYCerrarConexiones(void); //Espera a que terminen las conexiones y cierra sus hilos
t_pcb* crearPcb(char*,t_medatada_program*);
int solicitarMemoriaUMV(int tamanioSeg1, int tamanioSeg2, int tamanioSeg3, int tamanioSeg4, t_pcb*);
void enviar_pcb_a_cpu(void);

/************************* HILOS *************************/
void core_plp(void);
void core_pcp(void);
void core_io(t_struct_pcb_io* bloqueo);
void core_conexion_plp_programas(void);
void core_conexion_umv(void);
void core_conexion_pcp_cpu(void);


#endif /* FUNCIONESPLP_H_ */
