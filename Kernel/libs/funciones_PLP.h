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
#include "socket.h"
#include "funciones_aux.h"
#include <semaphore.h>

/************************* VARIABLES GLOBALES *************************/
extern t_config_kernel configuracion_kernel;
extern log_t* archLog;
extern char* PATH;
extern t_thread_io io;
extern pthread_t pcp, plp;
extern cola_procesos cola;

/************************* PROTOTIPOS DE FUNCIONES *************************/

void calcularPeso(t_programa programa); //Calcula el peso del programa
void agregarAColaSegunPeso(t_programa programa, t_list* lista); //Agrega ordenadamente en la cola de new
void mostrarNodosPorPantalla(t_list* lista); //Muestra los programas que estan en New por pantalla
void inicializarConfiguracion(void); //Crea el Log. Si el archivo no existe escribe el log con el error correspondiente
void leerConfiguracion(void); //Lee la config del archivo y la asigna la struct correspondiente
void imprimirConfiguracion(void);

/************************* HILOS *************************/
void* core_plp(void);
void* core_plp_conexiones(void);
void* core_pcp(void);
void* core_io(int);


#endif /* FUNCIONESPLP_H_ */
