################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/libs/Estructuras.c \
../src/libs/funciones.c \
../src/libs/log.c 

OBJS += \
./src/libs/Estructuras.o \
./src/libs/funciones.o \
./src/libs/log.o 

C_DEPS += \
./src/libs/Estructuras.d \
./src/libs/funciones.d \
./src/libs/log.d 


# Each subdirectory must supply rules for building sources it contributes
src/libs/%.o: ../src/libs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/utnso/SisOp/tp-2014-1c-commiteando-por-un-suenio/src/libs" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


