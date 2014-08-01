/*
 * UMV.c
 *
 *  Created on: 21/04/2014
 *      Author: utnso
 */


#include "funciones_UMV.h"





int estaEnDicOP(char palabra[]){
	char *dic_op[]={"operacion","dump","compactacion","retardo","algoritmo","exit"};
	int aux;
	for(aux=0;aux<6;aux++){
		if(strcmp(dic_op[aux], palabra) == 0){
			return 1;
		}

	}
	return 0;
}

int estaEnDicTOP(char palabra[]){
	char *dic_top[]={"solicitar","escribir","crear","destruir"};
	int aux;
	for(aux=0;aux<4;aux++){
		if((strcmp(dic_top[aux], palabra)) == 0){
			return 1;
		}
	}
	return 0;
}


void* crearMP(void) { // Cambie para que no reciba parametro, total la config es una variable externa -- Fede

	tamanioMP = configuracion_UMV.memSize;
	void* aux_MP;
	aux_MP = malloc(tamanioMP);
	if(aux_MP==NULL){
		log_escribir(archLog, "Error en tamaño de memoria principal", ERROR, "No hay memoria suficiente");
		abort();
	}
	return aux_MP;
}

void log_error_socket(void){
	pthread_mutex_lock(mutex_log);
	log_escribir(archLog, "Abrir conexion", ERROR, "No se pudo abrir la conexion");
	pthread_mutex_unlock(mutex_log);
}



// ***********************************Solicitar bytes en memoria*******************

t_struct_respuesta_umv solicitarBytes(int base,int offset, int longitud){
	pthread_mutex_lock(&Sem_DevuelveBytes);

		t_struct_respuesta_umv respuesta;
		Segmento segmento = BuscarSegmentoIndice(base);
		log_escribir(archLog,"Se busca el segmento que contiene informacion a enviar",INFO,"");
		int max_direccion_mem_segmento = segmento.tamano + segmento.baseVirtual;

		if(max_direccion_mem_segmento < segmento.baseVirtual + offset + longitud){
			printf("Violacion de Segmento. Memoria no accesible por este segmento\n");
			log_escribir(archLog,"Segmentation Fault",ERROR,"Memoria no accesible por este segmento.");
			void*buff_fault=malloc(sizeof(int));
			int valor=-30;
			memcpy(buff_fault,&valor,sizeof(int));
			respuesta.buffer=buff_fault;
			respuesta.tamano_buffer=sizeof(int);
			pthread_mutex_unlock(&Sem_DevuelveBytes);
			return respuesta ;
		}

		void * buffer = malloc(longitud);
		memcpy(buffer,(MP + segmento.baseVirtual + offset), longitud);
		respuesta.buffer = buffer;
		respuesta.tamano_buffer = longitud;
		log_escribir(archLog,"Se realiza una solicitud de bytes",INFO,"La solicitud tiene exito");

		pthread_mutex_unlock(&Sem_DevuelveBytes);

		return respuesta;
}


//****************************************enviarBytes*************************************


int enviarBytes(int base,int offset,int longitud,t_buffer buffer){
	pthread_mutex_lock(&Sem_GrabaBytes);
		int result = 0;
		Segmento segmento = BuscarSegmentoIndice(base);

		if(segmento.programa == -1){
			printf("No existe un segmento con este id:%d",base);
			log_escribir(archLog,"Se hace un envio de bytes",ERROR,"No existe un segmento con este id: %d",segmento.programa);
			return -1;
		}

		log_escribir(archLog,"Se busca el segmento de base para grabar",INFO,"Base: %d", segmento.baseVirtual);

		int max_direccion_mem_segmento = segmento.tamano + segmento.baseVirtual;

		if(max_direccion_mem_segmento < segmento.baseVirtual + offset + longitud){
			printf("Violacion de Segmento. Memoria no accesible por este segmento\n");
			log_escribir(archLog,"Violacion de Segmento",ERROR,"Se quiere grabar en la pos %d, y la maxima direccion del segmento es: %d", max_direccion_mem_segmento,segmento.baseVirtual + offset + longitud);
		}

		else{
			memcpy((MP + segmento.baseVirtual + offset),buffer,longitud);
			log_escribir(archLog,"Se hace un envio de bytes",INFO,"El envio tiene exito");
			result ++;
		}

		pthread_mutex_unlock(&Sem_GrabaBytes);

		return result;
}








/*************************Comandos de Consola:*************************/

void algoritmo(void){//Cambiar entre Worst fit y First fit
	pthread_mutex_lock(mutex_MP);
	if(configuracion_UMV.algoritmo==worstfit){
		configuracion_UMV.algoritmo=firstfit;
		printf("El algoritmo se cambio a: firstfit\n");
		escribir_log(archLog, "Se cambia el algoritmo de seleccion", INFO, "De worst-fit a first-fit");
	}
	else{
		configuracion_UMV.algoritmo=worstfit;
		printf("El algoritmo se cambio a: worstfit\n");
		escribir_log(archLog, "Se cambia el algoritmo de seleccion", INFO, "De first-fit a worst-fit");
	}
	pthread_mutex_unlock(mutex_MP);
}

/*************************Retardo*************************/

void retardoFunc(int retardo_nuevo){
	retardo=retardo_nuevo;
}



//****************************************Compactacion*****************************************

