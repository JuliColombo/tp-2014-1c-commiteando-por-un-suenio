#include "FuncionesPLP.h"
#include "UMV.h"

void calcularPeso (t_programa programa){ //Calcula peso del programa
	programa.peso=(5*programa.metadata.cantidad_de_etiquetas + 3* programa.metadata.cantidad_de_funciones + programa.metadata.instrucciones_size);
}

void agregarAListaSegunPeso(t_programa programa, t_list* lista){
	t_link_element *element = lista->head;
	int position = 0;

	while ((element != NULL)&&(((t_programa*)(element->data))->peso < programa.peso) ) {
		element = element->next;
		position++;
	}

	list_add_in_index(lista, position, &programa);
}

void mostrarNodosPorPantalla(t_list* lista){
int i;
int p;
	system("clear");
	printf("Estado de la cola:\n");
	for (i=0;i<(list_size(lista));i++){
		t_programa* aux=(t_programa*)list_get(lista, i);
		p=aux->peso;
		printf(" Nodo %d   Peso %d\n", i, p); //Tal vez agregar el nombre del programa(? Preguntar
	}
}

int main_plp(){
	t_list* cola_programas=list_create();
	printf("La cola está vacia");

	//Cuando entra un programa nuevo
		t_programa programa;
		calcularPeso(programa);
		char* literal="Abc"; //hay que ver como hacer para sacar un literal con el código
		programa.metadata=*(metadatada_desde_literal(literal));
		if(solicitarMemoria(programa)){
			agregarAListaSegunPeso(programa,cola_programas);
			}
			else{
				printf("No hay memoria suficiente para agregar el programa");
			}
		mostrarNodosPorPantalla(cola_programas);


	return 0;
}

/*test de main_plp
int main(){
	t_list* cola_programas=list_create();
	printf("La cola está vacia\n");

		t_programa programa0,programa1,programa2,programa3;
		programa0.peso=2;
		programa1.peso=programa3.peso=7;
		programa2.peso = 10;
		t_programa v[4]={programa0,programa1,programa2,programa3};
		//calcularPeso(programa);
		//char* literal="a=b+1"; //hay que ver como hacer para sacar un literal con el código
		int i;
		for (i=0;i<4;i++){
			//v[i].metadata=*(metadatada_desde_literal(literal));
			if(solicitarMemoria(v[i])){
				agregarAListaSegunPeso(v[i],cola_programas);
				}
				else{
					printf("No hay memoria suficiente para agregar el programa");
				}
			mostrarNodosPorPantalla(cola_programas);
			};

	return 0;
}
*/
