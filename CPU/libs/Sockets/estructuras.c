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
	estructura->posicion = posicion;
	return estructura;
}

struct_pop_retornar* crear_struct_pop_retornar(uint32_t posicion) {
	struct_pop_retornar* estructura;
	estructura->posicion = posicion;
	return estructura;
}

struct_pop_position* crear_struct_pop_position(uint32_t posicion) {
	struct_pop_position* estructura;
	estructura->posicion = posicion;
	return estructura;
}

struct_numero* crear_struct_numero(uint32_t numero) {
	struct_numero* estructura;
	estructura->numero = numero;
	return estructura;
}

struct_char* crear_struct_char(char letra) {
	struct_char* estructura;
	estructura->letra = letra;
	return estructura;
}

struct_string* crear_struct_string(char* string) {
	struct_string* estructura;
	estructura->string = string;
	return estructura;
}

struct_asignar_compartida* crear_struct_asignar_compartida(char* id, uint32_t valor) {
	struct_asignar_compartida* estructura;
	estructura->id = id;
	estructura->valor = valor;
	return estructura;
}

struct_tipo_instruccion* crear_struct_tipo_instruccion(t_intructions instruccion) {
	struct_tipo_instruccion* estructura;
	estructura->instruccion = instruccion;
	return estructura;
}

struct_semaforo* crear_struct_semaforo(t_nombre_semaforo semaforo, uint32_t operacion) {
	struct_semaforo* estructura;
	estructura->semaforo= semaforo;
	estructura->operacion= operacion;
	return estructura;
}

struct_modificar_top_index* crear_struct_modificar_top_index(t_puntero top){
	struct_modificar_top_index* estructura;
	estructura->top = top;
	return estructura;
}