void compactar(){
		pthread_mutex_lock(&Sem_DevuelveBytes);
		pthread_mutex_lock(&Sem_GrabaBytes);
		pthread_mutex_lock(&Sem_Elimina_Segmento);
		pthread_mutex_lock(&Sem_Graba_Segmento);

		log_escribir(archLog,"Empieza el proceso de compactado",INFO,"");

		// liberamos la memoria todos los rangos que ya no usaremos.
		RangoMemoria * RangoInicial;
		list_iterate(Rangos_Libres, free);
		free(Rangos_Libres);
		Rangos_Libres = list_create();

		void * MemAux;
		t_list * MemAuxList = list_create();

		log_escribir(archLog,"Se libera la memoria se crea una lista nueva",INFO,"");

		sleep(Retardo);
		if(!list_is_empty(Segmentos_UMV)){

			Segmento * SegmentoAnterior;
			int posSegmento = 0;

			//Obtenemos el primer segmento de la lista
			Segmento * SegmentoActual = ((Segmento*)list_get(Segmentos_UMV,posSegmento));

			log_escribir(archLog,"El primer segmento",INFO,"Tiene base: %d y un tamaño: %d",SegmentoActual->baseVirtual,SegmentoActual->tamano);

			MemAux = malloc(SegmentoActual->tamano + 1);

			//Copiamos toda la info que va a estar en el nuevo lugar
			memcpy(MemAux, MP + SegmentoActual->baseVirtual, SegmentoActual->tamano);

			list_add(MemAuxList, MemAux);

			log_escribir(archLog,"copiamos el contenido del segmento",INFO,"");

			// asignamos la base 0 al primer compactado
			SegmentoActual->baseVirtual = 0;

			// Actualizamos la base del segmento en la tabla de BaseID
			BaseID * bi = BuscarBI(SegmentoActual->ID);
			bi->base = 0;

			log_escribir(archLog,"Actualizamos la base en la tabla de id",INFO,"");

			sleep(Retardo);

			posSegmento ++;
			while(list_size(Segmentos_UMV) > posSegmento && list_size(Segmentos_UMV) > 1 ){

				// sacamos el segmento de despues y guardamos el anterior en una auxiliar
				SegmentoAnterior = SegmentoActual;
				SegmentoActual = ((Segmento*)list_get(Segmentos_UMV,posSegmento));

				log_escribir(archLog,"Informacion de segmento",INFO,"Segmento numero %d tiene base: %d y un tamaño: %d",posSegmento + 1,SegmentoActual->baseVirtual,SegmentoActual->tamano);

				//Copiamos toda la info al segmento que va a estar en su lugar
				MemAux = malloc(SegmentoActual->tamano + 1);
				memcpy(MemAux, MP + SegmentoActual->baseVirtual, SegmentoActual->tamano);
				list_add(MemAuxList, MemAux);

				log_escribir(archLog,"Informacion de copiado de segmento",INFO,"copiamos el contenido del segmento: %d",SegmentoActual->baseVirtual);

				// Asignamos la nueva base al segmento
				SegmentoActual->baseVirtual = SegmentoAnterior->baseVirtual + SegmentoAnterior->tamano + 1;

				// Actualizamos la base del segmento en la tabla de BaseID
				BaseID * bi = BuscarBI(SegmentoActual->ID);
				bi->base =  SegmentoActual->baseVirtual;

				log_escribir(archLog,"Informacion de actualizacion",INFO,"Actualizamos la base en la tabla de id ID:%d Base:%d",bi->ID,bi->base);

				posSegmento ++;
			}

			int ultimaPosMemoriaOcupada = SegmentoActual->baseVirtual + SegmentoActual->tamano;

			RangoInicial = create_rango_mem(ultimaPosMemoriaOcupada +1 , tamanioMP - (ultimaPosMemoriaOcupada +1));
			log_escribir(archLog,"Informacion de creado",INFO,"creamos el ultimo RangoLibre Base:%d Tamaño:%d", RangoInicial->base,RangoInicial->tamano);

			// Copiamos toda la memoria a sus respectivos nuevos lugares
			int pos = 0;

			log_escribir(archLog,"Volcamos todos los datos comapctados a la memoria principal",INFO,"");
			while(pos < list_size(MemAuxList)){
				Segmento * aSeg = ((Segmento*)list_get(Segmentos_UMV,pos));
				MemAux = (void*)list_get(MemAuxList, pos);
				memcpy(MP + aSeg->baseVirtual, MemAux , aSeg->tamano);

				pos++;
			}

			list_iterate(MemAuxList, free);
			free(MemAuxList);

		}
		else{
			RangoInicial = create_rango_mem(0 , tamanioMP);
			log_escribir(archLog,"No hay segmentos en memoria, se compacta toda la memoria libre",INFO,"");
		}

		sleep(Retardo);
		list_add(Rangos_Libres,RangoInicial);

		pthread_mutex_unlock(&Sem_DevuelveBytes);
		pthread_mutex_unlock(&Sem_GrabaBytes);
		pthread_mutex_unlock(&Sem_Elimina_Segmento);
		pthread_mutex_unlock(&Sem_Graba_Segmento);
}

// devuelve la posicion de un segmento en la lista de segmentos por su base
BaseID * BuscarBI(int ID){
	BaseID * bi;
	int pos = 0;

	while( pos < list_size(List_Base_ID) ){
		bi = (BaseID*)list_get(List_Base_ID,pos);
		if(bi->ID == ID)
			break;
		pos ++;
	}

	return bi;
}

//Nos muestra los rangos libres de mem -- ok
void MostrarTablaBI() {

	void PrintTable(BaseID *  rango) {
		 printf("Rango: Base %d , ID: %d \n", rango->base, rango->ID);
	}

	printf("TABLAS DE BASESxID: \n");
	list_iterate(List_Base_ID, (void*)PrintTable);

}




/*************************Dump: *************************/

/*
 * Nombre:dump
 * Argumentos: void
 *
 *
 * Devuelve: void
 *
 * Funcion: Genera archivos con el estado de la MP y la tabla de segmentos
 */

void dump(){
	FILE* archivo_MP;
	FILE* archivo_TS;
	int procesoAVer;
	//void imprimirBuffer(t_buffer);
	//void imprimirBufferEnArchivo(t_buffer,FILE*);

	pthread_mutex_lock(mutex_MP);

	archivo_MP = fopen("/home/utnso/dump_file_MP", "a");
	archivo_TS = fopen("/home/utnso/dump_file_TS", "a");
	if (archivo_MP==NULL) {
		fputs ("File error",stderr); exit (1);
	}
	if (archivo_TS==NULL) {
		fputs ("File error",stderr); exit (1);
	}


	//Estructuras de memoria
	fprintf(archivo_MP, "Dump del estado de la memoria principal\n");
	fprintf(archivo_TS, "Dump del estado de la tabla de segmentos\n");

	puts("\nIngrese el numero de proceso del cual se quiere conocer sus segmentos o '-1' para verlos todos");
	scanf("%d",&procesoAVer);
	if(procesoAVer == -1){
		//imprimirEstadoTablaSeg(archivo_TS,0,cant_tablas);
	}else {
		//int ubicacion= getPosTabla(procesoAVer);
		//int tablaFinal=ubicacion+1;
		DumpDeUnPrograma(procesoAVer,archivo_TS);
	}

	//Memoria Principal

	//Segmentos de los programas TODO


	// Logueamos los Segmentos
	void logSegmentos(Segmento* rango) {
		 fprintf(archivo_TS,"\t Programa: %d - ID: %d - Base %d  - Tam: %d \n",rango->programa,rango->ID, rango->baseVirtual, rango->tamano);
	}

	fprintf(archivo_TS,"Segmetos actuales del Sistema: \n");

	list_iterate(Segmentos_UMV, (void*)logSegmentos);


	//Espacio libre
	fprintf(archivo_MP,"\tCantidad Memoria Libre: %d \n", CantidadMemoriaLibre());


	//Contenido de la memoria principal
	int offset,tamanio;
	char* buffer;
	printf("\nIngrese el offset con la posicion de MP a conocer y la cantidad de bytes a leer\n");
	scanf("%d", &offset);
	scanf("%d", &tamanio);



	pthread_mutex_lock(&Sem_DevuelveBytes);

	buffer = malloc((tamanio)*sizeof(char));
	memcpy(buffer, MP+offset, tamanio);
	printf("\nLa posicion de memoria %d contiene: %s\n", offset, (char*)buffer);
	fprintf(archivo_MP, "La posicion de memoria %d contiene: %s\n", offset, (char*)buffer);//Ojo que pongo archivo_MP pero capaz deberia ser en otro

	pthread_mutex_unlock(&Sem_DevuelveBytes);
	//imprimirEstadoMP(archivo_MP);//-Ya no deberia ir no?-Va escribiendo en el archivo el contenido de las posiciones de la MP
	pthread_mutex_unlock(mutex_MP);

	fprintf(archivo_MP,"Termina el dump del estado de la memoria");
	fprintf(archivo_TS,"Termina el dump del estado de la tabla de segmentos");

	escribir_log(archLog, "Se realiza un dump", INFO, "El dump se realiza con exito");

	fclose(archivo_MP);
	fclose(archivo_TS);
}

