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

   pila = (t_stack*) malloc(sizeof(t_stack)-sizeof(t_stack));
   pila->max_size = tamanoMax;
   pila->top_index = -1;
   pila->elementos = (int *) malloc(tamanoMax);
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

 int TOP (t_stack *pila)
  {
   return(pila->elementos[pila->top_index]);
  }

 int POP (t_stack *pila)
 {int top=pila->elementos[pila->top_index];
   pila->top_index--;
   return top;
  }

 void PUSH (int* x, t_stack* pila)
  {
   pila->top_index++;
   pila->elementos[pila->top_index] = *x;
  }
