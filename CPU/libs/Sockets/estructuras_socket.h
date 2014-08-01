/*
 * estructurasPackage.h
 *
 *  Created on: 28/05/2014
 *      Author: utnso
 */

#ifndef ESTRUCTURASPACKAGE_H_
#define ESTRUCTURASPACKAGE_H_

#include "commons/collections/list.h"
#include <stdint.h>
#include "parser/metadata_program.h"

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
	D_STRUCT_SOLICITARMEMORIA=6,
	D_STRUCT_GRABARBYTES=7,
	D_STRUCT_PUSH=8,
	D_STRUCT_POP=9, //Del tipo t_struct_numero
	D_STRUCT_ASIGNARCOMPARTIDA=11,
	D_STRUCT_INSTRUCCION=12,
	D_STRUCT_INDICE_ETIQUETAS=13,
	D_STRUCT_OBTENERCOMPARTIDA=14,
	D_STRUCT_WAIT=15,
	D_STRUCT_SIGNALSEMAFORO=16,
	D_STRUCT_VARIABLES=17,
	D_STRUCT_PCBSF = 18,
	D_STRUCT_SEGFAULT=19,
	D_STRUCT_NORMAL = 20,
	D_STRUCT_PCBIO = 21,
	D_STRUCT_PCBFIN= 22,
	D_STRUCT_SF=23,
	D_STRUCT_PCBSEM = 24,
	D_STRUCT_SEGCODIGO = 25,
	D_STRUCT_SOL_BYTES=29,
	D_STRUCT_ENV_BYTES=30,
	D_STRUCT_RESPUESTA_UMV=32,
	D_STRUCT_OBTENER_VALOR=33,
	D_STRUCT_ASIGNAR_VALOR=34,
	D_STRUCT_ENTRADA_SALIDA=35,
	D_STRUCT_IMPRIMIR=36,
	D_STRUCT_IMPRIMIRTEXTO = 37
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
	int pid;
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
typedef int t_program_counter;
typedef int t_tamanio_contexto;
typedef int t_tamanio_indice;
typedef int t_segmento_codigo;
typedef int t_segmento_stack;
typedef int t_cursor_stack;
typedef int t_index_codigo;
typedef int t_index_etiquetas;


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

typedef struct struct_pcb_fin{
		t_pid pid;								//Identificador único del Programa en el sistema
		t_segmento_codigo codigo;				//Dirección del primer byte en la UMV del segmento de código
		t_segmento_stack stack;					//Dirección del primer byte en la UMV del segmento de stack
		t_cursor_stack c_stack;					//Dirección del primer byte en la UMV del Contexto de Ejecución Actual
		t_index_codigo index_codigo;			//Dirección del primer byte en la UMV del Índice de Código
		t_index_etiquetas index_etiquetas;		//Dirección del primer byte en la UMV del Índice de Etiquetas
		t_program_counter	program_counter;	//Número de la próxima instrucción a ejecutar
		t_tamanio_contexto tamanio_contexto;	//Cantidad de variables (locales y parámetros) del Contexto de Ejecución Actual
		t_tamanio_indice tamanio_indice;		//Cantidad de bytes que ocupa el Índice de etiquetas
		char* variables;
} __attribute__ ((__packed__)) t_struct_pcb_fin;


typedef struct struct_pcb_io{
		t_pid pid;								//Identificador único del Programa en el sistema
		t_segmento_codigo codigo;				//Dirección del primer byte en la UMV del segmento de código
		t_segmento_stack stack;					//Dirección del primer byte en la UMV del segmento de stack
		t_cursor_stack c_stack;					//Dirección del primer byte en la UMV del Contexto de Ejecución Actual
		t_index_codigo index_codigo;			//Dirección del primer byte en la UMV del Índice de Código
		t_index_etiquetas index_etiquetas;		//Dirección del primer byte en la UMV del Índice de Etiquetas
		t_program_counter	program_counter;	//Número de la próxima instrucción a ejecutar
		t_tamanio_contexto tamanio_contexto;	//Cantidad de variables (locales y parámetros) del Contexto de Ejecución Actual
		t_tamanio_indice tamanio_indice;		//Cantidad de bytes que ocupa el Índice de etiquetas
		uint32_t tiempo;
		t_nombre_dispositivo dispositivo;
} __attribute__ ((__packed__)) t_struct_pcb_io;


