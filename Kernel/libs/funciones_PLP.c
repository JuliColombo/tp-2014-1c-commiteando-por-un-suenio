#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "funciones_PLP.h"
#include "estructuras_kernel.h"
#include "parser/metadata_program.h"
#include "commons/config.h"
#include "commons/collections/list.h"
#include "log.h"


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


void inicializarConfiguracion(char* PATH) {
	archLog = log_crear(PATHLOG);
	struct stat file_info;
	int control = lstat(PATH, &file_info);
	if (control == -1) {
		log_escribir(archLog, "Leer archivo de configuracion", ERROR, "El archivo no existe");
		}
	else{
	leerConfiguracion(PATH);
	imprimirConfiguracion(configuracion_kernel); //Imprime las configuraciones actuales por pantalla
	}
}

void leerConfiguracion(char* PATH) {
	t_config* config=config_create(PATH);

	configuracion_kernel.puerto_programas = config_get_int_value(config,"Puerto TCP para recibir conexiones de los Programas");
	configuracion_kernel.puerto_cpus = config_get_int_value(config,"Puerto TCP para recibir conexiones de los CPUs");
	configuracion_kernel.quantum = config_get_int_value(config,"Quantum de tiempo para algoritmos expropiativos");
	configuracion_kernel.retardo_quantum = config_get_int_value(config,"Retardo del Quantum");
	configuracion_kernel.multiprogramacion = config_get_int_value(config,"Maximo nivel de multiprogramacion");
	//configuracion_kernel.id_semaforos = config_get_array_value(config,"Lista de nombres de Semaforos");
	//configuracion_kernel.valor_semaforos = config_get_array_value(config,"Lista de valores de Semaforos");
	//configuracion_kernel.retardo_hio = config_get_array_value(config,"Retardo de hio");
	//configuracion_kernel.id_hio = config_get_array_value(config,"Lista de hio");
	configuracion_kernel.uvm_ip = config_get_int_value(config,"Direccion IP para conectarse a la UMV");
	configuracion_kernel.umv_puerto = config_get_int_value(config,"Puerto TCP para conectarse a la UMV");
	//configuracion_kernel.var_globales = config_get_array(config,"Variables globales");
}

void imprimirConfiguracion(t_config_kernel configuracion) { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("%d\n", configuracion.puerto_programas);
	printf("%d\n", configuracion.puerto_cpus);
	printf("%d\n", configuracion.quantum);
	printf("%d\n", configuracion.retardo_quantum);
	printf("%d\n", configuracion.multiprogramacion);
	//printf("%d\n", configuracion.id_semaforos);
	//printf("%d\n", configuracion.semaforos);
	//printf("%d\n", configuracion.retardo_hio);
	//printf("%d\n", configuracion.id_hio);
	printf("%d\n", configuracion.uvm_ip);
	printf("%d\n", configuracion.umv_puerto);
	//printf("%d\n", configuracion.var_globales);
}
