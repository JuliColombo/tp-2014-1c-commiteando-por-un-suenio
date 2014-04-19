
#include "funciones.h"

typedef struct pares{int desplazamiento; int longitudLinea;} Pares;

typedef struct pcb {
		int ID;
	    int* CODE, STACK, CursorSTACK;
	    Pares indiceCodigo[];
	    int* indiceEtiquetas;
	    int* ProgramCounter;
	    int TamanioContext;
} PCB;


