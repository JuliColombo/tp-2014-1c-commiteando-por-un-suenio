/*
 * Primitivas.c
 *
 *  Created on: 16/04/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <string.h>
#include "parser/parser.h"


/*definirVariable: Creo que esta mal pensado esto, no se le debría asignar al identificador de la variable la dirección de memoria sino devolver otra variable que sea de tipo puntero
t_puntero definirVariable(t_nombre_variable identificador_variable){
	identificador_variable = malloc(sizeof(signed int));
	if(identificador_variable!=NULL){
		return identificador_variable;
	}
	else{
		printf("No se puede reservar memoria");
		return (t_puntero)NULL;
	}
}*/


t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable){
	return 0;
}

t_valor_variable desreferenciar(t_puntero direccion_variable){  //extrae el valor de la variable ubicada en direccion_variable
	t_valor_variable aux=*((int*)direccion_variable);
	return aux;
}

void asignar(t_puntero direccion_variable, t_valor_variable valor){ //copia el valor en la variable ubicada en direccion_variable
	direccion_variable=(t_puntero)&valor;
}

t_valor_variable obtenerValorCompartida(t_nombre_compartida variable){ //deberia solicitar el valor de la variable al kernel
	return 0;
}

t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor){ //deberia solicitar al kernel que asigne valor a la variable
	return 0;
}

t_puntero_instruccion irAlLabel(t_nombre_etiqueta etiqueta){
	//int pos;
	//Se conecta con la PCB y accede al indice de etiquetas con indiceEtiquetas[etiqueta]
	//if (noHayError!){
	if (1){
		return 0;
	}else return -1;
}

//En este cuando dice "Preserva el contexto" no entiendo bien como,por lo que entendi no deberia hacer nada con eso
t_puntero_instruccion llamarSinRetorno(t_nombre_etiqueta etiqueta,t_puntero_instruccion linea_en_ejecucion){//Almacenar direccion para almacenar retorno de procedimiento actual, cursor de nuevo Contexto Actual,Program Counter + 1,
	//return (programCounter);
	return 0;
}

t_puntero_instruccion llamarConRetorno(t_nombre_etiqueta etiqueta,t_puntero donde_retornar,t_puntero_instruccion linea_en_ejecucion){//Igual que el anterior solo que guardando la posicion donde se va a almacenar la variable que retorne la funcion
	return 0;
}

//Aca hice la funcion contarDigitos de valor_mostrar de imprimir. Imprimir le manda al kernel el valor y a su vez devuelve la cantidad de digitos de este
int contarDigitos(int numero) {
	int contador=0;
	while(numero != 0) {
		contador++;
		numero=numero/10;}
	return contador;
}

int imprimir(t_valor_variable valor_mostrar) {
	//enviarAKernel(valor_mostrar);
	return contarDigitos(valor_mostrar);
}

int imprimirTexto(char* texto) {
	//enviarAKernel(texto);
	return strlen(texto);
}

int entradaSalida(t_nombre_dispositivo dispositivo, int tiempo){
	return 0;
}

int wait(t_nombre_semaforo identificador_semaforo){
	return 0;
}

int signal(t_nombre_semaforo identificador_semaforo){
	return 0;
}
