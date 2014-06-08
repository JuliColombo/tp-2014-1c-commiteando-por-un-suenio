/*
 * PLP.c
 *
 *  Created on: Apr 22, 2014
 *      Author: utnso
 */


#include "FuncionesPLP.h"

int main(int arc, char **argv[]){
	t_programa programa,programa1,programa2,programa3;
	programa.peso=2;
	programa1.peso=programa3.peso=7;
	programa2.peso = 10;
	t_programa v[4]={programa,programa1,programa2,programa3};
	int a=main_plp(4,v[4]);
	printf("%d",a);
	return 0;

}
