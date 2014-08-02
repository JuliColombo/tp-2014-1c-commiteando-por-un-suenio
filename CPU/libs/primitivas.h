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


extern int sockKernel; // sockets globales para ser usados en las primitivas
extern int sockUMV;    // sockets globales para ser usados en las primitivas
extern uint32_t var_seg_stack;
extern uint32_t var_tamanio_contexto;
extern uint32_t temp_estado;
extern t_dictionary * dicc_variables;
extern char * dicc_etiquetas;
extern uint32_t var_ind_etiquetas;
extern uint32_t var_tamanio_etiquetas;
extern uint32_t temp_counter;
extern uint32_t temp_cursor_stack;
extern uint32_t temp_cursor_ant;
extern uint32_t temp_id;
extern int SEG_flag;
extern uint32_t temp_ind_codigo;
extern uint32_t temp_seg_codigo;
extern int fin_quantum;
int fin_quatum_ansisop;
extern t_quantum quantum;
extern int fin_PCB;
extern log_t * archLog;
extern t_dictionary* dicc_variables;


void excepcion_UMV(int i);

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

t_struct_pcb * PCB_Actualizado(void) ;

#endif /* PRIMITIVAS_H_ */
