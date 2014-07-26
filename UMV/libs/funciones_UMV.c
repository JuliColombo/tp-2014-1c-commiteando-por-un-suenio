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

t_buffer solicitarBytes(int base,int offset, int longitud){
	t_buffer buffer;
	if(!segmentationFault(base, offset, longitud)){
	buffer = malloc((longitud+1)*sizeof(char));
	int j;
	j=traducirPosicion(base)+offset;
	printf("La posicion real es: %d\n",j);
	memcpy(buffer, (char *) &MP[j], longitud);
	printf("El buffer solicitado es: %s\"\n",(char*)buffer); //TODO: Cuando este funcionando, reemplazar por imprimirBuffer(t_buffer)
	escribir_log(archLog, "Se realiza una solicitud de bytes", INFO, "La solicitud tiene exito");
	sleep(retardo);
	return buffer;
	} else {
		printf("Seg fault\n");
		return NULL;
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
		if (!segmentationFault(base,offset,longitud)){
			aux=traducirPosicion(base);
			if(aux==-1){
							printf("La direccion base es erronea\n");
							escribir_log(archLog, "Se realiza un envio de bytes", ERROR, "La direccion base es erronea");
							sleep(retardo);
							return -1;
						}
			j=traducirPosicion(base)+offset;
			printf("La posicion Virtual es: %d y la Real es : %d\n", base, j);
			printf("El resultado de la asignacion es:\n");
			printf("%s\n",(char*)buffer);//TODO: Cuando este funcionando, reemplazar por imprimirBuffer(t_buffer)
			memcpy(&MP[j], (int*) buffer, longitud);
			escribir_log(archLog, "Se realiza envio de bytes", INFO, "El envio tiene exito");
			return 0;
			} else {
				printf("Seg fault\n");
				sleep(retardo);
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

	sleep(retardo);
}






//****************************************Compactacion*****************************************

void compactar(){
	int posicionFinal=0;
	int i,j,k,l=0;
	int posicionSegmento,tamanio;
	//Recorre la MP
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
			printf("Compactacion finalizada\n");
			escribir_log(archLog, "Se termina de realizar la compactacion", INFO, "");
			sleep(retardo);
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

void imprimirBuffer(t_buffer buffer){
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
}

void imprimirBufferEnArchivo(t_buffer buffer,FILE* archivo){
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
	int getEspacioLibreMP(void);
	void imprimirBuffer(t_buffer);
	void imprimirBufferEnArchivo(t_buffer,FILE*);

	archivo_MP = fopen("/home/utnso/dump_file_MP", "w");
	archivo_TS = fopen("/home/utnso/dump_file_TS", "w");
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
	printf("\nEl espacio libre en la memoria principal es: \n");
	printf("%d",espacioLibre);
	fprintf(archivo_MP,"%s", "\nEl espacio libre en la memoria principal es:\n");
	fprintf(archivo_MP,"%d",espacioLibre);

	//Contenido de la memoria principal
	int offset,tamanio;
	t_buffer buffer;
	printf("\nIngrese el offset con la posicion de MP a conocer y la cantidad de bytes a leer\n");
	scanf("%d", &offset);
	scanf("%d", &tamanio);
	buffer = malloc((tamanio+1)*sizeof(char));
	memcpy(buffer, (char*) &MP[offset], tamanio);
	printf("\nLa posicion de memoria %d contiene: \n", offset);
	imprimirBuffer(buffer);
	fprintf(archivo_MP, "\nLa posicion de memoria %d contiene: \n", offset);//Ojo que pongo archivo_MP pero capaz deberia ser en otro
	imprimirBufferEnArchivo(buffer,archivo_MP); //Revisar si en archivo_MP, capaz tendria que ser en otro

	//imprimirEstadoMP(archivo_MP);//-Ya no deberia ir no?-Va escribiendo en el archivo el contenido de las posiciones de la MP

	escribir_log(archLog, "Se realiza un dump", INFO, "El dump se realiza con exito");
	fclose(archivo_MP);
	fclose(archivo_TS);
	sleep(retardo); //O dejarlo hasta que apreten algo
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
	//Escoge la ubicacion en base al algoritmo de config
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
		compactar();
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
			escribir_log(archLog, "Se trata de crear un segmento [MEMORY OVERLOAD]:", ERROR, "No hay espacio para reservar en memoria");
			sleep(retardo);
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
	escribir_log(archLog, "Se trata de crear un segmento", INFO, "El segmento se crea con exito");
	sleep(retardo);
	return 0;
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
		//FIXME: no funciona bien este realloc
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
			//FIXME: no funciona bien este realloc
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

void destruirSegmentosPrograma(int id_prog){
	int pos= getPosTabla(id_prog);
	liberarMP(pos);
	eliminarSegmentos(pos);
	pthread_mutex_lock(mutex_log);
	log_escribir(archLog, "Se destruyen segmentos de un programa", INFO, "");
	pthread_mutex_unlock(mutex_log);
	printf("Segmentos del programa %d destruidos con exito",id_prog);
	sleep(retardo);
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

void ejecutar(t_tipoEstructura tipo_estructura,void* estructura){
		int baseStack; //Tendria que ser global y creada con la conexion del kernel?
		int base;
		t_signal senial;
		t_tipoEstructura tipoRecibido;
		void* structRecibida;
		t_struct_push* structPush;
		t_struct_pop* structPop;
		t_struct_instruccion* structInstr;
		t_struct_seg_codigo* structCodigo;
		socket_recibir(sock_cpu,&tipoRecibido,&structRecibida);
		switch(tipoRecibido){
		case D_STRUCT_PUSH:
			structPush= ((t_struct_push*)structRecibida);
			int pos= structPush->posicion;
			int valor= structPush->valor;
			baseStack = structPush->stack_base;
			if(enviarBytes(baseStack,pos,sizeof(valor),(int*)valor)==0){
				//signaltodopiola
				senial = D_STRUCT_NORMAL;
				socket_enviarSignal(sock_cpu,senial);
			}else{
				//signaltodomal
				senial = D_STRUCT_SEGFAULT;
				socket_enviarSignal(sock_cpu,senial);
			}
			free(structRecibida);
			break;			//Revisar bien los tipos del valor (int,t_buffer,void*) y como manejarlos

		case D_STRUCT_POP:
			structPop= ((t_struct_pop*)structRecibida);
			pos= structPop->posicion;
			int tamanio = structPop->tamanio;
			baseStack = structPop->stack_base;

			//ACA VENDRIA UN IF CHEQUEANDO QUE SE PUEDE HACER LO QUE LA CPU ME PIDE Y MANDAN SIGNAL

			t_buffer valor_a_enviar = solicitarBytes(baseStack,pos,tamanio);
			t_struct_numero* estructura = malloc(sizeof(t_struct_numero));
			estructura->numero = valor_a_enviar;
			socket_enviar(sock_cpu, D_STRUCT_NUMERO, estructura);
			free(estructura);
			break;
		case D_STRUCT_INSTRUCCION:
			structInstr=(t_struct_instruccion*)structRecibida;
			base=structInstr->indice_codigo; //El segmento almacenado que en realidad es indice de codigo
			pos=structInstr->inst;//La instruccion correspondiente
			//tamanio=2*sizeof(int); //Tamaño fijo de cada lugar en el indice de codigo (4(indice)+4(longitud) bytes)
			//long long int posDelIndice; //Tiene que ser de 8 bytes (Pag 16 enunciado)
			//posDelIndice=solicitarBytes(base,pos,tamanio);
			//t_struct_seg_codigo* segAEnviar;


			tamanio = sizeof(int);
			int start = solicitarBytes(base,pos,tamanio); //busco el start de la instruccion que voy a mandar

			pos += sizeof(int);
			int offset = solicitarBytes(base,pos,tamanio); //busco el offset. Para eso, tengo que correr 4 bytes la posicion, porque sino
															//les devuelvo nuevamente el start

			t_intructions instruccion;
			instruccion.start=start;
			instruccion.offset=offset;

			t_struct_seg_codigo* estruc = malloc(sizeof(t_struct_seg_codigo));
			estruc->inst = instruccion;
			estruc->seg_codigo = 0;//No uso el numero,asi que me mandan cualquier cosa
			socket_enviar(sock_cpu, D_STRUCT_SEGCODIGO, estruc);
			free(estruc);

			//segAEnviar->inst=primeros4Bytes(posDelIndice);
			//int longitudDeInstr=segundos4Bytes(posDelIndice); //Esta variable se va a usar en el pedido del codigo
			//socket_enviar(sock_cpu,D_STRUCT_SEGCODIGO, segAEnviar);
			//-------------------Aca se espera la devolucion del seg o es otro case? Preg a juli

			break;

		case D_STRUCT_SEGCODIGO:
			structCodigo = (t_struct_seg_codigo*)structRecibida;
			t_intructions inst = structCodigo->inst;
			base = structCodigo->seg_codigo;
			char* linea = solicitarBytes(base,inst.start,inst.offset);

			//CHEQUEAN QUE SE PUEDE HACER LO QUE PIDE LA CPU CON EL IF

			t_struct_string* structure = malloc(sizeof(t_struct_string));
			structure->string = linea;

			int j=socket_enviar(sock_cpu,D_STRUCT_STRING,estructura);
			if(j == 1){
				free(structure);
			}


			break;
		default:
			escribir_log(archLog, "Solicitud de CPU", ERROR, "Solicitud no reconocida");
			break;
		//Etc

		}
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
				pthread_create(&atender_pedido, NULL, (void*) &atender_kernel, sock);
				break;
			case 1:
				numeroEnviado->numero=1;
				socket_enviar(sock_aceptado, D_STRUCT_NUMERO, numeroEnviado);
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




void core_conexion_cpu(void){
	int sock;
	pthread_t atender_pedido;

	/*/if((sock_cpu=socket_crearServidor("127.0.0.1", configuracion_UMV.puerto_cpus))>0){
	printf("Hilo de CPU \n");
	escribir_log(archLog, "Escuchando en el socket de CPU's", INFO, "");
	}*/ 	//TODO ESTO NO VA

	while(1){
		if((sock=socket_aceptarCliente(sock_cpu))>0){
			printf("Acepta conexion");
			escribir_log(archLog, "Se acepta la conexion de una CPU", INFO, "");
			pthread_create(&atender_pedido, NULL, (void*) &atender_cpu, NULL);	//Crea un hilo para atender cada conexion de cpu

			//RECIBO PEDIDO DE INDICE DE ETIQUETAS
			t_tipoEstructura tipoRecibido;
			void* structRecibida;
			t_struct_indice_etiquetas* indice;
			socket_recibir(sock, &tipoRecibido,&structRecibida);
			indice = ((t_struct_indice_etiquetas*)structRecibida);
			//printf("me llego esto %d y %d\n",tamanio->etiquetas_size, *tamanio->index_etiquetas);

			t_struct_string* estructura = malloc(sizeof(t_struct_indice_etiquetas));
			estructura->string = "hola wachada";		//ACA MANDAN EL INDICE DE ETIQUETAS EN BASE A LO RECIBIDO

			int j=socket_enviar(sock,D_STRUCT_STRING,estructura);
			if(j == 1){
			printf("se envio el intento de indice de etiquetas\n");
			free(estructura);
			}
		}
	}
		if(socket_cerrarConexion(sock_cpu)==0){
				escribir_log(archLog, "Se trata de cerrar el socket de CPU", ERROR, "Hay problemas para cerrar el socket");
				//Error cerrando el socket
			} else {
				escribir_log(archLog, "Se cierra el socket de CPU", INFO, "No hay problemas para cerrar el socket");
			}
	return;

}


void atender_cpu(sock_struct* sock){
	pthread_detach(pthread_self());
	/*UNSOLVED:
	  int programaEnHilo;
	  void* estructura;
	  t_tipoEstructura tipo_estructura;
	  socket_recibir(sock, &tipo_estructura, &estructura);
	  ejecutar(&tipo_estructura, &estructura);		//ejecutaria lo correspondiente y crearia la estructura a enviar
	  send(sock, &tipo_estructura, &estructura); //Esto no deberia ir, que se envie durante la ejecucion
	  free(sock);
	 */

}




/*void core_conexion_kernel(void){
	if((sock_kernel_servidor=socket_crearServidor("127.0.0.1",configuracion_UMV.puerto_kernel))>0){
	printf("Hilo de Kernel\n");
	escribir_log(archLog, "Escuchando en el socket de Kernel", INFO, "");
	}
	int sock_aceptado;
	if((sock_aceptado=socket_aceptarCliente(sock_kernel_servidor))>0){
			printf("Acepta conexion");
			escribir_log(archLog, "Conexion", INFO, "Se acepta conexion del kernel");
		}
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	socket_recibir(sock_aceptado, &tipoRecibido, &structRecibida);
	if(tipoRecibido==D_STRUCT_NUMERO){
		tamanioMaxStack = ((t_struct_numero*)structRecibida)->numero;
		free(structRecibida);

	}
		while(1){

			atender_kernel(sock_aceptado);

		}
	if(socket_cerrarConexion(sock_kernel_servidor)==0){
		escribir_log(archLog, "Se trata de cerrar el socket de Kernel", ERROR, "Hay problemas para cerrar el socket");
		//Error cerrando el socket
	} else {
		escribir_log(archLog, "Se cierra el socket de Kernel", INFO, "No hay problemas para cerrar el socket");
	}

	return;
}*/

void atender_kernel(sock_struct* sock){
	pthread_detach(pthread_self());
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	t_struct_memoria* tamanio;
	int i,id_prog,memoriaSuficiente=0;
	int tamanioSolicitado;

	socket_recibir(sock->fd, &tipoRecibido, &structRecibida);
	tamanioMaxStack = ((t_struct_numero*)structRecibida)->numero;
	free(structRecibida);


	while(1){

		socket_recibir(sock->fd, &tipoRecibido, &structRecibida);
		if(tipoRecibido==D_STRUCT_NUMERO){
			t_struct_numero* id = ((t_struct_numero*)structRecibida);
			id_prog = id->numero;
			free(id);
		}
		socket_recibir(sock->fd, &tipoRecibido,&structRecibida);
		if(tipoRecibido==D_STRUCT_SOLICITARMEMORIA){
			tamanio = ((t_struct_memoria*)structRecibida);


			memoriaSuficiente = crearSegmentoPrograma(id_prog, tamanioMaxStack);
			if(memoriaSuficiente==0){
				tamanioSolicitado = tamanio->tamanioScript;
				memoriaSuficiente = crearSegmentoPrograma(id_prog, tamanioSolicitado);
				if(memoriaSuficiente==0){
					tamanioSolicitado = tamanio->tam2;
					memoriaSuficiente = crearSegmentoPrograma(id_prog, tamanioSolicitado);
					if(memoriaSuficiente==0){
						tamanioSolicitado = tamanio->tam3;
						memoriaSuficiente = crearSegmentoPrograma(id_prog, tamanioSolicitado);
					}
				}
			}

		}
		t_struct_numero* respuesta= malloc(sizeof(t_struct_numero));
		if (memoriaSuficiente==-1){
			escribir_log(archLog, "Memoria insuficiente", ERROR, "No hay memoria suficiente para el programa");
			destruirSegmentosPrograma(id_prog);
			respuesta->numero=1;
			socket_enviar(sock->fd, D_STRUCT_SF, respuesta);
		}
		if(memoriaSuficiente==0){
			respuesta->numero=memoriaSuficiente;
			socket_enviar(sock->fd, D_STRUCT_NUMERO, respuesta);
			if(memoriaSuficiente==0){
				for(i=0;i<4;i++){
					//socket_recibir(sock->fd,&tipoRecibido,&structRecibida);
					//ACA IRIAN LOS SEGMENTOS DE CODIGO PARA GRABAR LOS BYTES
					//enviarBytes(base,offset,longitud,buffer);
				}
			}
		}
		free(respuesta);
	}


	free(sock);
	return;
}


//***********************************************Inicializacion de semaforos************************************


void inicializarMutex(void){
	mutex=malloc(sizeof(pthread_mutex_t));
	mutex_log=malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex,NULL);
	pthread_mutex_init(mutex_log, NULL);
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
	int procesoDelHilo,unaBase,unOffset,unTamanio;
	t_buffer buffer;
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
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					  aux_buffer = solicitarBytes(unaBase,unOffset,unTamanio);
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
				if(strcmp(tipoOperacion, "escribir") == 0){
					 puts("\nIngrese Base");
					 scanf("%d",&unaBase);
					 puts("\nIngrese Offset");
					 scanf("%d",&unOffset);
					 puts("\nIngrese Buffer");
					 scanf("%s",aux_buffer);
					 unTamanio=strlen(aux_buffer+1);
					 printf("El tamanio es: %d\n",unTamanio);
					 pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					 enviarBytes(unaBase,unOffset,unTamanio,aux_buffer);
					 pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida

				}
				if(strcmp(tipoOperacion, "crear") == 0){
					    puts("\nIngrese el processID de programa a usar");
				 	  scanf("%d",&procesoDelHilo);
					  puts("\nIngrese el tamaño del segmento");
					  int tamanio;
					  scanf("%d",&tamanio);
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					  crearSegmentoPrograma(procesoDelHilo,tamanio);
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
				if(strcmp(tipoOperacion, "destruir") == 0){
					  puts("\nIngrese el processID de programa a usar");
					  scanf("%d",&procesoDelHilo);
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					  destruirSegmentosPrograma(procesoDelHilo);
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
			}

	else { if (strcmp(comando, "retardo") == 0){
				  puts("\nIngrese el valor de retardo en Milisegundos");
				  int valorRetardo;
				  scanf("%d", &valorRetardo);
				  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
				  retardo= valorRetardo/1000;
				  escribir_log(archLog, "Se cambia el valor del retardo", INFO, "");
				  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
			   }
			   if (strcmp(comando, "algoritmo") == 0){
				   pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
				   algoritmo();
				   pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
			   }
			   if (strcmp(comando, "compactacion") == 0){
				   pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
				   compactar();
				   pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
			   }
			   if (strcmp(comando,"dump") ==0){
				   pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
				   dump();
				   pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
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
		   	socket_cerrarConexion(sock_servidor);
		   	socket_cerrarConexion(sock_cpu);
		   	matarHilos();
			if(pthread_kill(CPU,0)==0) printf("Muere el hilo cpu\n");
			if(pthread_kill(CONEXIONES,0)==0) printf("Muere el hilo de Conexiones\n");
			sleep(retardo);
			system("clear");
		}
}

void matarHilos(void){
	pthread_cancel(CPU);
	pthread_cancel(CONEXIONES);

}

void destruirTodosLosSegmentos(void){/*
	int i=0;
	while(i<cant_tablas){
	free(tablaDeSegmentos[i].segmentos);
	}
	return;*/
}





