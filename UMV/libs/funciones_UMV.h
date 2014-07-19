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

/*Pathcongif*/
#define PATHCONFIG "/home/utnso/tp-2014-1c-commiteando-por-un-suenio/UMV/configuracion_UMV.cfg"

/*Variables globales*/

extern t_config_UMV configuracion_UMV;
extern int* MP;
extern pthread_t CONSOLA,KERNEL,CPU;
extern log_t* archLog;
extern pthread_mutex_t* mutex;
extern pthread_mutex_t* mutex_log;
extern char* PATH;
extern int tamanioMP;
extern int procesoEnUso;
extern tablaSeg* tablaDeSegmentos;
extern int cant_tablas;
extern int sock_kernel_servidor;
extern int sock_cpu;
extern int retardo;
extern int* procesosActivos;
extern int gradoDeMultiprogramacion;
extern lista_handshake lista_handshakes;

/*Prototipos de funciones auxiliares*/

int asignarFisicamente();
void log_error_socket(void);
int solicitarMemoria(int tamanio);
_Bool memoryOverload(int longitud);
_Bool segmentationFault(int base,int offset,int longitud);
_Bool validarSolicitud(int base, int offset, int longitud);
_Bool hayEspacioEnMemoriaPara(uint32_t longitud);
_Bool tamanioSuficienteEnMemoriaPara(uint32_t longitud);

void destruirTodosLosSegmentos(void);
void matarHilos(void);
void liberarMP(int id_prog);
void eliminarSegmentos(int id_prog);
int escogerUbicacionF(int tamanio);
int escogerUbicacionW(int tamanio);
int* crearMP();
int estaEnDicOP(char palabra[]);
int estaEnDicTOP(char palabra[]);
int inicializarTabla(int id_prog);
void inicializarListaHandshakes(void);
void reservarEspacioMP(int ubicacion, int tamanio);
int getPosTabla(int id_prog);
void hacerHandshake(tipo_handshake tipo);
void agregarHandshake(tipo_handshake tipo);
void inicializarYAgregar(tipo_handshake tipo);
int traducirPosicion(int base);
int validarPosicionVirtual(int posVirtual);
void imprimirEstadoMP(FILE* archivo);
void imprimirEstadoTablaSeg(FILE* archivo);
int ubicarEnTabla(int posicionSegmento);
int ubicarPosiconRealEnTabla(int posicionSegmento);
t_buffer asignarPosicionesBuffer(t_buffer buffer, int unTamanio);


/*Operaciones de Consola*/
void algoritmo(void);
void retardoFunc(int valorRetardoEnMilisegundos);
void compactar(void);
void dump();
t_buffer solicitarDesdePosicionDeMemoria(uint32_t base,uint32_t offset, uint32_t longitud);
t_buffer obtenerBytesDesdeHasta(uint32_t posicionReal,uint32_t longitud);
void enviarBytes(int base,int offset,int longitud,t_buffer buffer);
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

void* core_consola(void);
void* consola(void);

void core_conexion_cpu(void);
void crear_hilo_por_cpu(void);
void atender_cpu(void);

void core_conexion_kernel(void);
void atender_kernel(int sock);

#endif /* UMV_H_ */