// Muestreo de Datos por Programa
void DumpDeUnPrograma(int Programa,FILE* archivo){

	if(list_is_empty(Segmentos_UMV))
			return;

	void Mostrar(Segmento* rango) {
		 if(Programa == rango->programa)
			 fprintf(archivo,"\tRango: Base %d  - Tamano: %d \n", rango->baseVirtual, rango->tamano);
	}

	fprintf(archivo,"Segmetos actuales del programa: %d \n", Programa);
	list_iterate(Segmentos_UMV, (void*)Mostrar);
}

void imprimirEstadoMP(FILE* archivo){
/*	int i=0;
	fprintf(archivo, "%s", "El estado de la memoria principal:\n");
		while(i<tamanioMP){
			fprintf(archivo, "%s", "La posicion ");
			fprintf(archivo, "%d", i);
			fprintf(archivo, "%s", " contiene ");
			if(MP[i]==NULL){
				fprintf(archivo, "%s", "NULL");
			} else{
			fprintf(archivo, "%d", MP[i]);
			}
			fprintf(archivo, "%s", " \n");
			i++;
		}*/
}

void imprimirEstadoTablaSeg(FILE* archivo,int i, int tablaFinal){
	int j;
	fprintf(archivo, "%s", "El estado de la tabla de segmentos:\n\n");
			while(i<tablaFinal){
				j=0;
				fprintf(archivo, "%s", "La tabla ");
				fprintf(archivo, "%d", i);
				fprintf(archivo, "%s", ":\n Corresponde al programa ");
				fprintf(archivo, "%d", tablaDeSegmentos[i].id_prog);
				fprintf(archivo, "%s", "\n Cantidad de segmentos ");
				fprintf(archivo, "%d", tablaDeSegmentos[i].cant_segmentos);

				printf("La tabla ");
				printf("%d", i);
				printf(":\n Corresponde al programa ");
				printf("%d", tablaDeSegmentos[i].id_prog);
				printf("\n Cantidad de segmentos ");
				printf("%d", tablaDeSegmentos[i].cant_segmentos);

				while(j<tablaDeSegmentos[i].cant_segmentos){

				fprintf(archivo, "%s", "\n Segmento ");
				fprintf(archivo, "%d", j);
				fprintf(archivo, "%s", ":\n Posicion real ");
				fprintf(archivo, "%d", tablaDeSegmentos[i].segmentos[j].ubicacionMP);
				fprintf(archivo, "%s", "\n Posicion virtual ");
				fprintf(archivo, "%d", tablaDeSegmentos[i].segmentos[j].inicio);
				fprintf(archivo, "%s", "\n Tamanio ");
				fprintf(archivo, "%d", tablaDeSegmentos[i].segmentos[j].tamanio);

				printf("\n Segmento ");
				printf("%d", j);
				printf(":\n Posicion real ");
				printf("%d", tablaDeSegmentos[i].segmentos[j].ubicacionMP);
				printf("\n Posicion virtual ");
				printf("%d", tablaDeSegmentos[i].segmentos[j].inicio);
				printf("\n Tamanio ");
				printf("%d", tablaDeSegmentos[i].segmentos[j].tamanio);

				j++;
				}
				fprintf(archivo, "%s", " \n\n");
				printf("\n");
				i++;
			}
}

// Nos dice la cantidad total de memoria libre,
int CantidadMemoriaLibre() {

	int tamanoTotal = 0;

	void ContarTamano(RangoMemoria* rango) {
		tamanoTotal = tamanoTotal + rango->tamano;
	}

	list_iterate(Rangos_Libres, (void*)ContarTamano);

	return tamanoTotal;
}



/*
 * Nombre: crearSegmentoPrograma
 * Argumentos: id_prog, tamanio
 *
 *
 * Devuelve: int (se pudo crear o no)
 *
 * Funcion: Dado un id_prog y un tamanio, reserva espacio en MP para el programa
 */

int crearSegmentoPrograma(int id_prog, int tamanio){
	int result;
	RangoMemoria rango;
	if(tamanio>0){
	pthread_mutex_lock(&Sem_GrabaBytes);

	if (!SePuedeGrabarSegmento(tamanio)) {
		printf("MEMORY OVERLOAD, No hay memoria para grabar el segmento \n");
		log_escribir(archLog,"Memory Overload",ERROR,"No se puede crear el segmento porque no hay memoria suficiente");
		result = -1;
	}
	else
	{
		sleep(Retardo);
		// segun el algoritmo se elige un rago de memoria para grabar
		if (configuracion_UMV.algoritmo == firstfit)
		{
			int pos = 0;
			long int tamanoRangoLibre = 0;
			// calculamos el primer rango de memoria en el que entra el segmento
			while (tamanoRangoLibre < tamanio)
			{
				rango = *((RangoMemoria*)list_get(Rangos_Libres, pos));
				tamanoRangoLibre = rango.tamano;
				pos++;
			}
			log_escribir(archLog,"Algoritmo de seleccion",INFO,"First-Fit");
			}
		else // algortimo worst fit
		{
			rango = RangoMasGrandeLibre();
			log_escribir(archLog,"Algoritmo de seleccion",INFO,"Worst-Fit");
		}
		sleep(Retardo);
		// calculamos el id aletoria dentro del rango
		int ID = rand() % tamanioMP + 1;
		int pos = 0;

		//**** Este while villero nos dice si un segmento ya tiene ese id asignado ****
		while(!list_is_empty(List_Base_ID)){
			// si la pos es mayor o igual al tamaño de la lista es porque no esta.
			if(list_size(List_Base_ID) <= pos) break;

			BaseID bi = *((BaseID*)list_get(List_Base_ID,pos));
			if(bi.ID == ID){
				pos = 0;
				ID = rand() % 1 + tamanioMP;
			}else{
				pos ++;
			}
		}
		printf("La base es: %d\n",ID);
		printf("La dir real es: %d\n",rango.base);
		sleep(Retardo);

		// guardamos ordenado
		GuardarNuevoSegmentoOrdenado(ID, id_prog, rango.base, tamanio);

		log_escribir(archLog,"Se guardo el nuevo segmento ordenado",INFO,"");

		ActualizarRangoGrabado(tamanio, rango);

		//Se agrega la referencia del ID a esa base
		BaseID * bi2 = create_BaseID(rango.base, ID);
		list_add(List_Base_ID,bi2);

		result = ID;
	}
	pthread_mutex_unlock(&Sem_GrabaBytes);

	log_escribir(archLog,"Se solicita crear un segmento",INFO,"El segmento se crea con exito");
	return result;
	}

	log_escribir(archLog,"Se solicita crear un segmento",ERROR,"El tamanio indicado es menor a 0");
	result=-2;
	return result;

}

