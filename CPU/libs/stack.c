/*
 * stack.c
 *
 *  Created on: 04/05/2014
 *      Author: utnso
 */

#include "stack.h"
#include <stdlib.h>

 t_stack* CREATE_STACK (int tamanoMax)
  {
   t_stack* pila;

   pila = (t_stack*) malloc(sizeof(t_stack));
   pila->max_size = tamanoMax;
   pila->top_index = -1;
   pila->elementos = (t_valor_variable *) malloc(tamanoMax-sizeof(t_stack));
  // pila->stack_base = pila->elementos;
  // pila->cursor_stack = pila->elementos;
   return pila;
  }

 void DESTROY_STACK (t_stack *pila)
  {
   free(pila->elementos);
   free(pila);
   //*P = NULL;
  }


 int IS_EMPTY (t_stack *pila)
  {
   return(pila->top_index == -1);
  }

 int IS_FULL (t_stack *pila, int pos) {
	return(pos==(pila->max_size-1));
 }

 t_valor_variable TOP (t_stack *pila)
  {
   return(pila->elementos[pila->top_index]);
  }


 t_valor_variable POP (t_stack *pila)
 {t_valor_variable top=pila->elementos[pila->top_index];
   pila->top_index--;
   return top;
  }

 t_valor_variable POP_DESREFERENCIAR (t_stack *pila, t_puntero posicionValor) {
	 if(IS_EMPTY(pila)) {
	 		return STACK_UNDERFLOW;
	 } else {
	 		int top = posicionValor;
	 		return pila->elementos[top+1];}
 }

 t_valor_variable POP_RETORNAR (t_stack *pila, t_puntero posicionValor) {
 	 if(IS_EMPTY(pila)) {
 	 		return STACK_UNDERFLOW;
 	 } else {
 	 		int top = posicionValor;
 	 		return pila->elementos[top-1];}
  }


 void PUSH_POSITION (t_valor_variable* x, t_stack* pila,int pos) {
	 pila->top_index = pos;
	 pila->elementos[pos] = *x;
 }

int PUSH_SIZE_CHECK(t_valor_variable*x , t_stack* pila, int pos) {
	if(IS_FULL(pila, pos))
		return STACK_OVERFLOW;
	else {
		PUSH_POSITION(x,pila,pos);
		return SIN_ERROR;
	}
}

t_valor_variable POP_SIZE_CHECK(t_stack* pila) {
	if(IS_EMPTY(pila))
		return STACK_UNDERFLOW;
	else {
		return POP(pila);
	}
}

/*void IR_A_POSICION(t_stack* pila,int pos) {
	pila->elementos[pos];
}*/
