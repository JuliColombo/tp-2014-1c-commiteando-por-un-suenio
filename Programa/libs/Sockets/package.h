/*
 * package.h
 *
 *  Created on: 28/05/2014
 *      Author: utnso
 */

#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include "estructuras_socket.h"


//FUNCION PRINCIPAL PAQUETIZAR
t_stream* paquetizar(int tipoEstructura, void* estructuraOrigen);
//FUNCION PAQUETIZAR SEGUN TIPO DE ESTRUCTURA
t_stream* paquetizarStruct_nombreMensaje(t_struct_nombreMensaje* estructuraOrigen); //ESTRUCTURA DINAMICA
t_stream* paquetizarStruct_numero(t_struct_numero* estructuraOrigen);//ESTRUCTURA ESTATICA
t_stream* paquetizarStruct_char(t_struct_char* estructuraOrigen); //E
t_stream* paquetizarStruct_string(t_struct_string* estructuraOrigen); //D
t_stream* paquetizarStruct_signal(t_struct_signal* estructuraOrigen); //E
t_stream* paquetizarStruct_pcb(t_struct_pcb* estructuraOrigen);//E
t_stream* paquetizarStruct_pidycodigo(t_struct_pidycodigo* estructuraOrigen);//D
t_stream* paquetizarStruct_push(t_struct_push* estructuraOrigen);//E
t_stream* paquetizarStruct_pop(t_struct_pop* estructuraOrigen);
t_stream * paquetizarStruct_asignarCompartida(t_struct_asignar_compartida * estructuraOrigen);
t_stream * paquetizarStruct_instruccion(t_struct_instruccion * estructuraOrigen);
t_stream * paquetizarStruct_SF(t_struct_numero * estructuraOrigen);
t_stream * paquetizarStruct_PROGFIN(t_struct_numero * estructuraOrigen);
t_stream * paquetizarStruct_indiceEtiquetas(t_struct_indice_etiquetas * estructuraOrigen);
t_stream * paquetizarStruct_obtenerCompartida(t_struct_string * estructuraOrigen);
t_stream * paquetizarStruct_wait(t_struct_semaforo * estructuraOrigen);
t_stream * paquetizarStruct_signalSemaforo(t_struct_semaforo * estructuraOrigen);
t_stream * paquetizarStruct_io(t_struct_io * estructuraOrigen);
t_stream * paquetizarStruct_variables(t_struct_string * estructuraOrigen);

//FUNCIONES DE HEADER QUE SIRVEN PARA PAQUETIZAR
char* crearDataConHeader(uint8_t tipoEstructura, int length);
t_header crearHeader(uint8_t tipoEstructura, uint16_t lengthDatos);

//FUNCION PRINCIPAL DESPAQUETIZAR
void* despaquetizar(uint8_t tipoEstructura, char* dataPaquete, uint16_t length);
//FUNCION DESPAQUETIZAR SEGUN TIPO DE ESTRUCTURA
t_struct_nombreMensaje* despaquetizarStruct_nombreMensaje(char * dataPaquete, uint16_t length); //ESTRUCTURA DINAMICA
t_struct_numero* despaquetizarStruct_numero(char * dataPaquete, uint16_t length);//ESTRUCTURA ESTATICA
t_struct_char* despaquetizarStruct_char(char * dataPaquete, uint16_t length);
t_struct_string* despaquetizarStruct_string(char * dataPaquete, uint16_t length);
t_struct_signal* despaquetizarStruct_signal(char * dataPaquete, uint16_t length);
t_struct_pcb* despaquetizarStruct_pcb(char* dataPaquete, uint16_t length);
t_struct_pidycodigo* despaquetizarStruct_pidycodigo(char* dataPaquete, uint16_t length);
t_struct_push* despaquetizarStruct_push(char* dataPaquete, uint16_t length);
t_struct_pop* despaquetizarStruct_pop(char* dataPaquete, uint16_t length);
t_struct_asignar_compartida * despaquetizarStruct_asignarCompartida(char * dataPaquete, uint16_t length);
t_struct_instruccion * despaquetizarStruct_instruccion(char * dataPaquete, uint16_t length);
t_struct_numero * despaquetizarStruct_SF(char * dataPaquete, uint16_t length);
t_struct_numero * despaquetizarStruct_PROGFIN(char * dataPaquete, uint16_t length);
t_struct_indice_etiquetas* despaquetizarStruct_indiceEtiquetas(char * dataPaquete, uint16_t length);
t_struct_string * despaquetizarStruct_obtenerCompartida(char * dataPaquete, uint16_t length);
t_struct_semaforo * despaquetizarStruct_signalSemaforo(char * dataPaquete, uint16_t length);
t_struct_semaforo * despaquetizarStruct_wait(char * dataPaquete, uint16_t length);
t_struct_io * despaquetizarStruct_io(char * dataPaquete, uint16_t length);
t_struct_string * despaquetizarStruct_variables(char * dataPaquete, uint16_t length);
t_struct_numero * despaquetizarStruct_MO(char * dataPaquete, uint16_t length);



//FUNCIONES DE HEADER QUE SIRVEN PARA DESPAQUETIZAR
t_header despaquetizarHeader(char * header);

#endif /* PACKAGE_H_ */
