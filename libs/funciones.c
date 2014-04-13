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