/* Estructura tipo STRUCT_GRADOMP
 * envia el grado de multiprogramacion del sistema
 *
 */

typedef int t_gradoMP;

typedef struct struct_grado_MP{
	t_gradoMP gradoMP;
}__attribute__ ((__packed__)) t_struct_gradoMP;


/* Estructura tipo STRUCT_PIDYCODIGO
 * envia el pid de un programa y su respectivo codigo ansisop
 */

typedef int* pid;
typedef char* t_codigo;

typedef struct struct_pidycodigo{
	t_pid pid;
	t_codigo codigo;
}__attribute__ ((__packed__)) t_struct_pidycodigo;

/* Estructura tipo STRUCT_PUSH
 *
 */

typedef uint32_t t_posicion;
typedef int32_t t_valor;

typedef struct struct_push{
	t_posicion posicion;
	t_valor valor;
	t_puntero stack_base;
}__attribute__ ((__packed__)) t_struct_push;


/* Estructura tipo STRUCT_POP
 *
 */

typedef struct struct_pop{
	t_posicion posicion;
	t_size tamanio;
	t_puntero stack_base;
}__attribute__ ((__packed__)) t_struct_pop;


/* Estructura tipo STRUCT_ASIGNAR_COMPARTIDA
 *
 */

typedef char* t_nombre_compartida;

typedef struct struct_asignar_compartida{
	t_nombre_compartida nombre;
	t_valor valor;
}__attribute__ ((__packed__)) t_struct_asignar_compartida;

/* Estructura tipo STRUCT_SEGCODIGO
 *
 */

typedef struct struct_seg_codigo{
	t_intructions inst;
	t_puntero seg_codigo;
}__attribute__ ((__packed__)) t_struct_seg_codigo;

/* Estructura tipo STRUCT_INSTRUCCION
 *
 */

typedef struct struct_instruccion{
	t_puntero inst;
	t_puntero indice_codigo;
}__attribute__ ((__packed__)) t_struct_instruccion;

/* Estructura tipo STRUCT_PEDIR_INDICE_ETIQUETAS
 *
 */

typedef struct struct_etiquetas{
	t_puntero index_etiquetas;
	t_size etiquetas_size;
}__attribute__ ((__packed__)) t_struct_indice_etiquetas;

/* Estructura tipo STRUCT_PEDIR_SEMAFORO
 *
 */

typedef struct struct_semaforo{
	t_nombre_semaforo nombre_semaforo;
}__attribute__ ((__packed__)) t_struct_semaforo;

typedef struct struct_sol_bytes{
	uint32_t base;
	uint32_t offset;
	uint32_t tamanio;
}__attribute__ ((__packed__)) t_struct_sol_bytes;

/* Estructura tipo STRUCT_IO
 *
 */

typedef struct struct_io{
	uint32_t pid;
	t_nombre_dispositivo dispositivo;
	int32_t tiempo;
}__attribute__ ((__packed__)) t_struct_io;

typedef struct struct_respuesta_UMV{
	void* buffer;
	uint32_t tamano_buffer;
}__attribute__((__packed__)) t_struct_respuesta_umv;


/* Estructura tipo STRUCT_ENV_BYTES
 *
 */

typedef struct struct_env_bytes{
	uint32_t base;
	uint32_t offset;
	uint32_t tamanio;
	void* buffer;
}__attribute__ ((__packed__)) t_struct_env_bytes;


typedef struct struct_int_char{
	char* string;
	uint32_t tamano;
	uint32_t numero;
}__attribute__((__packed__)) t_struct_int_char;


#endif /* ESTRUCTURASPACKAGE_H_ */
