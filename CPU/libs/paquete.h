/*
 * paquete.h
 *
 *  Created on: 29/05/2014
 *      Author: utnso
 */

#ifndef PAQUETE_H_
#define PAQUETE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define STRUCT_PUSH 0
#define STRCUT_POP 1
#define STRUCT_POP_DESREFERENCIAR 2
#define STRUCT_POP_RETORNAR 3


typedef struct {
	uint8_t Type;
	uint32_t Lenght;

}__attribute__((packed)) t_nipc;


typedef struct {

	uint32_t length;
	char* buffer;
}__attribute__((packed)) t_stream ;

/****************************************ESTRUCTURAS DE OPERACIONES*************************************************/
typedef enum {
	PUSH_SIZE_CHECK,
	POP,
	POP_DESREFERENCIAR,
	POP_RETORNAR,
	ACTUALIZAR_TOP_INDEX, //??
	RECIBIR_INST, //??
	OBTENER_COMPARTIDA,
	ASIGNAR_COMPARTIDA,
	IMPRIMIR,
	IMPRIMIR_TEXTO,
	WAIT,
	SIGNAL,
}tipo_operacion;

typedef struct {
	uint32_t posicion;
	char id;
}__attribute__((packed)) struct_push;


typedef struct {

} __attribute__((packed)) struct_pop;


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

}__attribute__((packed)) struct_signal;


/****************************************************FUNCIONES******************************************************/

void nipc_destruirPaquete(t_stream* paquete);
t_stream* nipc_serializar2(int type, void* estructura);
t_nipc cargarData(uint8_t estructura, uint32_t length);
char* crearData(uint8_t estructura, uint32_t length);
t_stream* nipc_paquetePopRetornar(struct_pop_retornar* estructura);
t_stream* nipc_paquetePopDesreferenciar(struct_pop_desreferenciar* estructura);
t_stream* nipc_paquetePop(struct_pop* estructura);
t_stream* nipc_paquetePush(struct_push* estructura);

struct_push* nipc_sacarPaquetePush(char* data, uint32_t length);
struct_push* nipc_sacarPaquetePop(char* data, uint32_t length);
struct_push* nipc_sacarPaquetePopDesreferenciar(char* data, uint32_t length);
struct_push* nipc_sacarPaquetePopRetornar(char* data, uint32_t length);
void *nipc_deserializar2(int type, char* data, uint32_t length);

#endif /* PAQUETE_H_ */
