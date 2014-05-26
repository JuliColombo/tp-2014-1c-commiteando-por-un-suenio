#include "funciones_PLP.h"



void calcularPeso (t_programa programa){ //Calcula peso del programa
	programa.peso=(5*programa.metadata.cantidad_de_etiquetas + 3* programa.metadata.cantidad_de_funciones + programa.metadata.instrucciones_size);
}

void agregarAColaSegunPeso(t_programa programa, t_list* lista){
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
	if(lista==NULL){
		return;
	}
	printf("Estado de la cola:\n");
	for (i=0;i<(list_size(lista));i++){
		t_programa* aux=(t_programa*)list_get(lista, i);
		p=aux->peso;
		printf(" Nodo %d   Peso %d\n", i, p); //Tal vez agregar el nombre del programa(? Preguntar
	}
}

int cantidadProgramasEnEjecucion(void){
	return (list_size(cola.ready)+list_size(cola.exec)+list_size(cola.block));
}

void completarGradoMultip(void){
	void* aux=list_remove(cola.new,0);

	while(cantidadProgramasEnEjecucion()<configuracion_kernel.multiprogramacion && aux!=NULL){
		list_add(cola.ready,aux);
		aux=list_remove(cola.new,0);
	}

}

void inicializarColas(void){
	cola.new=list_create();
	cola.ready=list_create();
	cola.exec=list_create();
	cola.block=list_create();
	cola.exit=list_create();
}

/*int main_plp(){
	t_list* cola_programas=list_create();
	printf("La cola está vacia");

	//Cuando entra un programa nuevo
		t_programa programa;
		calcularPeso(programa);
		//programa.metadata=*(metadatada_desde_literal(literal));
		if(solicitarMemoria(programa)){
			agregarAListaSegunPeso(programa,cola_programas);
			}
			else{
				printf("No hay memoria suficiente para agregar el programa");
			}
		mostrarNodosPorPantalla(cola_programas);


	return 0;
}*/

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


void inicializarConfiguracion(void){
	archLog = log_crear(PATHLOG);
	struct stat file_info;
	int control = lstat(PATH, &file_info);
	if (control == -1){
		log_escribir(archLog, "Leer archivo de configuracion", ERROR, "El archivo no existe");
		}
	else{
	leerConfiguracion();
	imprimirConfiguracion(); //Imprime las configuraciones actuales por pantalla
	}
}

void leerConfiguracion(void){
	t_config* config=config_create(PATH);

	configuracion_kernel.puerto_programas = config_get_int_value(config,"Puerto TCP para recibir conexiones de los Programas");
	configuracion_kernel.puerto_cpus = config_get_int_value(config,"Puerto TCP para recibir conexiones de los CPUs");
	configuracion_kernel.quantum = config_get_int_value(config,"Quantum de tiempo para algoritmos expropiativos");
	configuracion_kernel.retardo_quantum = config_get_int_value(config,"Retardo del Quantum");
	configuracion_kernel.multiprogramacion = config_get_int_value(config,"Maximo nivel de multiprogramacion");
	configuracion_kernel.id_semaforos = config_get_array_value(config,"Lista de nombres de Semaforos");
	configuracion_kernel.valor_semaforos=vector_num(config_get_array_value(config,"Lista de valores de Semaforos"),configuracion_kernel.id_semaforos);
	configuracion_kernel.id_hio = config_get_array_value(config,"Lista de hio");
	configuracion_kernel.retardo_hio = vector_num(config_get_array_value(config,"Retardo de hio"),configuracion_kernel.id_hio);
	configuracion_kernel.ip_umv = config_get_int_value(config,"Direccion IP para conectarse a la UMV");
	configuracion_kernel.puerto_umv = config_get_int_value(config,"Puerto TCP para conectarse a la UMV");
	configuracion_kernel.var_globales = config_get_array_value(config,"Variables globales");

	}

void imprimirConfiguracion() { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("Puerto programas: %d\n", configuracion_kernel.puerto_programas);
	printf("Puerto CPUs: %d\n", configuracion_kernel.puerto_cpus);
	printf("Quantum: %d\n", configuracion_kernel.quantum);
	printf("Retardo quantum: %d\n", configuracion_kernel.retardo_quantum);
	printf("Grado de multiprogramacion: %d\n", configuracion_kernel.multiprogramacion);


	int i;
	for(i=0;i<cant_identificadores(configuracion_kernel.id_semaforos);i++){
		printf("Semaforo (valor): %s (%d)\n",configuracion_kernel.id_semaforos[i], configuracion_kernel.valor_semaforos[i]);
	}

	free(configuracion_kernel.valor_semaforos);
	for(i=0;configuracion_kernel.id_hio[i]!=NULL;i++){
		printf("ID HIO (retardo): %s ", configuracion_kernel.id_hio[i]);
		printf("(%d)\n", configuracion_kernel.retardo_hio[i]);
	}
	free(configuracion_kernel.retardo_hio);
	printf("IP de la UMV: %d\n", configuracion_kernel.ip_umv);
	printf("Puerto UMV: %d\n", configuracion_kernel.puerto_umv);
	
	printf("Variables globales: ");
	for(i=0;configuracion_kernel.var_globales[i]!=NULL;i++){
		printf("%s ", configuracion_kernel.var_globales[i]);
	}
	printf("\n");

}


pthread_t plp_conexiones;

void* core_plp(void){

	int thread_plp_conexiones = pthread_create (&plp_conexiones, NULL, core_plp_conexiones(), NULL);
	t_programa* programa = list_remove(cola.new,0);
	/*int flag_comienzo=0;
	int flag_hio=0;*/



	while(1){

		/*flag_comienzo=0;
		flag_terminado=0;
		flag_hio=0;
		quantum=0;*/





		while(programa->quantum < configuracion_kernel.quantum && programa->flag_terminado==0){


		if(programa->flag_terminado==1){
			completarGradoMultip();
		}

		}
	}
	//Logica del PLP

	pthread_join(thread_plp_conexiones, NULL);

	return NULL;
}


void* core_pcp(void){
	return 0;
}


void* core_io(int retardo){

	return 0;
}


void* core_plp_conexiones(void){
	int sock_plp;		//El socket de conexion
	int n_sock_plp;		//El socket de datos
	t_nipc* paquete;	//El paquete que recibe el socket

	if ((sock_plp = nipc_abrirConexion(configuracion_kernel.puerto_programas))<0){
		if (close(sock_plp)<0){
			//Error con el close
		}
		abort();
	}//El socket esta creado y listo para escuchar a los clientes por el puerto_programas

	while(1){
		printf("Esperando conexion...\n");
		n_sock_plp = nipc_aceptarConexion(sock_plp);
		memset(paquete, 0, sizeof(paquete)); // Hay que inicializar paquete en algún lado! //Aca lo estamos inicializando, el memset le pone todos 0s a la variable)
		if (nipc_recibir(n_sock_plp,paquete)<0){
			//No se recibieron datos
		} else {
			//Se recibieron datos
		}
		break; //Esto va a hacer que salga del bucle y solo se corra una vez, despues hay que sacarlo
	}
	//Esto nunca se ejecutaria, al salir del bucle deberia terminar el proceso. Esta para que el eclipse no se queje
	if (close(sock_plp)<0){
		//Error con el close
	}

	return 0;
}
