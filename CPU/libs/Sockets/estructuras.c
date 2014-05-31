/*
 * estructuras.c
 *
 *  Created on: 31/05/2014
 *      Author: utnso
 */

#include "estructuras.h"

struct_push* crear_struct_push(uint32_t posicion, int id) {
	struct_push* estructura;
	estructura->id=id;
	estructura->posicion=posicion;
	return estructura;
}

struct_pop_desreferenciar* crear_struct_pop_desreferenciar(uint32_t posicion) {
	struct_pop_desreferenciar* estructura;
	estructura->posicion=posicion;
	return estructura;
}

struct_pop_retornar* crear_struct_pop_retornar(uint32_t posicion) {
	struct_pop_retornar* estructura;
	estructura->posicion = posicion;
	return estructura;
}

struct_numero* crear_struct_numero(uint32_t numero) {
	struct_numero* estructura;
	estructura->numero = numero;
	return estructura;
}
