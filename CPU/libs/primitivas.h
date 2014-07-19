/*
 * primitivas.h
 *
 *  Created on: 23/05/2014
 *      Author: utnso
 */

#ifndef PRIMITIVAS_H_
#define PRIMITIVAS_H_

#include "primitivasAux.h"
#include "log.h"

t_puntero definirVariable(t_nombre_variable identificador_variable);

t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable);

t_valor_variable dereferenciar(t_puntero direccion_variable);

void asignar(t_puntero direccion_variable, t_valor_variable valor);

t_valor_variable obtenerValorCompartida(t_nombre_compartida variable);

t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor);

void irAlLabel(t_nombre_etiqueta etiqueta);

void llamarSinRetorno(t_nombre_etiqueta etiqueta);

void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar);

void finalizar();

void retornar();

void imprimir(t_valor_variable valor_mostrar);

void imprimirTexto(char* texto);

void entradaSalida(t_nombre_dispositivo dispositivo, int tiempo);

void signal_ansisop(t_nombre_semaforo identificador_semaforo);

void wait_ansisop(t_nombre_semaforo identificador_semaforo);


#endif /* PRIMITIVAS_H_ */
