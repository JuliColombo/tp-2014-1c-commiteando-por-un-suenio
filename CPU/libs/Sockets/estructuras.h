/*
 * estructuras.h
 *
 *  Created on: 31/05/2014
 *      Author: utnso
 */

#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <stdint.h>

typedef enum {
	POP,
	ACTUALIZAR_TOP_INDEX, //??
	RECIBIR_INST, //??
	OBTENER_COMPARTIDA,
	ASIGNAR_COMPARTIDA,
	IMPRIMIR,
	IMPRIMIR_TEXTO,
	WAIT,
	SIGNAL,
}tipo_operacion_signal;

typedef struct {
	uint32_t posicion;
	int id;
}__attribute__((packed)) struct_push;


typedef struct {
	uint32_t posicion;
} __attribute__((packed)) struct_pop_desreferenciar;


typedef struct {
	uint32_t posicion;
} __attribute__((packed)) struct_pop_retornar;


typedef struct {
	char id;
}__attribute__((packed)) struct_imprimir;

typedef struct {
	char* texto;
}__attribute__((packed)) struct_imprimir_texto;


typedef struct {

}__attribute__((packed)) struct_wait;


typedef struct {
	uint32_t signal;
}__attribute__((packed)) struct_signal;

typedef struct {
	uint32_t numero;
}__attribute__((packed)) struct_numero;


struct_push* crear_struct_push(uint32_t posicion, int id);
struct_pop_desreferenciar* crear_struct_pop_desreferenciar(uint32_t posicion);
struct_pop_retornar* crear_struct_pop_retornar(uint32_t posicion);


#endif /* ESTRUCTURAS_H_ */
