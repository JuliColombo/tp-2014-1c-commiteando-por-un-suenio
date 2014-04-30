#include <stdio.h>
#include <stdlib.h>
#include "funciones_PLP.h"
#include "parser/metadata_program.h"
#include "commons/config.h"
#include "estructuras_kernel.h"


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
	//system("clear");
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
		//programa.metadata=*(metadatada_desde_literal(literal));
		if(1 /*solicitarMemoria(programa)*/){
			agregarAListaSegunPeso(programa,cola_programas);
			}
			else{
				printf("No hay memoria suficiente para agregar el programa");
			}
		mostrarNodosPorPantalla(cola_programas);


	return 0;
}

//Tenemos una estructura con programas que se conectan o la funcion va a tener una llamada cada vez que entra uno nuevo?? lo modélo pensando lo ultimo

/* int plp(programa){
//Faltaria tambien inicializar el segmento de Stack y el puntero de Contexto Actual(tienen que coincidir), y setear el contador de variables locales en 0
	int crearPcb(programa);
	//Cuando creamos el PCB hay que aprovechar la obtencion de los datos(codigo literal, indice de etiquetas,indice de funciones, indice de codigo y stack) para conseguir los tamaños de este ""malloc"" de umv
	int solicitarMemoria(codigo,indiceEtiquetas,indiceFunciones,indiceCodigoYStack);
	//Ni idea los tipos de esos parametros, no me da mas la nafta

	int agregarACola(programa);

	return 0;

}
*/


/*Aca intente hacer el crearPcb. La consigna dice "PLP creara PCB y usara la funcionalidad del parser, que
 * recibira todo el codigo del script y devolvera una estrucutra con la info del programa, que contiene:
 * primera instruc, indice etiquetas, indice de codigo etc"
 *

int crearPcb(int primeraInstruc,Pares indiceCod, int tamanioIndEti, int tamanioIndCod, int tamanioContext, int PC) {
	PCB nuevoPcb;
	nuevoPcb->ID = getpid();
	nuevoPcb->indiceEtiquetas = dictionary_create();
	nuevoPcb->indiceCodigo = indiceCod;
	nuevoPcb->TamanioContext = tamanioContext;
	nuevoPcb->ProgramCounter = PCB;
	//No se que hacer con CODE, STACK y CursorSTACK
	//Creo que aca podria venir el solicitarMemoria. No use el tamanioIndEti ni tamanioIndCod que son cosas que
	//me da el parser supuestamente, y podrian servir para solicitar la memoria. No se

	return 0;
	}
*/

void leerConfiguracion(archivo_config_kernel configuracion) {
	t_config* config = config_create("/home/utnso/workspace/tp-2014-1c-commiteando-por-un-suenio/src");

	configuracion.puerto_programa = config_get_int_value(config,"Puerto TCP para recibir conexiones de Procesos InterpretesCPU");
	configuracion.puerto_cpu = config_get_int_value(config,"Puerto TCP para recibir conexiones de Procesos Interpretes");
	configuracion.quantum = config_get_int_value(config,"Quantum de tiempo para algoritmos expropiativos");
	configuracion.retardo_quantum = config_get_int_value(config,"Retardo del Quantum");
	configuracion.grado_mp = config_get_int_value(config,"Maximo nivel de multiprogramacion");
	//configuracion.id_semaforos = config_get_array_value(config,"Lista de nombres de Semaforos");
	//configuracion.semaforos = config_get_array_value(config,"Lista de valores de Semaforos");
	//configuracion.retardo_hio = config_get_array_value(config,"Retardo de hio");
	//configuracion.id_hio = config_get_array_value(config,"Lista de hio");
	configuracion.tamanio_mp = config_get_int_value(config,	"Tamanio de Memoria Principal");
}

//Esta bien que esto sea una funcion de la UMV?
void imprimirConfiguracion(archivo_config_kernel configuracion) { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("%d\n", configuracion.puerto_programa);
	printf("%d\n", configuracion.puerto_cpu);
	printf("%d\n", configuracion.quantum);
	printf("%d\n", configuracion.retardo_quantum);
	printf("%d\n", configuracion.grado_mp);
	//printf("%d\n", configuracion.id_semaforos);
	//printf("%d\n", configuracion.semaforos);
	//printf("%d\n", configuracion.retardo_hio);
	//printf("%d\n", configuracion.id_hio);
	printf("%d\n", configuracion.tamanio_mp);
}
