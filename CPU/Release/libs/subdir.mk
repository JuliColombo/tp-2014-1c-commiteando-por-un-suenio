################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/log.c \
../libs/paquete.c \
../libs/primitivas.c \
../libs/primitivasAux.c \
../libs/socket.c 

OBJS += \
./libs/log.o \
./libs/paquete.o \
./libs/primitivas.o \
./libs/primitivasAux.o \
./libs/socket.o 

C_DEPS += \
./libs/log.d \
./libs/paquete.d \
./libs/primitivas.d \
./libs/primitivasAux.d \
./libs/socket.d 


# Each subdirectory must supply rules for building sources it contributes
libs/%.o: ../libs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


