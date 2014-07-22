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

	int numTabla=ubicarEnTabla(base);
	int numSeg=ubicarSegmentoEnTabla(base);
	if (tablaDeSegmentos[numTabla].segmentos[numSeg].tamanio > (offset+longitud)) return 0;
	return 1;
}

int segmentationFault(int base,int offset,int longitud){// TODO Revisar bien esto y el memOverload de abajo
	if (validacionSegFault(base,offset,longitud) ) {
		escribir_log(archLog, "SegmentationFault", ERROR, "Segmentation fault al intentar acceder a la posicion");
		return true;
	} else return false;
}



// ***********************************Solicitar bytes en memoria*******************

t_buffer solicitarBytes(int base,int offset, int longitud){
	t_buffer buffer;
	buffer = malloc((longitud+1)*sizeof(char));
	int j;
	j=traducirPosicion(base)+offset;
	printf("La posicion real es: %d\n",j);
	memcpy(buffer, (char *) &MP[j], longitud);
	printf("El buffer solicitado es: %s\"\n",(char*)buffer);
	escribir_log(archLog, "Se realiza una solicitud de bytes", INFO, "La solicitud tiene exito");
	sleep(retardo);
	return buffer;
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


void enviarBytes(int base,int offset,int longitud,t_buffer buffer){
	int i=0;
	int j,aux;
	while(i<longitud){
		//printf("El valor de la posicion %d del buffer es: %d\n",i,*(int*)buffer[i]);
		i++;
	}
		if (segmentationFault(base,offset,longitud)){
			aux=traducirPosicion(base);
			if(aux==-1){
							printf("La direccion base es erronea\n");
							escribir_log(archLog, "Se realiza un envio de bytes", ERROR, "La direccion base es erronea");
							sleep(retardo);
							return;
						}
			j=traducirPosicion(base)+offset;
			printf("La posicion Virtual es: %d y la Real es : %d\n", base, j);
			printf("El resultado de la asignacion es:\n");
			i=0;
			printf("%s\n",(char*)buffer);
			memcpy(&MP[j], (int*) buffer, longitud);
			escribir_log(archLog, "Se realiza envio de bytes", INFO, "El envio tiene exito");
			} else puts("No se pudo realizar la asignacion");
		sleep(retardo);
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
		if (&MP[i] == NULL) libre++;
		i++;
	}
	return libre;
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
	archivo_MP = fopen("/home/utnso/dump_file_MP", "w");
	archivo_TS = fopen("/home/utnso/dump_file_TS", "w");
	if (archivo_MP==NULL) {
		fputs ("File error",stderr); exit (1);
	}
	if (archivo_TS==NULL) {
		fputs ("File error",stderr); exit (1);
	}

	//Estructuras de memoria
	puts("Ingrese el numero de proceso del cual se quiere conocer sus segmentos o '-1' para verlos todos");
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
	int getEspacioLibreMP(void);
	int espacioLibre= getEspacioLibreMP();
	printf("\n El espacio libre en la memoria principal es: \n");
	printf("%d",espacioLibre);
	fprintf(archivo_MP,"%s", "\n El espacio libre en la memoria principal es: \n");
	fprintf(archivo_MP,"%d",espacioLibre);

	//Contenido de la memoria principal
	int offset,tamanio;
	t_buffer buffer;
	puts("\n Ingrese el offset con la posicion de MP a conocer y la cantidad de bytes a leer \n");
	scanf("%d", &offset);
	scanf("%d", &tamanio);
	buffer = malloc((tamanio+1)*sizeof(char));
	memcpy(buffer, (char *) &MP[offset], tamanio);
	printf("La posicion de memoria %d contiene: %s\"\n", offset,(char*)buffer);
	fprintf(archivo_MP, "La posicion de memoria %d contiene: %s\"\n", offset,(char*)buffer);//Ojo que pongo archivo_MP pero capaz deberia ser en otro

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
				printf("La tabla ");
				printf("%d", i);
				printf(":\n Corresponde al programa ");
				printf("%d", tablaDeSegmentos[i].id_prog);
				printf("\n Cantidad de segmentos ");
				printf("%d", tablaDeSegmentos[i].cant_segmentos);

				fprintf(archivo, "%s", "La tabla ");
				fprintf(archivo, "%d", i);
				fprintf(archivo, "%s", ":\n Corresponde al programa ");
				fprintf(archivo, "%d", tablaDeSegmentos[i].id_prog);
				fprintf(archivo, "%s", "\n Cantidad de segmentos ");
				fprintf(archivo, "%d", tablaDeSegmentos[i].cant_segmentos);

				while(j<tablaDeSegmentos[i].cant_segmentos){
				printf("\n Segmento ");
				printf("%d", j);
				printf(":\n Posicion real ");
				printf("%d", tablaDeSegmentos[i].segmentos[j].ubicacionMP);
				printf("\n Posicion virtual ");
				printf("%d", tablaDeSegmentos[i].segmentos[j].inicio);
				printf("\n Tamanio ");
				printf("%d", tablaDeSegmentos[i].segmentos[j].tamanio);

				fprintf(archivo, "%s", "\n Segmento ");
				fprintf(archivo, "%d", j);
				fprintf(archivo, "%s", ":\n Posicion real ");
				fprintf(archivo, "%d", tablaDeSegmentos[i].segmentos[j].ubicacionMP);
				fprintf(archivo, "%s", "\n Posicion virtual ");
				fprintf(archivo, "%d", tablaDeSegmentos[i].segmentos[j].inicio);
				fprintf(archivo, "%s", "\n Tamanio ");
				fprintf(archivo, "%d", tablaDeSegmentos[i].segmentos[j].tamanio);

				j++;
				}
				fprintf(archivo, "%s", " \n\n");
				printf("\n\n");
				i++;
			}
		sleep(retardo);
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
			compactar();
			sleep(retardo);
			return -1;
		}
	reservarEspacioMP(ubicacion, tamanio);
	int pos=inicializarTabla(id_prog);
	i=rand();
	while(!validarPosicionVirtual(i)) rand();//Recorrer la tabla de segmentos validando que ninguno ocupe entre la posicion y la posicion y el tamanio
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
	return 1;
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

		configuracion_UMV.puerto_cpus = config_get_int_value(config,
					"Puerto TCP para recibir conexiones de los CPUs");

		configuracion_UMV.puerto_kernel = config_get_int_value(config,
					"Puerto TCP para recibir conexiones del Kernel");


		configuracion_UMV.ip_kernel = config_get_string_value(config,
					"Direccion IP para conectarse al Kernel");


		configuracion_UMV.algoritmo = config_get_int_value(config,
					"Algoritmo de seleccion de ubicacion de segmento");

}