// crea un segmento nuevo -- OK
Segmento *create_segmento(int ID,int programa, void* base, int base_virtual,int tamano) {
	Segmento * segmento_nuevo = malloc(sizeof(Segmento));
	segmento_nuevo->ID = ID;
	segmento_nuevo->base = base;
	segmento_nuevo->baseVirtual = base_virtual;
	segmento_nuevo->programa = programa;
	segmento_nuevo->tamano = tamano;

	return segmento_nuevo;
}

// crea un rango de memoria --OK
RangoMemoria *create_rango_mem(int base, int tamano) {
	RangoMemoria *aRango = malloc(sizeof(RangoMemoria));
	aRango->base = base;
	aRango->tamano = tamano;

	return aRango;
}

// crea una BaseID
BaseID *create_BaseID(int base, int ID) {
	BaseID *aRango = malloc(sizeof(BaseID));
	aRango->base = base;
	aRango->ID = ID;

	return aRango;
}

int GuardarNuevoSegmentoOrdenado(int ID, int programa, int base_virtual, int tamano) {

	Segmento * nuevo_segmento = create_segmento(ID,programa,(MP + base_virtual), base_virtual, tamano);

	if (list_is_empty(Segmentos_UMV)) { // si la lista esta vacia lo agregamos en la primer posicion
		list_add(Segmentos_UMV, nuevo_segmento);
		return 1;
	}
	else {   // si no esta vacia agregamos ordenado por la baseVirtual
		int pos = 0;
		Segmento aSeg = *((Segmento*) list_get(Segmentos_UMV, pos));
		while (list_size(Segmentos_UMV) > pos && aSeg.baseVirtual < base_virtual) // vamos a la posicion donde este ordenado
		{
			pos++;
			if(pos != list_size(Segmentos_UMV)){
				aSeg = *((Segmento*) list_get(Segmentos_UMV, pos));
			}
		}

		list_add_in_index(Segmentos_UMV, pos, nuevo_segmento);

		//logueamos
		log_escribir(archLog,"Guardamos el segmento en la lista",INFO,"En la pos %d",pos);

		return 1;
	}
}

// Nos dice si hay memoria disponible junta para grabar un sengmento de tamaño fijo -- Ok
bool SePuedeGrabarSegmento(int tamano) {


	// esta funcion auxiliar para fijarse si algun rango satisface la condicion
	int Hay_algun_rango_de_tamano_suficiente(RangoMemoria *rango) {
		return rango->tamano >= tamano;
	}

	bool result = (list_any_satisfy(Rangos_Libres, (void*) Hay_algun_rango_de_tamano_suficiente));

	log_escribir(archLog,"Checkea si se puede grabar un segmento",INFO,"");

	return result;
}

// Nos devuelve la pos del rango de mayor tamaño -- OK
RangoMemoria RangoMasGrandeLibre() {
	int pos = 0;
	long int mayorTamano = 0;
	RangoMemoria mayorRango;
	escribir_log(archLog,"Adquiriendo rango de memoria mas grande",INFO,"");

	while (list_size(Rangos_Libres) > pos) {
		RangoMemoria rango = *((RangoMemoria*) list_get(Rangos_Libres, pos));
		if (rango.tamano > mayorTamano) {
			mayorRango = rango;
			mayorTamano = rango.tamano;
		}
		pos++;
	}

	return mayorRango;
}

//Actualiza la lista de rangos de memoria libres
void ActualizarRangoGrabado(int tamano_guardado, RangoMemoria rango){
	RangoMemoria * rangoAux;
	RangoMemoria * rangoActualizado = create_rango_mem(rango.base + tamano_guardado + 1, rango.tamano - tamano_guardado);
	int posRemplazo = 0;

	// Buscamos la posicion del rango que queremos actualizar
	while (list_size(Rangos_Libres) > posRemplazo) // vamos a la posicion donde este ordenado
	{
	    rangoAux = (RangoMemoria*)list_get(Rangos_Libres,posRemplazo);
		// si encuentra el rango con la misma base tomamos esa posicion para remplazarlo por el rango actualizado
		if(rangoAux->base == rango.base)
			break;

		posRemplazo++;
	}

	// Remplazamos en la posicion encontrada
	if(rangoActualizado->tamano == 0)
		list_remove(Rangos_Libres,posRemplazo);
	else
		list_replace(Rangos_Libres,posRemplazo, rangoActualizado);

	free(rangoAux);

}




/*
 * Nombre: destruirSegmentosPrograma
 * Argumentos: id_prog
 *
 *
 * Devuelve: void
 *
 * Funcion: Dado un id_prog, libera el espacio reservado para el programa
 */

void destruirSegmentos(int id_prog){

	pthread_mutex_lock(&Sem_Elimina_Segmento);

	Segmento segmento;
	int pos = 0;

	while( pos < list_size(Segmentos_UMV))
	{
		segmento = *((Segmento*)list_get(Segmentos_UMV,pos));
		if(segmento.programa == id_prog)
		{
			EliminarSegmento(segmento.baseVirtual);
			sleep(Retardo);
		}
		else
		{
			pos ++;
		}
	}

	pthread_mutex_unlock(&Sem_Elimina_Segmento);

	log_escribir(archLog,"Informacion de destruccion de segmentos de un programa",INFO,"se eliminaron los segmentos del programa ID: %d",id_prog);
}

