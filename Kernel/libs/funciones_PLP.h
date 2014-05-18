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


extern t_config_kernel configuracion_kernel;
extern log_t* archLog;
extern char* PATH;

typedef struct { // se define un vector para los programas en new
	int peso;
	t_medatada_program metadata;

} t_programa;

void calcularPeso(t_programa programa); //Calcula el peso del programa
void agregarAColaSegunPeso(t_programa programa, t_list* lista); //Agrega ordenadamente en la cola de new
void mostrarNodosPorPantalla(t_list* lista); //Muestra los programas que estan en New por pantalla
void inicializarConfiguracion(char*); //Crea el Log. Si el archivo no existe escribe el log con el error correspondiente
void leerConfiguracion(char*); //Lee la config del archivo y la asigna la struct correspondiente

void imprimirConfiguracion(t_config_kernel);

void* core_plp(void);
void* core_plp_conexiones(void);
void* core_pcp(void);
void* core_io(void);


#endif /* FUNCIONESPLP_H_ */
