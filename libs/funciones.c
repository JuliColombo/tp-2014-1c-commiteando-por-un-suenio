/*
 * funciones.c
 *
 *  Created on: 12/04/2014
 *      Author: utnso
 */

#include "funciones.h"


int peso (int cantEtiquetas, int cantFunc, int cantLineas){ //Calcula peso del programa
	return (5*cantEtiquetas + 3* cantFunc + cantLineas);
}
void ordenarPorPeso (int p[], int n){      //Ordena la cola de new según el peso (no sé si ponerle que la cola ya están los pesos o para ordenarlos hay que calcular y ordenar en el momento)
	int i;
	int max=p[0];

	for(i=0;i<n;i++){
		if (max < p[i]) {
		max = p[i];
		}
	}
}

typedef struct { // se define un vector para los programas en new, y cuando el programa pasa de new a ready le asignamos el peso 0 (nunca van a tener peso 0)
	int peso;
	int cantEtiquetas;
	int cantFunc;
	int cantLineas;

} programa;


//No creo que vaya aca, pero voy poniendo una idea de PLP:

//Tenemos una estructura con programas que se conectan o la funcion va a tener una llamada cada vez que entra uno nuevo?? lo modélo pensando lo ultimo

int plp(programa){

	int crearPcb(programa);
	//Cuando creamos el PCB hay que aprovechar la obtencion de los datos(codigo literal, indice de etiquetas,indice de funciones, indice de codigo y stack) para conseguir los tamaños de este ""malloc"" de umv
	int solicitarMemoria(codigo,indiceEtiquetas,indiceFunciones,indiceCodigoYStack);
	//Ni idea los tipos de esos parametros, no me da mas la nafta

	int agregarACola(programa);

}
