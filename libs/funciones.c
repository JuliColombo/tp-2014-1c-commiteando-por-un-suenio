/*
 * funciones.c
 *
 *  Created on: 12/04/2014
 *      Author: utnso
 */


int peso (int cantEtiquetas, int cantFunc, int cantLineas){
	return (5*cantEtiquetas + 3* cantFunc + cantLineas);
}
void ordenarPorPeso (int p[], int n){
	int i;
	int max=p[0];

	for(i=0;i<n;i++){
		if (max < p[i]) {
		max = p[i];
		}
	}
}


