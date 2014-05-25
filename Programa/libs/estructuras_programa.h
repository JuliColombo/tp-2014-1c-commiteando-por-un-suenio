#ifndef ESTRUCTURAS_PROGRAMA_H_
#define ESTRUCTURAS_PROGRAMA_H_

typedef uint16_t t_puerto_kernel;
typedef uint16_t t_ip_kernel;

typedef struct {
t_ip_kernel ip_kernel;	//Direccion IP para conectarse al Kernel
t_puerto_kernel puerto_kernel;	//Puerto tcp para recibir las conexiones del kernel
} t_config_programa;


#endif /* ESTRUCTURAS_PROGRAMA_H_ */
