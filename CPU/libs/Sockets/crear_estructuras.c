/*
 * crear_estructuras.c
 *
 *  Created on: 20/06/2014
 *      Author: utnso
 */

#include "crear_estructuras.h"

t_struct_push* crear_struct_pushear(uint32_t posicion, int32_t valor) {
	t_struct_push* estructura = malloc(sizeof(t_struct_push));
	estructura->posicion=posicion;
	estructura->valor = valor;
	return estructura;
}

void socket_and_push(int sockAjeno, int posicion, int id) {
	t_struct_push* estructura = crear_struct_pushear(posicion,id);
	socket_enviar(sockAjeno, D_STRUCT_PUSH, estructura);
	free(estructura);
}

t_struct_pop* crear_struct_pop(uint32_t posicion){
	t_struct_pop* estructura = malloc(sizeof(t_struct_pop));
	estructura->posicion = posicion;
	return estructura;
}

void socket_and_pop_position(int sockAjeno,int posicion){
	t_struct_pop* estructura = crear_struct_pop(posicion);
	socket_enviar(sockAjeno, D_STRUCT_POP, estructura);
	free(estructura);
}

t_struct_modificar_top_index* crear_struct_modificar_top_index(int top_index){
	t_struct_modificar_top_index* estructura = malloc(sizeof(t_struct_modificar_top_index));
	estructura->posicion = top_index;
	return estructura;
}

void socket_and_modificar_top_index(int sockAjeno,int top_index){
	t_struct_modificar_top_index* estructura = crear_struct_modificar_top_index(top_index);
	socket_enviar(sockAjeno, D_STRUCT_MODIFICARTOPINDEX, estructura);
	free(estructura);
}

t_struct_string* crear_struct_string(char* string){
	t_struct_string* estructura = malloc(sizeof(t_struct_string));
	estructura->string = string;
	return estructura;
}

void socket_and_string(int sockAjeno,char* string){
	t_struct_string* estructura = crear_struct_string(string);
	socket_enviar(sockAjeno, D_STRUCT_STRING, estructura);
	free(estructura);
}

t_struct_asignar_compartida* crear_struct_asignar_compartida(char* string, int32_t valor){
	t_struct_asignar_compartida* estructura = malloc(sizeof(t_struct_asignar_compartida));
	estructura->nombre = string;
	estructura->valor = valor;
	return estructura;
}

void socket_and_asignar_compartida(int sockAjeno,char* string, int valor){
	t_struct_asignar_compartida* estructura = crear_struct_asignar_compartida(string, valor);
	socket_enviar(sockAjeno, D_STRUCT_ASIGNARCOMPARTIDA, estructura);
	free(estructura);
}

t_struct_instruccion* crear_struct_instruccion(t_intructions instruccion){
	t_struct_instruccion* estructura = malloc(sizeof(t_struct_instruccion));
	estructura->inst = instruccion;
	return estructura;
}

void socket_and_instruccion(int sockAjeno,t_intructions instruccion){
	t_struct_instruccion* estructura = crear_struct_instruccion(instruccion);
	socket_enviar(sockAjeno, D_STRUCT_INSTRUCCION, estructura);
	free(estructura);
}

t_struct_numero* crear_struct_numero(int32_t num){
	t_struct_numero* estructura = malloc(sizeof(t_struct_numero));
	estructura->numero = num;
	return estructura;
}

void socket_and_number(int sockAjeno,int num){
	t_struct_numero* estructura = crear_struct_numero(num);
	socket_enviar(sockAjeno, D_STRUCT_NUMERO, estructura);
	free(estructura);
}

void* socket_recibir_estructura(int sockAjeno){
	void* estructura;
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	socket_recibir(sockAjeno,&tipoRecibido,&structRecibida);
	return estructura;
}
