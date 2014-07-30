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


int* crearMP(void) { // Cambie para que no reciba parametro, total la config es una variable externa -- Fede

	tamanioMP = configuracion_UMV.memSize;
	int* aux_MP;
	aux_MP = malloc(tamanioMP*(sizeof(t_memoria_principal)));
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

int validacionSegFault(int base, int offset,int longitud){
	int pos=traducirPosicion(base);
	int numTabla=ubicarEnTabla(pos);
	int numSeg=ubicarSegmentoEnTabla(pos);
	printf("El numero de tabla es: %d y el numero de segmento es: %d\n", numTabla, numSeg);
	if (tablaDeSegmentos[numTabla].segmentos[numSeg].tamanio >= (offset+longitud)){
		return 0;
	} else {
	return 1;
	}
}

int segmentationFault(int base,int offset,int longitud){
	if (validacionSegFault(base,offset,longitud) ) {
		escribir_log(archLog, "Segmentation Fault", ERROR, "Segmentation fault al intentar acceder a la posicion");
		return 1;
	} else return 0;
}



// ***********************************Solicitar bytes en memoria*******************

t_struct_buffer solicitarBytes(int base,int offset, int longitud){
	t_struct_buffer respuesta;
	pthread_mutex_lock(mutex_MP);
	if(!segmentationFault(base, offset, longitud)){
	void* buffer=malloc(longitud);
	int j;
	j=traducirPosicion(base)+offset;
	printf("La posicion real es: %d\n",j);
	memcpy(buffer,  &MP[j], longitud);
	respuesta.buffer=buffer;
	respuesta.tamanio=longitud;
	pthread_mutex_unlock(mutex_MP);
	printf("El buffer solicitado es: %s\n",(char*)buffer); //TODO: Cuando este funcionando, reemplazar por imprimirBuffer(t_buffer)
	escribir_log(archLog, "Se realiza una solicitud de bytes", INFO, "La solicitud tiene exito");
	return respuesta;
	} else {
		void* buffer_fallo=malloc(sizeof(int));
		int valor=-1;
		memcpy(buffer_fallo,&valor,sizeof(int));
		respuesta.buffer=buffer_fallo;
		respuesta.tamanio=sizeof(int);
		pthread_mutex_unlock(mutex_MP);
		printf("Seg fault\n");
		return respuesta;
	}
}

int traducirPosicion(int base){
	int i=0,j;
	while(i<cant_tablas){
		j=0;
		while(j<tablaDeSegmentos[i].cant_segmentos){
			if(tablaDeSegmentos[i].segmentos[j].inicio == base){
						return tablaDeSegmentos[i].segmentos[j].ubicacionMP;
					} else {
						j++;
					}
		}
		i++;
	}
	log_escribir(archLog, "Solicitar memoria", ERROR, "Posicion erronea");
	return -1;
}

//****************************************enviarBytes*************************************


int enviarBytes(int base,int offset,int longitud,t_buffer buffer){
	int j,aux;
	pthread_mutex_lock(mutex_MP);
		if (!segmentationFault(base,offset,longitud)){
			aux=traducirPosicion(base);
			if(aux==-1){
							printf("La direccion base es erronea\n");
							escribir_log(archLog, "Se realiza un envio de bytes", ERROR, "La direccion base es erronea");
							return -1;
						}
			j=traducirPosicion(base)+offset;
			printf("La posicion Virtual es: %d y la Real es : %d\n", base, j);
			printf("El resultado de la asignacion es:\n");
			printf("%s\n",(char*)buffer);//TODO: Cuando este funcionando, reemplazar por imprimirBuffer(t_buffer)
			memcpy(&MP[j], (int*) buffer, longitud);
			pthread_mutex_unlock(mutex_MP);
			escribir_log(archLog, "Se realiza envio de bytes", INFO, "El envio tiene exito");
			return 0;
			} else {
				pthread_mutex_unlock(mutex_MP);
				printf("Seg fault\n");
				return -1;
			}
}






/*************************Handshake*************************/

void hacerHandshake(tipo_handshake tipo){
	if(lista_handshakes.cantidad == 0){
		inicializarYAgregar(tipo);
	} else { agregarHandshake(tipo);
	  }
}

void inicializarListaHandshakes(void){
	lista_handshakes.cantidad=0;
	lista_handshakes.handshakes=NULL;
}


void inicializarYAgregar(tipo_handshake tipo){
	tipo_handshake* aux_lista;
	tipo_handshake aux_handshake;
	aux_lista = malloc(sizeof(tipo_handshake));
	if(aux_lista==NULL){
		log_escribir(archLog, "Error en la lista de handshakes", ERROR, "No hay memoria suficiente");
		abort();
	}
	lista_handshakes.cantidad ++;
	aux_handshake = tipo;
	lista_handshakes.handshakes[0] = aux_handshake;
}

void agregarHandshake(tipo_handshake tipo){
	tipo_handshake* aux_lista;
	tipo_handshake aux_handshake;
	int aux=lista_handshakes.cantidad;
	aux_lista = realloc(lista_handshakes.handshakes,(sizeof(lista_handshakes.handshakes)+sizeof(tipo_handshake)));
	if(aux_lista==NULL){
		log_escribir(archLog, "Error en la lista de handshakes", ERROR, "No hay memoria suficiente");
		abort();
	}
	lista_handshakes.handshakes = aux_lista;
	aux_handshake=tipo;
	lista_handshakes.handshakes[aux] = aux_handshake;
	lista_handshakes.cantidad++;
	return;
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
	int posicionFinal=0;
	int i,j,k,l=0;
	int posicionSegmento,tamanio;
	//Recorre la MP
	pthread_mutex_lock(mutex_MP);
	while(l<tamanioMP){
	//Encuentra la primera posicion libre
	while(MP[posicionFinal]!=NULL){
		posicionFinal++;
	}
	printf("La posicion a reubicar es: %d\n", posicionFinal);
	posicionSegmento=posicionFinal;
		//Encuentra la primera posicion ocupada
	while(MP[posicionSegmento]==NULL && posicionSegmento<tamanioMP) posicionSegmento++;
		if(posicionSegmento == tamanioMP){
			pthread_mutex_unlock(mutex_MP);
			printf("Compactacion finalizada\n");
			escribir_log(archLog, "Se termina de realizar la compactacion", INFO, "");
			return;
		}
		printf("La posicion de inicio del segmento es: %d\n", posicionSegmento);
		i=ubicarEnTabla(posicionSegmento);
		printf("La tabla correspondiente es: %d\n",i);
		j=ubicarSegmentoEnTabla(posicionSegmento);
		printf("El segmento correspondiente es: %d\n",j);
		tamanio=tablaDeSegmentos[i].segmentos[j].tamanio;
		k=posicionFinal;
		//Traslado el segmento
	while(tamanio>0){
			MP[k]=MP[posicionSegmento];
			MP[posicionSegmento]=NULL;
			k++;
			posicionSegmento++;
			tamanio--;
	}
	tablaDeSegmentos[i].segmentos[j].ubicacionMP = posicionFinal;
	posicionFinal = posicionFinal+tamanio;
	l++;
	}
}

int ubicarEnTabla(int posicionR){
	int i=0,j;
	while(i<cant_tablas){
		j=0;
		while(j<tablaDeSegmentos[i].cant_segmentos){
			if(tablaDeSegmentos[i].segmentos[j].ubicacionMP == posicionR){
				return i;
			} else {
			j++;
				}
		}
		i++;
	}
	return -1;
}



int ubicarSegmentoEnTabla(int posicionR){
	int i=0,j;
	while(i<cant_tablas){
		j=0;
		while(j<tablaDeSegmentos[i].cant_segmentos){
			if(tablaDeSegmentos[i].segmentos[j].ubicacionMP == posicionR){
							return j;
				}
			j++;
		}
		i++;
	}
	return -1;
}

int getEspacioLibreMP(void){
	int libre=0;
	int i=0;
	while (i<tamanioMP){
		if (MP[i] == NULL) libre++;
		i++;
	}
	return libre;
}

void imprimirBuffer(t_buffer buffer){/*
	int tamanioBuffer= sizeof(buffer);
	int i=0;
	while (i<tamanioBuffer){
		if(buffer[i] != NULL){
		printf("%c", (char)buffer[i]);
		}	else{
			printf(" (NULL) ");
		}
		i++;
	}
	printf("\n");
*/}

void imprimirBufferEnArchivo(t_buffer buffer,FILE* archivo){/*
	int tamanioBuffer= sizeof(buffer);
	int i=0;
	while (i<tamanioBuffer){
		if(buffer[i] != NULL){
		fprintf(archivo,"%c",(char)buffer[i]);
		} else{
			fprintf(archivo," (NULL) ");
		}
		i++;
	}
	fprintf(archivo,"\n");
*/}

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
	int getEspacioLibreMP(void);
	void imprimirBuffer(t_buffer);
	void imprimirBufferEnArchivo(t_buffer,FILE*);

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
	puts("\nIngrese el numero de proceso del cual se quiere conocer sus segmentos o '-1' para verlos todos");
	scanf("%d",&procesoAVer);
	if(procesoAVer == -1){
		imprimirEstadoTablaSeg(archivo_TS,0,cant_tablas);
	}else {
		int ubicacion= getPosTabla(procesoAVer);
		int tablaFinal=ubicacion+1;
		imprimirEstadoTablaSeg(archivo_TS,ubicacion,tablaFinal);
	}

	//Memoria Principal

	//Segmentos de los programas TODO

	//Espacio libre

	int espacioLibre= getEspacioLibreMP();
	printf("\nEl espacio libre en la memoria principal es: %d\n",espacioLibre);
	fprintf(archivo_MP,"El espacio libre en la memoria principal: %d\n",espacioLibre);

	//Contenido de la memoria principal
	int offset,tamanio;
	t_buffer buffer;
	printf("\nIngrese el offset con la posicion de MP a conocer y la cantidad de bytes a leer\n");
	scanf("%d", &offset);
	scanf("%d", &tamanio);
	buffer = malloc((tamanio+1)*sizeof(char));
	memcpy((char*)buffer, (char*) &MP[offset], tamanio);
	printf("\nLa posicion de memoria %d contiene: %s\n", offset, (char*)buffer);
	fprintf(archivo_MP, "La posicion de memoria %d contiene: %s\n", offset, (char*)buffer);//Ojo que pongo archivo_MP pero capaz deberia ser en otro

	//imprimirEstadoMP(archivo_MP);//-Ya no deberia ir no?-Va escribiendo en el archivo el contenido de las posiciones de la MP
	pthread_mutex_unlock(mutex_MP);
	escribir_log(archLog, "Se realiza un dump", INFO, "El dump se realiza con exito");
	fclose(archivo_MP);
	fclose(archivo_TS);
}

