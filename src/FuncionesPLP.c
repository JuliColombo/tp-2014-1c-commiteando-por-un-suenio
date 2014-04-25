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

void agregarAListaSegunPeso(t_programa programa, t_list* lista){
	//t_link_element *element = lista->head;
	//int position = 0;

	//while ((element != NULL)&&(element->data->peso < programa.peso) ) {
		//element = element->next;
		//position++;
	//}

	if (lista->elements_count==0) {
		list_add(lista, &programa);
	}


	//list_add_in_index(lista, &list_find(lista, index, programa);


}


int main_plp(){
	t_list* cola_programas=list_create();
	printf("La cola está vacia");

	//Cuando entra un programa nuevo
	t_programa programa;
	calcularPeso(programa);
	char* literal="Abc"; //hay que ver como hacer para sacar un literal con el código
	metadatada_desde_literal(literal);
	if(solicitarMemoria(programa)){
		agregarAListaSegunPeso(programa,cola_programas);
		}
		else{
			printf("No hay memoria suficiente para agregar el programa");
	}






	return 0;
}
