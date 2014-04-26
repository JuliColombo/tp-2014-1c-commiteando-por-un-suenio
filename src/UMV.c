/*
 * UMV.c
 *
 *  Created on: 21/04/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "commons/config.h"
#include "commons/collections/dictionary.h"
#include "FuncionesPLP.h"
#include <Estructuras.h>
//Al iniciar, la UMV crea la MP de un tamaño configurable por el archivo de configuracion, que supuse era
//el tamaño maximo que puede tener el diccionario de ese archivo de configuracion

archivo_config configuracion;
// Ojo!! Correccion del enunciado dice que es un campo del archivo,si es asi hay que revisar esto
int* crearMP(t_config archConfig) {
	int tamanio = (archConfig.properties)->table_max_size;
	int* MP = malloc(tamanio);

	return MP;
}


//Funcion que recibe el programa del PLP y le reserva memoria (si puede)
_Bool solicitarMemoria(t_programa programa){
	return true;
}

//Operacion Basica de UMV 1, se toma una cantidad de bytes (longitud) desde la posicion de memoria dada(base+offset)?
void solicitarBytes(uint32_t base,uint32_t offset, uint32_t longitud){

}

//Operacion Basica de UMV 2, se envia una cantidad de bytes (en el buffer?) a la posicion dada(base+offset)
void enviarBytes(uint32_t base,uint32_t offset, uint32_t longitud,t_buffer buffer){

}

//Dada una solicitud (solo necesita longitud?) responde True o genera Excepcion
_Bool validarSolicitud(uint32_t longitud){
	if(/*MemoriaSuficiente*/){
	}
	return true;
}

void leerConfiguracion(void) {

	t_config* config = config_create("/home/utnso/workspace/tp-2014-1c-commiteando-por-un-suenio/src");

	configuracion.puerto_programa = config_get_int_value(config,
			"Puerto TCP para recibir conexiones de Procesos InterpretesCPU");

	configuracion.puerto_cpu = config_get_int_value(config,
			"Puerto TCP para recibir conexiones de Procesos Interpretes");

	configuracion.quantum = config_get_int_value(config,
			"Quantum de tiempo para algoritmos expropiativos");

	configuracion.retardo_quantum = config_get_int_value(config,
			"Retardo del Quantum");

	configuracion.grado_mp = config_get_int_value(config,
			"Maximo nivel de multiprogramacion");

	configuracion.id_semaforos = config_get_array_value(config,
			"Lista de nombres de Semaforos");

	configuracion.semaforos = config_get_array_value(config,
			"Lista de valores de Semaforos");

	configuracion.retardo_hio = config_get_array_value(config,
			"Retardo de hio");

	configuracion.id_hio = config_get_array_value(config,
			"Lista de hio");

	configuracion.tamanio_mp = config_get_int_value(config,
				"Tamanio de Memoria Principal");

}

//Esta bien que esto sea una funcion de la UMV?
void imprimirConfiguracion(void) { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("%d\n", configuracion.puerto_programa);
	printf("%d\n", configuracion.puerto_cpu);
	printf("%d\n", configuracion.quantum);
	printf("%d\n", configuracion.retardo_quantum);
	printf("%d\n", configuracion.grado_mp);
	printf("%d\n", configuracion.id_semaforos);
	printf("%d\n", configuracion.semaforos);
	printf("%d\n", configuracion.retardo_hio);
	printf("%d\n", configuracion.id_hio);
	printf("%d\n", configuracion.tamanio_mp);

}


//Comandos de consola:

void operacion(t_proceso proceso, uint32_t base,uint32_t offset, uint32_t longitud){
	//Solicitar una posicion de memoria o escribir un buffer por teclado en una posicion
	/*if ( esValido(buffer) ){
	 * void enviarBytes(base,offset,longitud,buffer)
	 * }else{
	 *  solicitarBytes(base,offset,longitud)
	 *  } */
	printf(/*posicionDeMemoria*/);
	/*OPCIONAL: grabar en archivo*/
}

void retardo(int valorRetardoEnMilisegundos){ //Cantidad de ms que debe esperar UMV para responder una solicitud

}

void algoritmo(char tipo){//Cambiar entre Worst fit y First fit

//TODO compactar y dump !!

}