// elimina un segmento por su base
void EliminarSegmento(int base){
	Segmento aSeg = BuscarSegmento(base);
	BaseID * bi;
	RangoMemoria * aRango = create_rango_mem(aSeg.baseVirtual,aSeg.tamano);
	list_remove(Segmentos_UMV,PosicionDeSegmento(base));

	int posBI = 0;
	while( posBI <= ((int)list_size(List_Base_ID)) ){
		bi = (BaseID*)list_get(List_Base_ID,posBI);
		if(bi->base == aSeg.baseVirtual)
			break;
		posBI ++;
	}


	list_remove(List_Base_ID,posBI);
	free(bi);

	int pos = 0;

	while(pos < list_size(Rangos_Libres)){
		RangoMemoria RangoAux = *((RangoMemoria*)list_get(Rangos_Libres,pos));

		if(RangoAux.base > aRango->base)
			break;
		pos ++;
	}

	if(aRango->tamano > 0)
		list_add_in_index(Rangos_Libres,pos, aRango);

	//logueamos
	log_escribir(archLog,"Informacion de eliminacion de segmento",INFO,"se elimino el segmento de base: %d",base);

}

// devuelve la posicion de un segmento en la lista de segmentos por su base --OK
Segmento BuscarSegmentoIndice(int ID){
	BaseID bi;
	int pos = 0;
	log_escribir(archLog,"Informacion de busqueda de segmento",INFO,"Buscando el segmento de id %d",ID);
	while( pos < list_size(List_Base_ID)){
		bi = *((BaseID*)list_get(List_Base_ID,pos));
		if(bi.ID == ID)
			break;
		pos ++;
	}

	if(pos < list_size(List_Base_ID))
		return BuscarSegmento(bi.base);
	else
		return *create_segmento(1,-1,MP+tamanioMP,-1,MP);

}

// devuelve la posicion de un segmento en la lista de segmentos por su base
int PosicionDeSegmento(int base){
	Segmento segmento;
	int pos = 0;
	while( pos <= list_size(Segmentos_UMV) ){
		segmento = *((Segmento*)list_get(Segmentos_UMV,pos));
		if(segmento.baseVirtual == base)
			break;
		pos ++;
	}

	return pos;
}

// devuelve la posicion de un segmento en la lista de segmentos por su base
Segmento BuscarSegmento(int base){
	Segmento segmento;
	int pos = 0;
	log_escribir(archLog,"Informacion de busqueda de segmento",INFO,"Buscando el segmento de base %d",base);
	while( pos <= ((int)list_size(Segmentos_UMV)) ){
		segmento = *((Segmento*)list_get(Segmentos_UMV,pos));
		if(segmento.baseVirtual == base)
			break;
		pos ++;
	}

	return segmento;
}

int getPosTabla(int id_prog){
	int i=0;
	while(tablaDeSegmentos[i].id_prog != id_prog && i<cant_tablas) i++;
	if(i==cant_tablas){
		log_escribir(archLog, "Id_prog no encontrado", ERROR, "El id_prog no se encuentra en la tabla de segmentos");
		return -1;
	}
	return i;
}



void eliminarSegmentos(int pos){
	free(tablaDeSegmentos[pos].segmentos);
	tablaDeSegmentos[pos].cant_segmentos=0;
}

void cambioProcesoActivo(int id_prog){
	procesoActivo = id_prog;
}


//Funciones para lectura del archivo config y una funcion que imprime dichos campos para testear la lectura

void leerConfiguracion(void) {

	t_config* config = config_create(PATH);

		configuracion_UMV.memSize = config_get_int_value(config,
					"MemSize");

		configuracion_UMV.puerto_conexiones = config_get_int_value(config,
					"Puerto TCP para recibir conexiones");


		configuracion_UMV.algoritmo = config_get_int_value(config,
					"Algoritmo de seleccion de ubicacion de segmento");

}

void imprimirConfiguracion(void) { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("Tamanio de memoria Principal: %d\n", configuracion_UMV.memSize);
	printf("Puerto para conexiones: %d\n", configuracion_UMV.puerto_conexiones);
	printf("Algoritmo de segmentacion: %d\n", configuracion_UMV.algoritmo);
}

void inicializarConfiguracion(void){
	archLog = log_crear(PATHLOG);
	struct stat file_info;
	int control = lstat(PATH, &file_info);
	if (control == -1){
		escribir_log(archLog, "Leer archivo de configuracion", ERROR, "El archivo no existe");
	}else{
	leerConfiguracion();
	imprimirConfiguracion(); //Imprime las configuraciones actuales por pantalla
	}
}

int escribir_log(log_t *log, const char *program_name, e_message_type type,	const char* format){
	pthread_mutex_lock(mutex_log);
	int i = log_escribir(log, program_name, type, format);
	pthread_mutex_unlock(mutex_log);
	return i;
}

