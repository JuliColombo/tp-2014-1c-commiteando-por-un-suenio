################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/Sockets/estructuras.c \
../libs/Sockets/paquete.c \
../libs/Sockets/socket.c 

OBJS += \
./libs/Sockets/estructuras.o \
./libs/Sockets/paquete.o \
./libs/Sockets/socket.o 

C_DEPS += \
./libs/Sockets/estructuras.d \
./libs/Sockets/paquete.d \
./libs/Sockets/socket.d 


# Each subdirectory must supply rules for building sources it contributes
libs/Sockets/%.o: ../libs/Sockets/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


