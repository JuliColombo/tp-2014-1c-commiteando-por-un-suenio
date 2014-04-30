/*
 * Primitivas.h
 *
 *  Created on: 22/04/2014
 *      Author: utnso
 */

#ifndef PRIMITIVAS_H_
#define PRIMITIVAS_H_

#include "parser/metadata_program.h"
#include "parser/parser.h"

t_puntero definirVariable(t_nombre_variable identificador_variable );

t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable );

t_valor_variable dereferenciar(t_puntero direccion_variable);

void asignar(t_puntero direccion_variable, t_valor_variable valor);

t_valor_variable obtenerValorCompartida(t_nombre_compartida variable);

t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor);

t_puntero_instruccion irAlLabel(t_nombre_etiqueta etiqueta);

t_puntero_instruccion llamarSinRetorno(t_nombre_etiqueta etiqueta, t_puntero_instruccion linea_en_ejecuccion);

t_puntero_instruccion llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar, t_puntero_instruccion linea_en_ejecuccion);

t_puntero_instruccion finalizar(void);

t_puntero_instruccion retornar(t_valor_variable retorno);

int imprimir(t_valor_variable valor_mostrar);

int imprimirTexto(char* texto);

int entradaSalida(t_nombre_dispositivo dispositivo, int tiempo);

int wait(t_nombre_semaforo identificador_semaforo);

int signal(t_nombre_semaforo identificador_semaforo);


#endif /* PRIMITIVAS_H_ */
