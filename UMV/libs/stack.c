/*
 * Stack.c
 *
 *  Created on: 26/04/2014
 *      Author: utnso
 */

#include "parser/parser.h"
#include "stack.h"

int create_stack(STACK *s,int size) {
	s->pila = (datatype*) malloc(size*sizeof(datatype));
	if (s->pila == NULL) {
		return FAIL;
	}
	s->max_size = size;
	s->top_index = -1;
	return SUCCESS;
}

int destroy_stack(STACK *s) {
	free(s->pila);
	s->max_size = -1;
	return SUCCESS;
}

int push(STACK *s, datatype* element) {
	if(s->top_index == (s->max_size-1)) {
		return (int)"STACK_OVERFLOW";
	}
	s->top_index++;
	s->pila[s->top_index] = *element;
	return SUCCESS;
}

int pop(STACK *s, datatype* ptr) {
	if(s->top_index==-1){
		return (int)"STACK_UNDERFLOW";
	}
	*ptr = s->pila[s->top_index];
	s->top_index--;
	return SUCCESS;
}

