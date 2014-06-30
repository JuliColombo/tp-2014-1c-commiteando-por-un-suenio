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
#define pathconfig "/home/utnso/tp-2014-1c-commiteando-por-un-suenio/Programa/ANSISOP_CONFIG"


/************** VARIABLES GLOBALES ***************/

extern t_config_programa configuracion_programa;
extern char* PATH_config;
extern char* archivo_ansisop;
extern int sock_kernel_servidor;
extern FILE* script;

/************** FUNCIONES **************/

char* leerScript(off_t* ,char*); //lee el script
int validarScript(void);
void inicializarConfiguracion(void);
void leerConfiguracion(void);
void imprimirConfiguracion(void);
int abrirSocket(void);
void cerrarSocket(int);
t_struct_pidycodigo* crear_PID_Codigo(char* codigo);

#endif /* FUNCIONESINTERPRETE_H_ */