void ejecutar(t_tipoEstructura tipo_estructura,void* estructura,sock_struct* sock_cpu){/*
			int baseStack; //Tendria que ser global y creada con la conexion del kernel?
			int base;
			t_signal senial;
			t_struct_signal* signal = malloc(sizeof(t_struct_signal));
			t_tipoEstructura tipoRecibido;
			void* structRecibida;
			t_struct_push* structPush;
			t_struct_pop* structPop;
			t_struct_instruccion* structInstr;
			t_struct_seg_codigo* structCodigo;
			socket_recibir(sock_cpu->fd,&tipoRecibido,&structRecibida);

			printf("recibi el tipo %d\n",tipoRecibido);

			switch(tipoRecibido){
			case D_STRUCT_PUSH:
				structPush= ((t_struct_push*)structRecibida);
				int pos= structPush->posicion;
				int valor= structPush->valor;
				baseStack = structPush->stack_base;
				if(enviarBytes(baseStack,pos,sizeof(valor),(int*)valor)==0){
					senial = D_STRUCT_NORMAL;
					signal->signal = senial;
					socket_enviarSignal(sock_cpu->fd,senial);
				}else{
					senial = D_STRUCT_SEGFAULT;
					signal->signal = senial;
					socket_enviarSignal(sock_cpu->fd,senial);
				}
				free(structRecibida);
				break;			//Revisar bien los tipos del valor (int,t_buffer,void*) y como manejarlos

			case D_STRUCT_POP:
				structPop= ((t_struct_pop*)structRecibida);
				pos= structPop->posicion;
				int tamanio = structPop->tamanio;
				baseStack = structPop->stack_base;

				//ACA VENDRIA UN IF CHEQUEANDO QUE SE PUEDE HACER LO QUE LA CPU ME PIDE Y MANDAN SIGNAL
				if(solicitarBytes(baseStack,pos,tamanio) != NULL){
					senial = D_STRUCT_NORMAL;
					signal->signal = senial;
					socket_enviarSignal(sock_cpu->fd,senial);

					uint32_t valor_a_enviar = solicitarBytes(baseStack,pos,tamanio);
					t_struct_numero* estructura = malloc(sizeof(t_struct_numero));
					estructura->numero = valor_a_enviar;
					socket_enviar(sock_cpu->fd, D_STRUCT_NUMERO, estructura);
					free(estructura);
				}else{
					senial = D_STRUCT_SEGFAULT;
					signal->signal = senial;
					socket_enviarSignal(sock_cpu->fd,senial);
				}

				break;
			case D_STRUCT_INSTRUCCION:
				structInstr=(t_struct_instruccion*)structRecibida;
				base=structInstr->indice_codigo; //El segmento almacenado que en realidad es indice de codigo
				pos=structInstr->inst;//La instruccion correspondiente
				//tamanio=2*sizeof(int); //Tamaño fijo de cada lugar en el indice de codigo (4(indice)+4(longitud) bytes)
				//long long int posDelIndice; //Tiene que ser de 8 bytes (Pag 16 enunciado)
				//posDelIndice=solicitarBytes(base,pos,tamanio);
				//t_struct_seg_codigo* segAEnviar;

				printf("me llego de instruccion base %d y pos %d\n",base, pos);

				uint32_t* start;
				uint32_t* offset;
				tamanio = sizeof(int);

				if((start=solicitarBytes(base,pos,tamanio)) == NULL){
						printf("SE ROMPIO TODO\n");

						senial = D_STRUCT_SEGFAULT;
						signal->signal = senial;
						socket_enviarSignal(sock_cpu->fd,senial);}

				pos += sizeof(int);

				if((offset=solicitarBytes(base,pos,tamanio)) != NULL){

					printf("TODO PIOLA POR ACA\n");
					senial = D_STRUCT_NORMAL;
					signal->signal = senial;
					socket_enviarSignal(sock_cpu->fd,senial);

					t_intructions instruccion;
					instruccion.start=*start;
					instruccion.offset=*offset;

					t_struct_seg_codigo* estruc = malloc(sizeof(t_struct_seg_codigo));
					estruc->inst = instruccion;
					estruc->seg_codigo = 0;//No uso el numero,asi que me mandan cualquier cosa
					socket_enviar(sock_cpu->fd, D_STRUCT_SEGCODIGO, estruc);
					free(estruc);

					} else {
						printf("SE ROMPIO TODO\n");
						senial = D_STRUCT_SEGFAULT;
						signal->signal = senial;
						socket_enviarSignal(sock_cpu->fd,senial);}


				//segAEnviar->inst=primeros4Bytes(posDelIndice);
				//int longitudDeInstr=segundos4Bytes(posDelIndice); //Esta variable se va a usar en el pedido del codigo
				//socket_enviar(sock_cpu,D_STRUCT_SEGCODIGO, segAEnviar);
				//-------------------Aca se espera la devolucion del seg o es otro case? Preg a juli

				break;

			case D_STRUCT_SEGCODIGO:
				structCodigo = (t_struct_seg_codigo*)structRecibida;
				t_intructions inst = structCodigo->inst;
				base = structCodigo->seg_codigo;

				if(enviarBytes(baseStack,pos,sizeof(valor),(int*)valor)==0){
					senial = D_STRUCT_NORMAL;
					signal->signal = senial;
					socket_enviarSignal(sock_cpu->fd,senial);

					char* linea = solicitarBytes(base,inst.start,inst.offset);

					t_struct_string* structure = malloc(sizeof(t_struct_string));
					structure->string = linea;

					int j=socket_enviar(sock_cpu->fd,D_STRUCT_STRING,estructura);
					if(j == 1){
						free(structure);
					}
				}else{
					senial = D_STRUCT_SEGFAULT;
					signal->signal = senial;
					socket_enviarSignal(sock_cpu->fd,senial);
				}

				break;
			default:
				escribir_log(archLog, "Solicitud de CPU", ERROR, "Solicitud no reconocida");
				break;
			//Etc

			}
			free(signal);*/
	}

//****************************************Atender Conexiones de Kernel/CPU*******************

void handshake_conexion(void){
	if((sock_servidor=socket_crearServidor("127.0.0.1",configuracion_UMV.puerto_conexiones))>0){
	printf("Hilo de Conexiones\n");
	escribir_log(archLog, "Escuchando en el socket de Conexiones", INFO, "");
	}
	int sock_aceptado;

	//Recibir Handshake:
	t_tipoEstructura tipoRecibido;
	void* structRecibida;

	pthread_t atender_pedido;

	t_struct_numero* numeroEnviado = malloc(sizeof(t_struct_numero));
	while(1){
		sock_struct* sock = malloc(sizeof(sock_struct));
		if((sock_aceptado=socket_aceptarCliente(sock_servidor))>0){
			printf("Acepta conexion\n");
			escribir_log(archLog, "Conexion", INFO, "Se acepta una conexion");
		}
		socket_recibir(sock_aceptado, &tipoRecibido, &structRecibida);
		t_struct_numero* numeroRecibido = ((t_struct_numero*)structRecibida);
		sock->fd=sock_aceptado;
		switch(numeroRecibido->numero){
			case 0:
				numeroEnviado->numero=0;
				socket_enviar(sock_aceptado, D_STRUCT_NUMERO, numeroEnviado);
				escribir_log(archLog, "Handshake", INFO, "La conexion es de Kernel");
				pthread_create(&atender_pedido, NULL, (void*) &atender_kernel, sock);
				break;
			case 1:
				numeroEnviado->numero=1;
				socket_enviar(sock_aceptado, D_STRUCT_NUMERO, numeroEnviado);
				escribir_log(archLog, "Handshake", INFO, "La conexion es de CPU");
				pthread_create(&atender_pedido, NULL, (void*) &atender_cpu, sock);
				break;

		}
		free(structRecibida);
	}

	free(numeroEnviado);

	if(socket_cerrarConexion(sock_servidor)==0){
		escribir_log(archLog, "Se trata de cerrar el socket de Kernel", ERROR, "Hay problemas para cerrar el socket");
		//Error cerrando el socket
	} else {
		escribir_log(archLog, "Se cierra el socket de Kernel", INFO, "No hay problemas para cerrar el socket");
	}

	return;
}



