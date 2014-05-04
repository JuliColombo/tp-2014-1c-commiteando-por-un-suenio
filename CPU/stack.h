/*
 * stack.h
 *
 *  Created on: 04/05/2014
 *      Author: utnso
 */

#ifndef STACK_H_
#define STACK_H_

typedef struct {
     int *elementos;
 	 int max_size;
  	 int top_index;
 } t_stack;

 t_stack* CREATE_STACK (int tamanoMax);

 void DESTROY_STACK (t_stack *pila);

 int IS_EMPTY (t_stack *pila);

 int TOP (t_stack *pila);

 int POP (t_stack *pila);

 void PUSH (int* x, t_stack* pila);

#endif /* STACK_H_ */
