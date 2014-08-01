/*
 * UMV.h
 *
 *  Created on: 25/04/2014
 *      Author: utnso
 */

#ifndef UMV_H_
#define UMV_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include "commons/collections/dictionary.h"
#include "estructuras_umv.h"
#include <string.h>
#include "parser/metadata_program.h"
#include "funciones_UMV.h"
#include "commons/config.h"
#include "commons/collections/list.h"
#include "log.h"
#include <semaphore.h>
#include "Sockets/socket.h"
#include "Sockets/estructuras_socket.h"

/*Pathcongif*/

#define PATHCONFIG "/home/utnso/tp-2014-1c-commiteando-por-un-suenio/UMV/configuracion_UMV.cfg"
#define MAX_BUFFER 500

/*Variables globales*/

extern t_config_UMV configuracion_UMV;
extern void* MP;
extern pthread_t CONSOLA,CONEXIONES,CPU;
extern log_t* archLog;
extern pthread_mutex_t* mutex_MP;
extern pthread_mutex_t* mutex_log;
extern pthread_mutex_t* mutex_pid;
extern char* PATH;
extern int tamanioMP;
extern int tamanioMaxStack;
extern int procesoEnUso;
extern tablaSeg* tablaDeSegmentos;
extern int cant_tablas;
extern int sock_servidor;
extern int sock_cpu;
extern int retardo;
extern int procesoActivo;
extern int gradoDeMultiprogramacion;
extern lista_handshake lista_handshakes;

// Variables Globales de la UMV
extern t_list * Segmentos_UMV;
extern t_list * Rangos_Libres;
extern t_list * List_Base_ID;
extern int AlgoritmoActual, Puerto;
extern int FinPrograma;
extern int Retardo;

// Semaforos
extern pthread_mutex_t Sem_Graba_Segmento;
extern pthread_mutex_t Sem_Elimina_Segmento;
extern pthread_mutex_t Sem_GrabaBytes;
extern pthread_mutex_t Sem_DevuelveBytes;


/*************************Fin de variable globales*************************/


/*Prototipos de funciones auxiliares*/

void log_error_socket(void);


void destruirTodosLosSegmentos(void);
void matarHilos(void);
void* crearMP();
int estaEnDicOP(char palabra[]);
int estaEnDicTOP(char palabra[]);
void imprimirBuffer(t_buffer buffer);
void imprimirBufferEnArchivo(t_buffer buffer,FILE* archivo);
void imprimirEstadoMP(FILE* archivo);
void imprimirEstadoTablaSeg(FILE* archivo,int i, int tablaFinal);
int escribir_log(log_t *log, const char *program_name, e_message_type type,	const char* format);
void cambioProcesoActivo(int);

/*Operaciones de Consola*/
void algoritmo(void);
void retardoFunc(int valorRetardoEnMilisegundos);
void compactar(void);
void dump();
int enviarBytes(int base,int offset,int longitud,t_buffer buffer);
t_struct_respuesta_umv solicitarBytes(int base,int offset, int longitud);
int crearSegmentoPrograma(int id_prog, int tamanio);
void destruirSegmentos(int id_prog);

/*Funciones del archivo Config*/
void leerConfiguracion(void);				  				 			 // Lee en la macro del archivo de configuracion ubicado en la ruta PATH
void imprimirConfiguracion(void);			  				 			 // Muestra por pantalla todos los campos de la macro del archivo de configuracion
void inicializarConfiguracion(void);

/*Prototipos de semaforos*/
void inicializarMutex(void);

/*Prototipos de threads*/
void inicializarHilos(void);
void esperarHilos(void);

void handshake_conexion(void);

void* core_consola(void);
void* consola(void);

void atender_cpu(sock_struct* sock);

void atender_kernel(sock_struct* sock);

/*************************Segunda tanda de funciones*************************/

// Firmas funciones Void
void ContarTamano(RangoMemoria* rango);
void Grabar(int posMem, void * element);
void * Consultar(int posMem);
void ManejoKernel(int coneccion);
void MostrarRangosMemoriaLibre();
void EliminarSegmento(int base);
void EliminarSegmentosDePrograma(int programa);
void Mostrar(Segmento* rango);
void MostrarSegmentos();
void DumpInConsole();
void Dump();
void logRangosLibres(RangoMemoria* rango);
void logSegmentos(Segmento* rango);
void DumpDeUnPrograma(int Programa,FILE* archivo);
void DumpMemory(int base, int tamano);
void ActualizarRangoGrabado(int tamano_guardado, RangoMemoria rango);
void PrintTable(BaseID *  rango);
void MostrarTablaBI();
//Firmas funciones Int
int GrabarSegmento(int programa, int tamanoSegmento);
int Hay_algun_rango_de_tamano_suficiente(RangoMemoria *rango);
int CantidadMemoriaLibre();
int GuardarNuevoSegmentoOrdenado(int ID, int programa, int baseVirtual, int tamano);
int PosicionDeSegmento(int base);
int GrabarBytes(int ID, int offset, int tamano, void* buffer);
int GuardarNuevoRangoOrdenado(RangoMemoria Rango);


//Otra firmas de funciones
Segmento *create_segmento(int ID,int programa,void* base, int baseVirtual,int tamano);
RangoMemoria RangoMasGrandeLibre();
bool SePuedeGrabarSegmento(int tamano);
Segmento BuscarSegmento(int base);
t_struct_respuesta_umv EnviarBytes(int base, int offset, int tamano);
RangoMemoria *create_rango_mem(int base, int tamano);
BaseID *create_BaseID(int base, int ID);
Segmento BuscarSegmentoIndice(int ID);
BaseID * BuscarBI(int base);
char * ObtenerIP();

#endif /* UMV_H_ */

