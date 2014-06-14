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
#include <stdint.h>

typedef uint16_t t_puerto_cpu;
typedef uint16_t t_puerto_kernel;
typedef uint32_t t_memoria_principal;
typedef int t_algoritmo;
typedef char* t_ip_kernel;
typedef char** t_id_semaforos;
typedef uint8_t* t_valor_semaforos;

typedef uint32_t* t_buffer; //TODO ojo esto,no estoy seguro

typedef struct { // se define un vector para los programas en new
	int peso;
	t_medatada_program metadata;

} t_programa;

typedef struct t_config_UMV {
t_memoria_principal memSize;	//Tamanio de la memoria principal
t_puerto_cpu puerto_cpus;	//Puerto tcp para recibir las conexiones de las cpus
t_puerto_kernel puerto_kernel;	//Puerto tcp para recibir las conexiones del kernel
t_ip_kernel ip_kernel;	//Direccion IP para conectarse al Kernel
t_algoritmo algoritmo;	//Algoritmo de selección de ubicación de segmento (first-fit o worst-fit)
} t_config_UMV;

enum tipoAlgoritmo{
					firstfit,worstfit
};

typedef struct segmentDescriptor{
	//tipo segmento segmento; No iría algo de esto???
	int inicio;
	int tamanio;
	int ubicacionMP;
} segmentDescriptor;


typedef struct tablaSeg{
	int id_prog;
	segmentDescriptor* segmentos;
}tablaSeg;

#endif /* ESTRUCTURAS_UMV_H_ */