void imprimirEstadoMP(FILE* archivo){
	int i=0;
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
		}
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


int validarPosicionVirtual(int posVirtual) {
	int i=0,j=0;
	while(i<cant_tablas){
		while(j<tablaDeSegmentos[i].cant_segmentos){
			if(tablaDeSegmentos[i].segmentos[j].inicio == posVirtual){
						return 0;
					} else {
						j++;
					}
		}
		i++;
	}
	return 1;
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
	int ubicacion=-1;
	segmentDescriptor aux;
	int i,num_segmento;
	if(tamanio==0){
				escribir_log(archLog,"Se trata de crear un segmento",INFO,"El tamanio es 0");
				printf("El tamanio es: %d\n",tamanio);
				ubicacion=TAMANIO_NULO;
				printf("Devuelve: %d\n",ubicacion);
				return ubicacion;
	} else {
	//Escoge la ubicacion en base al algoritmo de config
	pthread_mutex_lock(mutex_MP);
	if(configuracion_UMV.algoritmo == firstfit){
		escribir_log(archLog,"Se selecciona ubicacion",INFO,"El algoritmo de seleccion es firstfit");
		ubicacion = escogerUbicacionF(tamanio);
	} else{
		if(configuracion_UMV.algoritmo == worstfit){
			escribir_log(archLog,"Se selecciona ubicacion",INFO,"El algoritmo de seleccion es firstfit");
			ubicacion = escogerUbicacionW(tamanio);
		}
	}
	if(ubicacion==-1){
	pthread_mutex_unlock(mutex_MP);
		compactar();
	pthread_mutex_lock(mutex_MP);
	}
	if(configuracion_UMV.algoritmo == firstfit){
			ubicacion = escogerUbicacionF(tamanio);
		} else{
			if(configuracion_UMV.algoritmo == worstfit){
				printf("Va a elegir por Worst");
				ubicacion = escogerUbicacionW(tamanio);
				printf("La ubicacion es: %d", ubicacion);
			}
		}
	if(ubicacion==-1){
			pthread_mutex_unlock(mutex_MP);
			escribir_log(archLog, "Se trata de crear un segmento [MEMORY OVERLOAD]:", ERROR, "No hay espacio para reservar en memoria");
			return -1;
		}
	reservarEspacioMP(ubicacion, tamanio);
	int pos=inicializarTabla(id_prog);
	i=rand();
	while(!validarPosicionVirtual(i)) rand();//-Lei que esto es innecesario, pero bueno,ya esta- Recorrer la tabla de segmentos validando que ninguno ocupe entre la posicion y la posicion y el tamanio
	//Armado del segmento creado
	aux.ubicacionMP=ubicacion;
	aux.inicio=i;
	aux.tamanio=tamanio;
	//Asignación de los campos de la tabla de segmentos correspondiente
	tablaDeSegmentos[pos].id_prog = id_prog;
	num_segmento=tablaDeSegmentos[pos].cant_segmentos;
	tablaDeSegmentos[pos].cant_segmentos++;
	tablaDeSegmentos[pos].segmentos[num_segmento]=aux;
	printf("El programa es : %d\n", tablaDeSegmentos[pos].id_prog);
	printf("La cantidad de segmentos es: %d\n", tablaDeSegmentos[pos].cant_segmentos);
	printf("La posicion real es : %d\n", tablaDeSegmentos[pos].segmentos[num_segmento].ubicacionMP);
	printf("La posicion virtual es : %d\n", tablaDeSegmentos[pos].segmentos[num_segmento].inicio);
	printf("El tamanio es : %d\n", tablaDeSegmentos[pos].segmentos[num_segmento].tamanio);
	pthread_mutex_unlock(mutex_MP);
	escribir_log(archLog, "Se trata de crear un segmento", INFO, "El segmento se crea con exito");
	return tablaDeSegmentos[pos].segmentos[num_segmento].inicio;
	}
}

