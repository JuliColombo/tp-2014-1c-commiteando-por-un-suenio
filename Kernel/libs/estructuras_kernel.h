#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <stdint.h>


typedef struct { //Hay que pensar bien esta estructura, porque no se puede tener mas de un array de longitud variable
	uint16_t puerto_programa;	//Puerto de programa
	uint16_t puerto_cpu;	//Puerto de Cpu
	uint8_t quantum;	//Quantum
	uint8_t retardo_quantum;	//Retardo del Quantum
	uint8_t grado_mp;	//Grado de multiprogramacion
	//char id_semaforos[];	//Identificadores de semaforos
	//uint8_t semaforos[];	//Valores de semaforos
	//uint8_t retardo_hio[];	//Retardo de hio
	//char id_hio[];	//Identificadores de hio
	uint16_t tamanio_mp;	//Tamaño fijo de la memoria principal
} archivo_config_kernel;

typedef struct {
	t_list* lista;
	pthread_mutex_t* mutex;
	int8_t* prioridad;
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

typedef struct pares{
		int desplazamiento;
		int longitudLinea;
} Pares;

typedef struct {
	t_hash_element **elements;
	int elements_amount;
} diccionario;

typedef struct pcb {
		int ID;
	    int* CODE, STACK, CursorSTACK;
	    //estaba int* indiceEtiquetas, pero por lo que dice la consigna: el indice de etiquetas constara de la serializacion
	    //de un diccionario que ontiene el id de cada funcion o etiqueta...
	    diccionario* indiceEtiquetas;
	    int* ProgramCounter;
	    int TamanioContext;
	    Pares indiceCodigo[];
} PCB;

#endif /* ESTRUCTURAS_H_ */


