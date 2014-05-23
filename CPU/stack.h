/*
 * stack.h
 *
 *  Created on: 04/05/2014
 *      Author: utnso
 */

#ifndef STACK_H_
#define STACK_H_


#include "parser/parser.h"

typedef struct {
     t_valor_variable *elementos;
 	 int max_size;
  	 int top_index;
  	 int* stack_base;
  	 int* cursor_stack;
 } t_stack;

 t_stack* CREATE_STACK (int tamanoMax);

 void DESTROY_STACK (t_stack *pila);

 int IS_EMPTY (t_stack *pila);

 int IS_FULL (t_stack *pila, int pos);

 t_valor_variable TOP (t_stack *pila);

 t_valor_variable POP (t_stack *pila);

 t_valor_variable POP_DESREFERENCIAR (t_stack *pila, t_puntero posicionValor);

 void PUSH_POSITION (t_valor_variable* x, t_stack* pila,int pos);

 int PUSH_SIZE_CHECK (t_valor_variable* x, t_stack* pila, int pos);

 t_valor_variable POP_SIZE_CHECK(t_stack* pila);

 void IR_A_POSICON(t_stack* pila, int pos);

#endif /* STACK_H_ */
