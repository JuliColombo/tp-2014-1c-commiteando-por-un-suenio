/*
 * Primitivas.c
 *
 *  Created on: 16/04/2014
 *      Author: utnso
 */

#include <stdio.h>
#include <string.h>



signed int* definirVariable(t_nombre_variable identificador_variable){
	identificador_variable = malloc(sizeof(signed int));
	if(identificador_variable!=NULL){
		return identificador_variable;
	}
	else{
		printf("No se puede reservar memoria");
		return NULL;
	}
}

signed int* obtenerPosicionVariable(t_nombre_variable identificador_variable){
	return 0;
}

signed int desreferenciar(signed int* direccion_variable){  //extrae el valor de la variable ubicada en direccion_variable
	signed int aux;
	aux=*direccion_variable;
	return aux;
}

void asignar(signed int* direccion_variable, signed int valor){ //copia el valor en la variable ubicada en direccion_variable
	direccion_variable=&valor;
}

signed int obtenerValorCompartida(t_nombre_compartida variable){ //deberia solicitar el valor de la variable al kernel
	return 0;
}

signed int asignarValorCompartida(t_nombre_compartida variable, signed int valor){ //deberia solicitar al kernel que asigne valor a la variable
	return 0;
}

signed int irAlLabel(t_nombre_etiqueta etiqueta){
	int pos;
	//Se conecta con la PCB y accede al indice de etiquetas con indiceEtiquetas[etiqueta]
	if (noHayError!){
		return etiqueta;
	}else return -1;
}

//En este cuando dice "Preserva el contexto" no entiendo bien como,por lo que entendi no deberia hacer nada con eso
signed int llamarSinRetorno(t_nombre_etiqueta etiqueta,t_puntero_instruccion linea_en_ejecucion){//Almacenar direccion para almacenar retorno de procedimiento actual, cursor de nuevo Contexto Actual,Program Counter + 1,
	return (programCounter);
}

signed int llamarConRetorno(t_nombre_etiqueta etiqueta,t_puntero donde_retornar,t_puntero_instruccion linea_en_ejecucion){//Igual que el anterior solo que guardando la posicion donde se va a almacenar la variable que retorne la funcion
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


