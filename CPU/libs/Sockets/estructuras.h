/*
 * estructuras.h
 *
 *  Created on: 31/05/2014
 *      Author: utnso
 */

#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <stdint.h>
#include <parser/metadata_program.h>

typedef enum {
	POP,
	TOP,
	ACTUALIZAR_TOP_INDEX, //??
	RECIBIR_INST, //??
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
	uint32_t signal;
}__attribute__((packed)) struct_signal;


typedef struct {
	uint32_t numero;
}__attribute__((packed)) struct_numero;


typedef struct {
	char letra;
}__attribute__((packed)) struct_char;


typedef struct {
	char * string;
} __attribute__ ((__packed__)) struct_string;


typedef struct {
	char * id;
	uint32_t valor;
} __attribute__ ((__packed__)) struct_asignar_compartida;


typedef struct {
	t_intructions instruccion;
}__attribute__ ((__packed__)) struct_tipo_instruccion;


typedef struct {
	t_nombre_semaforo semaforo;
	uint32_t operacion;
}__attribute__ ((__packed__)) struct_semaforo;


typedef struct {
	t_puntero top;
}__attribute__((__packed__)) struct_modificar_top_index;


struct_push* crear_struct_push(uint32_t posicion, int id);
struct_pop_desreferenciar* crear_struct_pop_desreferenciar(uint32_t posicion);
struct_pop_retornar* crear_struct_pop_retornar(uint32_t posicion);
struct_numero* crear_struct_numero(uint32_t numero);
struct_char* crear_struct_char(char letra);
struct_string* crear_struct_string(char* string);
struct_asignar_compartida* crear_struct_asignar_compartida(char* id, uint32_t valor);
struct_semaforo* crear_struct_semaforo(t_nombre_semaforo semaforo, uint32_t operacion);
struct_tipo_instruccion* crear_struct_tipo_instruccion(t_intructions instruccion);
struct_modificar_top_index* crear_struct_modificar_top_index(t_puntero top);


#endif /* ESTRUCTURAS_H_ */