void atender_cpu(sock_struct* sock){
	pthread_detach(pthread_self());

	//RECIBO PEDIDO DE INDICE DE ETIQUETAS
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	t_struct_sol_bytes * solicitud;
	t_struct_env_bytes* escritura;
	while(1){
		sleep(retardo);
		socket_recibir(sock->fd, &tipoRecibido,&structRecibida);
		switch(tipoRecibido){
			case D_STRUCT_SOL_BYTES:
				solicitud = (t_struct_sol_bytes*) structRecibida;
				pthread_mutex_lock(mutex_log);
				log_escribir(archLog,"Solicitud bytes",INFO, "Se solicitan; base: %d, offset: %d, tamanio: %d",solicitud->base, solicitud->offset, solicitud->tamanio);
				pthread_mutex_unlock(mutex_log);
				t_struct_respuesta_umv buffer = solicitarBytes(solicitud->base, solicitud->offset, solicitud->tamanio);
				socket_enviar(sock->fd, D_STRUCT_RESPUESTA_UMV, &buffer);
				break;
			case D_STRUCT_ENV_BYTES:
				escritura = (t_struct_env_bytes*) structRecibida;
				printf("%d        %d        %d", escritura->base, escritura->offset, escritura->tamanio);
				pthread_mutex_lock(mutex_log);
				log_escribir(archLog, "Se envian bytes",INFO, "base: %d, offset:%d , tamanio: %d",escritura->base, escritura->offset, escritura->tamanio);
				pthread_mutex_unlock(mutex_log);

				int resultado = enviarBytes(escritura->base,escritura->offset,escritura->tamanio,escritura->buffer);
				t_struct_numero* respuesta = malloc(sizeof(t_struct_numero));
				respuesta->numero = resultado;
				socket_enviar(sock->fd, D_STRUCT_NUMERO, &respuesta);
				pthread_mutex_lock(mutex_log);
				log_escribir(archLog, "Resultado",INFO,"Envio de bytes es: %d",resultado);
				pthread_mutex_unlock(mutex_log);
				free(respuesta);
				break;
			case 0:
				//log_escribir(archLog,"Termina la ejecucion de un pedido",INFO,"El pedido es de CPU");
				break;
		}
	}
	/*		printf("me llego un D_STRUCT_INDICE\n");
		}
	indice = ((t_struct_indice_etiquetas*)structRecibida);
	printf("me llego esto %d y %d\n",indice->etiquetas_size, indice->index_etiquetas);

	t_struct_string* estructura = malloc(sizeof(t_struct_string));
	estructura->string = "hola wachada";		//ACA MANDAN EL INDICE DE ETIQUETAS EN BASE A LO RECIBIDO

	int j=socket_enviar(sock->fd,D_STRUCT_STRING,estructura);
	if(j == 1){
	printf("se envio el intento de indice de etiquetas\n");
	free(estructura);
	}

	ejecutar(tipoRecibido,structRecibida,sock);*/

	free(sock);
	return;
}






void atender_kernel(sock_struct* sock){
	pthread_detach(pthread_self());
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	t_struct_memoria* tamanio;
	t_struct_numero* pid;
	t_struct_segmento* struct_seg;
	int i,id_prog,memoriaSuficiente=0;
	int tamanioSolicitado,tamanio_escribir;
	int base_stack,base_codigo,base_index_code,base_index_etiq;
	socket_recibir(sock->fd, &tipoRecibido, &structRecibida);
	tamanioMaxStack = ((t_struct_numero*)structRecibida)->numero;
	pthread_mutex_lock(mutex_log);
	log_escribir(archLog,"Se recibe el tamanio del stack",INFO,"El tamanio es: %d", tamanioMaxStack);
	pthread_mutex_unlock(mutex_log);
	free(structRecibida);


	while(1){
		sleep(retardo);
		socket_recibir(sock->fd, &tipoRecibido,&structRecibida);

		switch(tipoRecibido){
			case D_STRUCT_NUMERO:
				pid = ((t_struct_numero*)structRecibida);
				id_prog = pid->numero;
				pthread_mutex_unlock(mutex_log);
				log_escribir(archLog,"Se recibe un ID",INFO,"El id es: %d", id_prog);
				pthread_mutex_unlock(mutex_log);
				pthread_mutex_lock(mutex_pid);
				cambioProcesoActivo(id_prog);
				pthread_mutex_lock(mutex_log);
				log_escribir(archLog,"Se cambia el proceso activo",INFO,"El pid del proceso activo es: %d", id_prog);
				pthread_mutex_unlock(mutex_log);
				pthread_mutex_unlock(mutex_pid);
				free(pid);
				break;

			case D_STRUCT_SOLICITARMEMORIA:
				tamanio = ((t_struct_memoria*)structRecibida);
				escribir_log(archLog,"Se reciben tamanios de segmentos",INFO,"");
				pthread_mutex_lock(mutex_pid);
				//t_struct_numero* respuesta= malloc(sizeof(t_struct_numero));
				base_stack = crearSegmentoPrograma(procesoActivo, tamanioMaxStack);
				base_codigo = crearSegmentoPrograma(procesoActivo, tamanio->tamanioScript);
				base_index_code = crearSegmentoPrograma(procesoActivo, tamanio->tamanioIndiceCodigo);
				base_index_etiq = crearSegmentoPrograma(procesoActivo, tamanio->tamanioIndiceEtiquetas);

				pthread_mutex_unlock(mutex_pid);
				if((base_stack!=-1)&&(base_codigo!=-1)&&(base_index_code!=-1)&&(base_index_etiq!=-1)){
					//respuesta->numero=memoriaSuficiente;
					t_struct_bases* respuesta = malloc(sizeof(t_struct_bases));
					respuesta->stack=base_stack;
					printf("Stack: %d\n",base_stack);
					respuesta->codigo=base_codigo;
					printf("codigo: %d\n",base_codigo);
					respuesta->indice_codigo=base_index_code;
					printf("ind cod: %d\n",base_index_code);
					respuesta->indice_etiquetas=base_index_etiq;
					printf("ind et: %d\n",base_index_etiq);
					escribir_log(archLog,"Se envian bases de segmentos",INFO,"");
					socket_enviar(sock->fd, D_STRUCT_BASES, respuesta);
					free(respuesta);

				}else{
					pthread_mutex_lock(mutex_pid);
					destruirSegmentos(id_prog);
					pthread_mutex_unlock(mutex_pid);
					t_struct_numero* respuesta = malloc(sizeof(t_struct_numero));
					respuesta->numero=1;
					socket_enviar(sock->fd, D_STRUCT_SF, respuesta);
					escribir_log(archLog, "Memoria insuficiente", ERROR, "No hay memoria suficiente para el programa");
					free(respuesta);
				}

				break;

			case D_STRUCT_DESTRUIRSEGMENTOS:
				pid = ((t_struct_numero*)structRecibida);
				log_escribir(archLog,"Se recibe peticion de destruccion de segmentos del programa: %d",INFO,pid->numero);
				pthread_mutex_lock(mutex_pid);
				cambioProcesoActivo(pid->numero);
				destruirSegmentos(procesoActivo);
				pthread_mutex_lock(mutex_log);
				log_escribir(archLog, "Destruir Segmentos del programa: %", INFO, "Por solicitud del kernel se destruyen los segmentos del proceso: %d");
				pthread_mutex_unlock(mutex_log);
				pthread_mutex_unlock(mutex_pid);
				free(pid);
				break;

			case D_STRUCT_ESCRIBIRSEGMENTO:
				struct_seg = ((t_struct_segmento*) structRecibida);
				printf("%d         %d\n", struct_seg->base, struct_seg->tamanio);
				t_intructions* aux=malloc(sizeof(t_intructions));
				memcpy(aux,((t_intructions *)struct_seg->segmento),sizeof(t_intructions));
				printf("Supuesto offset: %d      Supuesto tamanio: %d\n",aux->start,aux->offset);
				if(struct_seg->tamanio==0){
					escribir_log(archLog,"Se realizo envio de bytes",INFO,"El segmento es de tamanio 0");

				} else {
					enviarBytes(struct_seg->base,0,struct_seg->tamanio,struct_seg->segmento);
				}
					t_struct_numero* respuesta= malloc(sizeof(t_struct_numero));
					respuesta->numero=1;
					socket_enviar(sock->fd, D_STRUCT_NUMERO, respuesta);

				free(struct_seg);

		}




		//No envio nada al segmento de stack, porque al inicio estaría vacio


	}


	free(sock);
	return;
}


