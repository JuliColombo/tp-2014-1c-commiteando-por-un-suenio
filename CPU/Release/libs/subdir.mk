################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/funciones_CPU.c \
../libs/log.c \
../libs/primitivas.c \
../libs/primitivasAux.c 

OBJS += \
./libs/funciones_CPU.o \
./libs/log.o \
./libs/primitivas.o \
./libs/primitivasAux.o 

C_DEPS += \
./libs/funciones_CPU.d \
./libs/log.d \
./libs/primitivas.d \
./libs/primitivasAux.d 


# Each subdirectory must supply rules for building sources it contributes
libs/%.o: ../libs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