void reservarEspacioMP(int ubicacion, int tamanio){
	int i=0;
	int aux=tamanio;
	while(i<=tamanioMP && aux>0){
			MP[ubicacion+i]=-1;
			aux--;
			i++;
		}
}

int inicializarTabla(int id_prog){
	int i=0;
	tablaSeg* aux_tabla;
	segmentDescriptor* aux_segmentos;
	//Verifico si hay tablas

	if(cant_tablas==0){
		//Como no hay tablas, la inicializo y asigno la cantidad de segmentos a 0
		aux_tabla = malloc(sizeof(tablaSeg));
			if(aux_tabla==NULL){
				log_escribir(archLog, "Error en la tabla de segmentos", ERROR, "No hay memoria suficiente");
				abort();
			}
		tablaDeSegmentos = aux_tabla;
		tablaDeSegmentos[i].cant_segmentos=0;
		aux_segmentos = malloc(sizeof(segmentDescriptor));
						if(aux_segmentos==NULL){
							log_escribir(archLog, "Error en la tabla de segmentos", ERROR, "No hay memoria suficiente");
							abort();
						}
		tablaDeSegmentos[i].segmentos = aux_segmentos;
		cant_tablas++;
		printf("La cant de tablas es: %d\n", cant_tablas);
		return i;
	}
	else{
		//Como hay tablas, busco si ya hay una para el prog correspondiente
	while (tablaDeSegmentos[i].id_prog != id_prog && i<= cant_tablas) i++;
	if (tablaDeSegmentos[i].id_prog != id_prog){
		//Si no encuentra una tabla para el programa, agrego una tabla e incremento la cantidad total de tablas
		aux_tabla =realloc(tablaDeSegmentos, (sizeof(tablaSeg)*(cant_tablas+1)));
		if(aux_tabla==NULL){
			log_escribir(archLog, "Error en la tabla de segmentos", ERROR, "No hay memoria suficiente");
			abort();
		}
		tablaDeSegmentos = aux_tabla;
		i=cant_tablas;
		aux_segmentos = malloc(sizeof(segmentDescriptor));
			if(aux_segmentos==NULL){
			log_escribir(archLog, "Error en la tabla de segmentos", ERROR, "No hay memoria suficiente");
			abort();
			}
		tablaDeSegmentos[i].segmentos = aux_segmentos;
		tablaDeSegmentos[i].cant_segmentos=0;
		cant_tablas++;
		printf("La cant de tablas es: %d\n", cant_tablas);
		return i;
		} else {
			aux_segmentos = realloc(tablaDeSegmentos[i].segmentos, ((tablaDeSegmentos[i].cant_segmentos+1)*sizeof(segmentDescriptor)));
									if(aux_segmentos==NULL){
										log_escribir(archLog, "Error en la tabla de segmentos", ERROR, "No hay memoria suficiente");
										abort();
									}
			tablaDeSegmentos[i].segmentos = aux_segmentos;
			printf("La cant de tablas es: %d\n", cant_tablas);
			return i;
		}
	}
}