//***********************************************Inicializacion de semaforos************************************


void inicializarMutex(void){
	mutex_MP=malloc(sizeof(pthread_mutex_t));
	mutex_log=malloc(sizeof(pthread_mutex_t));
	mutex_pid=malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex_MP,NULL);
	pthread_mutex_init(mutex_log, NULL);
	pthread_mutex_init(mutex_pid,NULL);
}

//***********************************************Inicializacion y espera de hilos************************************

void inicializarHilos(void){
	pthread_create(&CONSOLA, NULL, (void*) &core_consola, NULL);
	pthread_create(&CONEXIONES, NULL, (void*) &handshake_conexion, NULL);
}

void esperarHilos(void){
	pthread_join(CONSOLA,NULL);
}

//***********************************************Consola************************************

void *core_consola(void) {
	pthread_t inicio;
	pthread_create(&inicio, NULL, (void*) &consola, NULL);
	pthread_join(inicio,NULL);
	return EXIT_SUCCESS;
}

void *consola (void){

	char comando[32];
	char* aux_buffer;
	aux_buffer= malloc(MAX_BUFFER);
	t_struct_respuesta_umv buffer;
	int procesoDelHilo,unaBase,unOffset,unTamanio;
	puts("\nIngrese operacion a ejecutar (operacion, retardo, algoritmo, compactacion, dump y exit para salir)");
	scanf("%s",&comando);
	while(estaEnDicOP(comando)== 0){
		puts("\nOperacion erronea, escriba la operacion de nuevo");
		scanf("%s",&comando);
	}

	while(strcmp(comando, "exit") != 0){
			if(strcmp(comando, "operacion") == 0){
				char tipoOperacion[32];
				puts("\nDesea solicitar posicion de memoria (solicitar) o escribir buffer por teclado (escribir) o crear segmento de programa (crear)o destruir segmento de programa (destruir)?");
				scanf("%s",&tipoOperacion);
				while(estaEnDicTOP(tipoOperacion)== 0){
						puts("\nTipo de Operacion erronea, escriba el tipo de operacion de nuevo");
						scanf("%s",&tipoOperacion);
					}
				if(strcmp(tipoOperacion, "solicitar") == 0){
					  puts("\nIngrese Base");
					  scanf("%d",&unaBase);
					  puts("\nIngrese Offset");
					  scanf("%d",&unOffset);
					  puts("\nIngrese Tamanio de segmento");
					  scanf("%d",&unTamanio);
					  buffer = solicitarBytes(unaBase,unOffset,unTamanio);
				}
				if(strcmp(tipoOperacion, "escribir") == 0){
					 puts("\nIngrese Base");
					 scanf("%d",&unaBase);
					 puts("\nIngrese Offset");
					 scanf("%d",&unOffset);
					 puts("\nIngrese Buffer");
					 scanf("%s",aux_buffer);
					 unTamanio=strlen(aux_buffer)+1;
					 printf("El tamanio es: %d\n",unTamanio);
					 enviarBytes(unaBase,unOffset,unTamanio,aux_buffer);

				}
				if(strcmp(tipoOperacion, "crear") == 0){
					  puts("\nIngrese el processID de programa a usar");
				 	  scanf("%d",&procesoDelHilo);
					  puts("\nIngrese el tamaño del segmento");
					  int tamanio;
					  scanf("%d",&tamanio);
					  crearSegmentoPrograma(procesoDelHilo,tamanio);
				}
				if(strcmp(tipoOperacion, "destruir") == 0){
					  puts("\nIngrese el processID de programa a usar");
					  scanf("%d",&procesoDelHilo);
					  destruirSegmentos(procesoDelHilo);
				}
			}

	else { if (strcmp(comando, "retardo") == 0){
				  puts("\nIngrese el valor de retardo en Milisegundos");
				  int valorRetardo;
				  scanf("%d", &valorRetardo);
				  retardo= valorRetardo/1000;
				  escribir_log(archLog, "Se cambia el valor del retardo", INFO, "");
			   }
			   if (strcmp(comando, "algoritmo") == 0){
				   algoritmo();
			   }
			   if (strcmp(comando, "compactacion") == 0){
				   compactar();
			   }
			   if (strcmp(comando,"dump") ==0){
				   dump();
			   }
			}
	    puts("\nEscriba la siguiente operacion");
		scanf("%s",&comando);
		while(estaEnDicOP(comando)== 0){
			puts("\nOperacion erronea, escriba la operacion de nuevo");
			scanf("%s",&comando);
			}

		}
		if(strcmp(comando,"exit") ==0){
			destruirTodosLosSegmentos();
			free(MP);
			free(tablaDeSegmentos);
			pthread_mutex_destroy(mutex_MP);
			pthread_mutex_destroy(mutex_log);
			pthread_mutex_destroy(mutex_pid);
		   	socket_cerrarConexion(sock_servidor);
			escribir_log(archLog, "Se cierra forzosamente la UMV",INFO,"");
		   	if(pthread_kill(CONEXIONES,0)==0) printf("Muere el hilo de Conexiones\n");
			sleep(retardo);
		}
}


void destruirTodosLosSegmentos(void){
	int i=0;
	while(i<cant_tablas){
	free(tablaDeSegmentos[i].segmentos);
		i++;
	}
	return;
}





