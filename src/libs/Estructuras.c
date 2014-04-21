
#include "funciones.h"
#include "commons/collections/dictionary.h"

typedef struct pares{
		int desplazamiento;
		int longitudLinea;
} Pares;

typedef struct pcb {
		int ID;
	    int* CODE, STACK, CursorSTACK;
	    //estaba int* indiceEtiquetas, pero por lo que dice la consigna: el indice de etiquetas constara de la serializacion
	    //de un diccionario que ontiene el id de cada funcion o etiqueta...
	    t_dictionary* indiceEtiquetas;
	    int* ProgramCounter;
	    int TamanioContext;
	    Pares indiceCodigo[];
} PCB;
