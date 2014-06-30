/*
 * crear_estructuras.h
 *
 *  Created on: 20/06/2014
 *      Author: utnso
 */

#ifndef CREAR_ESTRUCTURAS_H_
#define CREAR_ESTRUCTURAS_H_

#include "estructuras_socket.h"
#include "socket.h"
#include "package.h"

t_struct_push* crear_struct_pushear(uint32_t posicion, int32_t valor);
void socket_and_push(int sockAjeno, int posicion, int id);
t_struct_pop* crear_struct_pop(uint32_t posicion);
void socket_and_pop_position(int sockAjeno,int posicion);
t_struct_modificar_top_index* crear_struct_modificar_top_index(int top_index);
void socket_and_modificar_top_index(int sockAjeno,int top_index);
t_struct_string* crear_struct_string(char* string);
void socket_and_string(int sockAjeno,char* string);
t_struct_asignar_compartida* crear_struct_asignar_compartida(char* string, int32_t valor);
void socket_and_asignar_compartida(int sockAjeno,char* string, int valor);

void* socket_recibir_estructura(int sockAjeno);

#endif /* CREAR_ESTRUCTURAS_H_ */