void imprimirConfiguracion(void) { // Funcion para testear que lee correctamente el archivo de configuracion

	printf("Tamanio de memoria Principal: %d\n", configuracion_UMV.memSize);
	printf("Puerto para conexiones con CPUs: %d\n", configuracion_UMV.puerto_cpus);
	printf("Puerto para conexiones con Kernel: %d\n", configuracion_UMV.puerto_kernel);
	printf("IP del Kernel: %s\n", configuracion_UMV.ip_kernel);
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

//****************************************Atender Conexiones de Kernel/CPU*******************

void core_conexion_cpu(void){
	int sock;
	pthread_t atender_pedido;

	if((sock_cpu=socket_crearServidor("127.0.0.1", configuracion_UMV.puerto_cpus))>0){
	printf("Hilo de CPU \n");
	escribir_log(archLog, "Escuchando en el socket de CPU's", INFO, "");
	}

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
		if(socket_cerrarConexion(sock_cpu)==0){
				escribir_log(archLog, "Se trata de cerrar el socket de CPU", ERROR, "Hay problemas para cerrar el socket");
				//Error cerrando el socket
			} else {
				escribir_log(archLog, "Se cierra el socket de CPU", INFO, "No hay problemas para cerrar el socket");
			}
	return;
	}
}


void atender_cpu(void){
	/*UNSOLVED:
	 * int programaEnHilo;
	 * void* estructura;
	 * t_tipoEstructura tipo_estructura;
	 * socket_recibir(sock, &tipo_estructura, &estructura);
	 * ejecutar(&tipo_estructura, &estructura);		//ejecutaria lo correspondiente y crearia la estructura a enviar
	 * send(sock, &tipo_estructura, &estructura);
	 */

}




void core_conexion_kernel(void){
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
	int tamanioMaxStack;
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
}

