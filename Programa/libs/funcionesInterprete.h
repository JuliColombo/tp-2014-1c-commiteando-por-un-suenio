/*
 * funcionesInterprete.h
 *
 *  Created on: 16/04/2014
 *      Author: utnso
 */

#ifndef FUNCIONESINTERPRETE_H_
#define FUNCIONESINTERPRETE_H_
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "commons/config.h"
#include "estructuras_programa.h"
#include "Sockets/socket.h"
#define PATHCONFIG "/home/utnso/tp-2014-1c-commiteando-por-un-suenio/Programa/configuracion_programa.cfg"


/************** VARIABLES GLOBALES ***************/

extern t_config_programa configuracion_programa;
extern char* PATH;
extern char* Archivo_Ansisop;
extern int sock_kernel_servidor;

/************** FUNCIONES **************/

char* leerScript(off_t* ,char*); //lee el script
void inicializarConfiguracion(void);
void leerConfiguracion(void);
void imprimirConfiguracion(void);
int abrirSocket(void);
void cerrarSocket(int);

#endif /* FUNCIONESINTERPRETE_H_ */
