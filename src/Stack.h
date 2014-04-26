/*
 * Stack.h
 *
 *  Created on: 26/04/2014
 *      Author: utnso
 */

#ifndef STACK_H_
#define STACK_H_

typedef char datatype;

#define SUCCESS 0
#define FAIL -1

typedef struct {
	datatype* pila;
	int max_size;
	int top_index;
} STACK;

int create_stack(STACK *s,int size);

int destroy_stack(STACK *s);

int push(STACK *s, datatype* element);

int pop(STACK *s, datatype* ptr);


#endif /* STACK_H_ */
