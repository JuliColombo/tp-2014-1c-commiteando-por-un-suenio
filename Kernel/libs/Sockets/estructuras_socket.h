/*
 * estructurasPackage.h
 *
 *  Created on: 28/05/2014
 *      Author: utnso
 */

#ifndef ESTRUCTURASPACKAGE_H_
#define ESTRUCTURASPACKAGE_H_

#include "commons/collections/list.h"

/* Defino los tipos de señales que se pueden mandar
 *
 * Sintaxis correcta para escribir una nueva señal:
 *
 * 	#define S_[Origen]_[Destino]_[Mensaje] numeroIncremental
 *
 * 	Donde:
 * 		[Origen] es quien manda el mensaje
 * 		[Destino] es quien recibe el mensaje
 * 		[Mensaje] lo que se le quiere mandar
 * 		numeroIncrementar un numero mas que la señal anterior
 */
#define S_ERROR 0	// Utilizada en socket_recibirSeñal, para decir que lo que se recibio no era una señal


/*Defino las estructuras de los paquetes
 *
 * Sintaxis correcta para escribir un nuev mensaje:
 *
 * 	#define D_STRUCT_[Nombre Estructura]
 *
 * 	Donde:
 * 		[Nombre Estructura] es el nombre de la estructura en MAYUSCULA
 *
 * 	Nota:
 * 		No olvidarse de crear la estructura y su paquetizador y despaquetizador asociado!
 */

enum{
	D_STRUCT_NOMBREMENSAJE=0,
	D_STRUCT_NUMERO=1,
	D_STRUCT_CHAR=2,
	D_STRUCT_STRING=3,
	D_STRUCT_SIGNAL=4,
	D_STRUCT_PCB=5,
	D_STRUCT_GRADOMP=6
};



//Estructura auxiliar de stream
typedef struct {
	int length;
	char *data;
} t_stream;

// Header de stream
typedef struct {
	uint8_t tipoEstructura;
	uint16_t length;
} __attribute__ ((__packed__)) t_header;


/*
 *	TYPEDEF DE LAS ESTRUCTURAS A ENVIAR
 */

//Estructura tipo STRUCT_NOMBREMENSAJE(0)
typedef struct struct_nombreMensaje {
	char * nombre;
	char * mensaje;
} __attribute__ ((__packed__)) t_struct_nombreMensaje;

//Estructura tipo STRUCT_NUMERO(1)
typedef struct struct_numero {
	unsigned int numero;
} __attribute__ ((__packed__)) t_struct_numero;

//Estructura tipo STRUCT_CHAR(2)
typedef struct struct_char {
	char letra;
} __attribute__ ((__packed__)) t_struct_char;

//Estructura tipo STRUCT_STRING(3)
typedef struct struct_string {
	char * string;
} __attribute__ ((__packed__)) t_struct_string;

/*Estructura tipo STRUCT_SIGNAL(4)
 * se va a usar cuando quiera enviar una señal.
 * cada señal va a estar numerada. Entonces aca envio el nro de señal.
 */
typedef unsigned int t_signal;

typedef struct struct_signal {
	t_signal signal;
} __attribute__ ((__packed__)) t_struct_signal;

/* Estructura tipo STRUCT_PCB (5)
 * se usa para enviar una pcb
 *
 */

typedef unsigned int t_pid;
typedef int* t_segmento_codigo;
typedef int* t_segmento_stack;
typedef int* t_cursor_stack;
typedef int* t_index_codigo;
typedef int* t_index_etiquetas;
typedef int t_program_counter;
typedef int t_tamanio_contexto;
typedef int t_tamanio_indice;


typedef struct struct_pcb{
		t_pid pid;								//Identificador único del Programa en el sistema
		t_segmento_codigo codigo;				//Dirección del primer byte en la UMV del segmento de código
		t_segmento_stack stack;					//Dirección del primer byte en la UMV del segmento de stack
		t_cursor_stack c_stack;					//Dirección del primer byte en la UMV del Contexto de Ejecución Actual
		t_index_codigo index_codigo;			//Dirección del primer byte en la UMV del Índice de Código
		t_index_etiquetas index_etiquetas;		//Dirección del primer byte en la UMV del Índice de Etiquetas
		t_program_counter	program_counter;	//Número de la próxima instrucción a ejecutar
		t_tamanio_contexto tamanio_contexto;	//Cantidad de variables (locales y parámetros) del Contexto de Ejecución Actual
		t_tamanio_indice tamanio_indice;		//Cantidad de bytes que ocupa el Índice de etiquetas
} __attribute__ ((__packed__)) t_struct_pcb;

/* Estructura tipo STRUCT_GRADOMP
 * envia el grado de multiprogramacion del sistema
 *
 */

typedef int t_gradoMP;

typedef struct grado_MP{
	t_gradoMP gradoMP;
}__attribute__ ((__packed__)) t_struct_gradoMP;



#endif /* ESTRUCTURASPACKAGE_H_ */
