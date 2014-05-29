#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <stdint.h>
#include <pthread.h>
#include "commons/collections/list.h"
#include "parser/metadata_program.h"

typedef uint16_t t_puerto_programa;
typedef uint16_t t_puerto_cpu;
typedef uint16_t t_tamanio_mp;
typedef uint16_t t_ip_umv;
typedef uint16_t t_puerto_umv;

typedef uint8_t t_quantum;
typedef uint8_t t_retardo_quantum;
typedef uint8_t t_grado_multip;

typedef int t_pid;
typedef int t_program_counter;
typedef int tamanio_indice;
typedef int t_tamanio_contexto;
typedef int t_tamanio_indice;
typedef int* t_segmento_codigo;
typedef int* t_segmento_stack;
typedef int* t_cursor_stack;
typedef int* t_index_codigo;
typedef int* t_index_etiquetas;

typedef char** t_id_semaforos;
typedef char** t_id_hio;
typedef int* t_valor_semaforos;
typedef int* t_retardo_hio;
typedef char** t_variables_globales;

typedef int8_t* t_prioridad;
typedef int8_t* t_nombre_cola;


typedef struct{ //Hay que pensar bien esta estructura, porque no se puede tener mas de un array de longitud variable
	t_puerto_programa puerto_programas;		//Puerto TCP utilizado para recibir las conexiones de los Programas
	t_puerto_cpu puerto_cpus;				//Puerto TCP utilizado para recibir las conexiones de los CPUs
	t_puerto_umv puerto_umv;				//Puerto de la UMV
	t_quantum quantum;						//Valor del Quantum (en instrucciones a ejecutar) del algoritmo Round Robin
	t_retardo_quantum retardo_quantum;		//Valor de retardo en milisegundos que el CPU deberá esperar luego de ejecutar cada	sentencia
	t_grado_multip multiprogramacion;		//Grado de multiprogramacion del sistema
	t_id_semaforos id_semaforos;			//Identificador de cada semáforo del sistema. Cada posición del array representa un semáforo
	t_valor_semaforos valor_semaforos;		//Valor inicial de cada semáforo
	t_id_hio id_hio;						//Identificador de cada dispositivo de entrada/salida
	t_retardo_hio retardo_hio;				//Retardo en milisegundos de cada unidad de	operación de entrada/salida.
	t_ip_umv ip_umv;						//IP de la UMV
	t_variables_globales var_globales;	//Variables globales del sistema
} t_config_kernel;

typedef struct{
	t_list* lista;
	pthread_mutex_t* mutex;
	t_prioridad prioridad;
	t_nombre_cola nombre_de_la_Cola;
} t_lista_programas;

typedef struct{
	t_list* new;
	t_list* ready;
	t_list* exec;
	t_list* block;
	t_list* exit;
} cola_procesos;

typedef struct{
	t_pid pid;								//Identificador único del Programa en el sistema
	t_segmento_codigo codigo;				//Dirección del primer byte en la UMV del segmento de código
	t_segmento_stack stack;					//Dirección del primer byte en la UMV del segmento de stack
	t_cursor_stack c_stack;					//Dirección del primer byte en la UMV del Contexto de Ejecución Actual
	t_index_codigo index_codigo;			//Dirección del primer byte en la UMV del Índice de Código
	t_index_etiquetas index_etiquetas;		//Dirección del primer byte en la UMV del Índice de Etiquetas
	t_program_counter	program_counter;	//Número de la próxima instrucción a ejecutar
	t_tamanio_contexto tamanio_contexto;	//Cantidad de variables (locales y parámetros) del Contexto de Ejecución Actual
	t_tamanio_indice tamanio_indice;		//Cantidad de bytes que ocupa el Índice de etiquetas
} t_pcb;

typedef struct {
	int peso;
	t_medatada_program metadata;
	t_pcb* pcb;
	t_quantum quantum;
	int flag_terminado;
	int flag_bloqueado;
} t_programa;

typedef struct{
	pthread_t tid;
}t_thread_array;


typedef struct{
	int a;
	pthread_mutex_t mutex;
	t_thread_array thread[];
} t_thread_io;

#endif /* ESTRUCTURAS_H_ */


