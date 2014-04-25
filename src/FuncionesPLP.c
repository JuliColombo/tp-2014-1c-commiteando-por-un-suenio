#include "FuncionesPLP.h"
#include "UMV.h"


void calcularPeso (t_programa programa){ //Calcula peso del programa
	programa.peso=(5*programa.metadata.cantidad_de_etiquetas + 3* programa.metadata.cantidad_de_funciones + programa.metadata.instrucciones_size);
}
void ordenarPorPeso (int p[], int n){//Ordena la cola de new según el peso (no sé si ponerle que la cola ya están los pesos o para ordenarlos hay que calcular y ordenar en el momento)
	int i;
	int max=p[0];

	for(i=0;i<n;i++){
		if (max < p[i]) {
		max = p[i];
		}
	}
}


int main_plp(){
	t_list* cola_programas=list_create();

	//Cuando entra un programa nuevo
	t_programa programa;
	char* literal="Abc"; //hay que ver como hacer para sacar un literal con el código
	metadatada_desde_literal(literal);
	solicitarMemoria(programa);
	calcularPeso(programa);





	return 0;
}
