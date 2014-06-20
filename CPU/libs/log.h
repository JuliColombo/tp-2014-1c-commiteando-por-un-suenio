/*
 * log.h
 *
 *  Created on: 08/11/2012
 *      Author: utnso
 */

#ifndef LOG_H_
#define LOG_H_
#include <stdio.h>
#include <stdint.h>
#include <linux/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <linux/sched.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <sys/timeb.h>
#define PATHLOG "/home/utnso/tp-2014-1c-commiteando-por-un-suenio"





#define MAX_MESSAGE_LENGHT 1024

typedef enum {
	DEBUG = 1, INFO = 2, WARNING = 3, ERROR = 4, LSCH = 5,
} e_message_type;

typedef enum {
	M_NONE = 1, M_CONSOLE = 2, M_FILE = 3, M_CONSOLEANDFILE = 4
} e_print_mode;

typedef struct {

	FILE* file;
	pid_t pid;
	e_print_mode mode;
} log_t;


extern log_t* archLog;

log_t* log_crear(char* pathLog);											// Crea un log

void log_destruir(log_t *log);												// Destruye el puntero al log y cierra el archivo

int log_escribir(log_t *log, const char *program_name, e_message_type type,	// Escribe en el log
		const char* format, ...);

int log_vwrite(log_t *log, const char *program_name,						// Escribe en el log
		e_message_type type, const char* format, va_list args_list);

char *log_errorTypeAsString( e_message_type type );							// Convierte un tipo numerico de error a su version en string

void log_setPrintMode(log_t* log, e_print_mode mode);						// Activa o descativa la impresion por consola

e_print_mode log_getPrintMode(log_t* log); 									// Devuelve el modo de impresion de la consola


#endif /* LOG_H_ */