void atender_kernel(int sock){
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	t_struct_numero* tamanio;
	int i,id_prog,memoriaSuficiente=0;
	int tamanioSolicitado;

	socket_recibir(sock, &tipoRecibido, &structRecibida);
	if(tipoRecibido==D_STRUCT_NUMERO){
		id_prog = ((t_struct_numero*)structRecibida)->numero;
		free(structRecibida);
	}
	for(i=0; i<4 && memoriaSuficiente==0;i++){
		socket_recibir(sock, &tipoRecibido,&structRecibida);
		tamanio = ((t_struct_numero*)structRecibida);
		tamanioSolicitado = tamanio->numero;
		//memoriaSuficiente = crearSegmentoPrograma(id_prog, tamanioSolicitado);
	}
	if (memoriaSuficiente==-1){
		escribir_log(archLog, "Memoria insuficiente", ERROR, "No hay memoria suficiente para el paquete");
		destruirSegmentosPrograma(id_prog);
		t_struct_numero* respuesta= malloc(sizeof(t_struct_numero));
		respuesta->numero=memoriaSuficiente;
		socket_enviar(sock, D_STRUCT_NUMERO, respuesta);
		free(respuesta);
	}
	if(memoriaSuficiente==1){
		t_struct_numero* respuesta= malloc(sizeof(t_struct_numero));
		respuesta->numero=memoriaSuficiente;
		socket_enviar(sock, D_STRUCT_NUMERO, respuesta);
		free(respuesta);
		for(i=0;i<4;i++){
			socket_recibir(sock,&tipoRecibido,&structRecibida);
			//ACA IRIAN LOS SEGMENTOS DE CODIGO PARA GRABAR LOS BYTES
			//enviarBytes(base,offset,longitud,buffer);
		}
	}





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
	pthread_create(&KERNEL, NULL, (void*) &core_conexion_kernel, NULL);
	pthread_create(&CPU, NULL, (void*) &core_conexion_cpu, NULL);
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
	puts("Ingrese operacion a ejecutar (operacion, retardo, algoritmo, compactacion, dump y exit para salir)");
	scanf("%s",&comando);
	while(estaEnDicOP(comando)== 0){
		puts("\nOperacion erronea, escriba la operacion de nuevo");
		scanf("%s",&comando);
	}

	while(strcmp(comando, "exit") != 0){
		system("clear");
			if(strcmp(comando, "operacion") == 0){

				char tipoOperacion[32];
				puts("Desea solicitar posicion de memoria (solicitar) o escribir buffer por teclado (escribir) o crear segmento de programa (crear)o destruir segmento de programa (destruir)?");
				scanf("%s",&tipoOperacion);
				while(estaEnDicTOP(tipoOperacion)== 0){
						puts("Tipo de Operacion erronea, escriba el tipo de operacion de nuevo");
						scanf("%s",&tipoOperacion);
					}
				if(strcmp(tipoOperacion, "solicitar") == 0){
					  system("clear");
					  puts("Ingrese Base");
					  scanf("%d",&unaBase);
					  puts("Ingrese Offset");
					  scanf("%d",&unOffset);
					  puts("Ingrese Tamanio de segmento");
					  scanf("%d",&unTamanio);
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					  aux_buffer = solicitarBytes(unaBase,unOffset,unTamanio);
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
				if(strcmp(tipoOperacion, "escribir") == 0){
					 system("clear");
					 puts("Ingrese Base");
					 scanf("%d",&unaBase);
					 puts("Ingrese Offset");
					 scanf("%d",&unOffset);
					 puts("Ingrese Buffer");
					 scanf("%s",aux_buffer);
					 unTamanio=strlen(aux_buffer+1);
					 printf("El tamanio es: %d\n",unTamanio);
					 pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					 enviarBytes(unaBase,unOffset,unTamanio,aux_buffer);
					 pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
				if(strcmp(tipoOperacion, "crear") == 0){
					  system("clear");
					  puts("Ingrese el processID de programa a usar");
				 	  scanf("%d",&procesoDelHilo);
					  puts("Ingrese el tamaño del segmento");
					  int tamanio;
					  scanf("%d",&tamanio);
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					  crearSegmentoPrograma(procesoDelHilo,tamanio);
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
				if(strcmp(tipoOperacion, "destruir") == 0){
					  system("clear");
					  puts("Ingrese el processID de programa a usar");
					  scanf("%d",&procesoDelHilo);
					  pthread_mutex_lock(mutex);	//Bloquea el semaforo para utilizar una variable compartida
					  destruirSegmentosPrograma(procesoDelHilo);
					  pthread_mutex_unlock(mutex);	//Desbloquea el semaforo ya que termino de utilizar una variable compartida
				}
			}

	else { if (strcmp(comando, "retardo") == 0){
				  puts("Ingrese el valor de retardo en Milisegundos");
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
	    system("clear");
		puts("Escriba la siguiente operacion");
		scanf("%s",&comando);
		while(estaEnDicOP(comando)== 0){
			system("clear");
			puts("Operacion erronea, escriba la operacion de nuevo");
			scanf("%s",&comando);
			}

		}
		if(strcmp(comando,"exit") ==0){
			destruirTodosLosSegmentos();
			free(MP);
			free(tablaDeSegmentos);
		   	socket_cerrarConexion(sock_kernel_servidor);
		   	socket_cerrarConexion(sock_cpu);
		   	matarHilos();
			if(pthread_kill(CPU,0)==0){
			printf("Muere el hilo cpu\n");
			}
			if(pthread_kill(KERNEL,0)==0){
			printf("Muere el hilo Kernel\n");

			}

}
}

void matarHilos(void){
	pthread_cancel(CPU);
	pthread_cancel(KERNEL);

}

void destruirTodosLosSegmentos(void){
	/*int i=0;
	while(i<cant_tablas){
	free(tablaDeSegmentos[i].segmentos);
	}
	return;*/
}