int escogerUbicacionF(int tamanio){
	int posicionDeDestino;
	int i=0;
	int aux;
			//Recorro la memoria principal
		while(i<tamanioMP){
			 aux=0;			//Obtengo primer posicion libre en MP
			while (MP[i]!=NULL && i<tamanioMP) i++;
			posicionDeDestino= i;
			//Checkeo la cantidad de posiciones libres hasta llegar a
			//una posicion ocupada, o el final de la memoria
			while(MP[i] == NULL && i<tamanioMP){
				aux++;
				i++;
			}
			//Checkeo si la cantidad de posiciones es mayor o igual al tamanio
			//solicitado
			if(aux>=tamanio){
			//Devuelve la posicion de ubicacion del segmento
			return posicionDeDestino;
			}
	}
			//Si llega hasta acá es porque no encontró una posición posible para el segmento
			//Le pongo que retorne -1 para mostrar este error
			return -1;
}

int escogerUbicacionW(int tamanio){
	int posicionDeDestino;
	int max=0;
	//La inicializo en -1, para que al final, si no se encontro una posicion libre
	//retorne este valor de error
	int posicionFinal=-1;
	int i=0;
	//Recorro la memoria principal
	while(i<tamanioMP){
				int aux=0;
			//Obtengo primer posicion libre en MP
				while (MP[i]!=NULL && i<tamanioMP) i++;
				posicionDeDestino= i;
			//Checkeo la cantidad de posiciones libres hasta llegar a
			//una posicion ocupada, o el final de la memoria
				while(MP[i]== NULL && i<tamanioMP){
					aux++;
					i++;
				}
			//Checkeo si la cantidad de posiciones es mayor o igual al tamanio
			//solicitado
				if(aux>=tamanio){
			//Checkeo si esta cantidad es mayor que el maximo numero de posiciones
			//libres encontradas
						if(max>aux){
			//Reasigno el maximo y la posicionFinal
							max=aux;
							posicionFinal=posicionDeDestino;
						}
						posicionFinal=posicionDeDestino;
				}
		}

	//Devuelve la posicionFinal, que valdrá -1 de no encontrar espacio
		return posicionFinal;
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
	int pos= getPosTabla(id_prog);
	pthread_mutex_lock(mutex_MP);
	liberarMP(pos);
	eliminarSegmentos(pos);
	pthread_mutex_lock(mutex_log);
	log_escribir(archLog, "Se destruyen segmentos de un programa", INFO, "");
	pthread_mutex_unlock(mutex_log);
	pthread_mutex_unlock(mutex_MP);
	printf("Segmentos del programa %d destruidos con exito",id_prog);
	return;
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


void liberarMP(int pos){
	int i,ubicacion,tam;
	i=0;
	while(i<tablaDeSegmentos[pos].cant_segmentos){

		//Recorro las posiciones ocupadas de la tabla de segmentos y obtengo la
		//ubicacionMP y tamanio
			ubicacion=tablaDeSegmentos[pos].segmentos[i].ubicacionMP;
			tam=tablaDeSegmentos[pos].segmentos[i].tamanio;
		//Recorro las posiciones de MP en base a ubicacionMP y tamanio
		// y las igualo a NULL
			while(tam!=0){
				MP[ubicacion]=NULL;
				ubicacion++;
				tam--;
			}
			i++;
	}

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
				t_struct_buffer buffer = solicitarBytes(solicitud->base, solicitud->offset, solicitud->tamanio);
				socket_enviar(sock->fd, D_STRUCT_BUFFER, &buffer);
				break;
			case D_STRUCT_ENV_BYTES:
				escritura = (t_struct_env_bytes*) structRecibida;
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
				//log_escibir(archLog,"Termina la ejecucion de CPU",INFO,"");
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
	log_escribir(archLog,"Se recibe el tamanio del stack",INFO,"El tamanio es: %d");
	pthread_mutex_unlock(mutex_log);
	free(structRecibida);


	while(1){
		sleep(retardo);
		socket_recibir(sock->fd, &tipoRecibido,&structRecibida);

		switch(tipoRecibido){
			case D_STRUCT_NUMERO:
				pid = ((t_struct_numero*)structRecibida);
				id_prog = pid->numero;
				escribir_log(archLog,"Se recibe un ID de programa: %d",INFO,id_prog);
				pthread_mutex_lock(mutex_pid);
				cambioProcesoActivo(id_prog);
				pthread_mutex_lock(mutex_log);
				log_escribir(archLog,"Se cambia el proceso activo",INFO,"El pid del proceso activo es: %d");
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
					printf("Stack: %d\n",base_codigo);
					respuesta->indice_codigo=base_index_code;
					printf("Stack: %d\n",base_index_code);
					respuesta->indice_etiquetas=base_index_etiq;
					printf("Stack: %d\n",base_index_etiq);
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
				escribir_log(archLog,"Se recibe peticion de destruccion de segmentos del programa: %d",INFO,pid->numero);
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
				if(struct_seg->tamanio==0){
					escribir_log(archLog,"Se realizo envio de bytes",INFO,"El segmento es de tamanio 0");
				} else {
				enviarBytes(struct_seg->base,0,struct_seg->tamanio,struct_seg->segmento);
				}
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
	t_struct_buffer buffer;
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





