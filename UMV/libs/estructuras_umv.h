/*
 * estructuras_umv.h
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */

#ifndef ESTRUCTURAS_UMV_H_
#define ESTRUCTURAS_UMV_H_

#include <pthread.h>
#include "parser/metadata_program.h"
#include "commons/collections/list.h"
#include <stdint.h>

#define TAMANIO_NULO -2

typedef uint16_t t_puerto_cpu;
typedef uint16_t t_puerto_conexiones;
typedef int t_memoria_principal;
typedef int t_algoritmo;
typedef char* t_ip_kernel;
typedef char** t_id_semaforos;
typedef uint8_t* t_valor_semaforos;

typedef void* t_buffer;

typedef struct { // se define un vector para los programas en new
	int peso;
	t_medatada_program metadata;

} t_programa;

typedef struct t_config_UMV {
t_memoria_principal memSize;	//Tamanio de la memoria principal
t_puerto_conexiones puerto_conexiones;	//Puerto tcp para recibir las conexiones
t_algoritmo algoritmo;	//Algoritmo de selección de ubicación de segmento (first-fit o worst-fit)
} t_config_UMV;

enum tipo_algoritmo{
					firstfit,worstfit
};
typedef struct{
	int fd;
}sock_struct;

typedef enum tipo_handshake{
	kernel=0,
	cpu=1
}tipo_handshake;


//Definicion de Structss
typedef struct{
   int dir_virtual;
   int programa;
   void* puntero_MP;
   int dir_real;
   int tamano;
}Segmento;

typedef struct{
   int dir_real;
   int tamano;
}RangoMemoria;

typedef struct{
   int dir_real;
   int dir_virtual;
}Base_ID;

#endif /* ESTRUCTURAS_UMV_H_ */
