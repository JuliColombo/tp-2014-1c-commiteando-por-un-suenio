/*
 * Estructuras.h
 *
 *  Created on: 23/04/2014
 *      Author: utnso
 */

/*Ideas de estructuras del tp, por ahora archivo_config y pcb*/


#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_




typedef struct {

	uint16_t puerto_programa;	//Puerto de programa
	uint16_t puerto_cpu;	//Puerto de Cpu
	uint8_t quantum;	//Quantum
	uint8_t retardo_quantum;	//Retardo del Quantum
	uint8_t grado_mp;	//Grado de multiprogramacion
	char id_semaforos[];	//Identificadores de semaforos
	uint8_t semaforos[];	//Valores de semaforos
	uint8_t retardo_hio[];	//Retardo de hio
	char id_hio[];	//Identificadores de hio

} archivo_config;






typedef struct {

	t_link_element *head;
	int elements_count;

} t_list;

typedef struct {

	t_list* lista;
	pthread_mutex_t* mutex;
	uint8_t* prioridad;
	int8_t* nombre_de_la_Cola;

} lista;


typedef struct {

	uint16_t valor;
	char* nombre;

} variable_t;

typedef struct {

	char* nombre;
	int llamada;

} stk_funcion;


typedef struct {

	char** linea;
	uint16_t cantidad;

} list_codigo;

typedef struct {

	variable_t* lista;
	uint16_t cantidad;

} list_variables;

typedef struct {

	stk_funcion* lista;
	uint16_t cantidad;

} list_funciones;



typedef struct {

	uint32_t pid;		 	 // ID del proceso
	int pc; 		 	 // Program counter
	int* cursor_stack;	//Cursor del Stack
	list_codigo codigo;  	 // Codigo del script
	list_variables datos; 	 // Tamaño de Contexto Actual
	list_funciones stack; 	 // Stack funciones

} t_pcb;

#endif /* ESTRUCTURAS_H_ */


