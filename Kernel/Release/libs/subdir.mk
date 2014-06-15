################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/funciones_PLP.c \
../libs/funciones_aux.c \
../libs/log.c \
../libs/string.c 

OBJS += \
./libs/funciones_PLP.o \
./libs/funciones_aux.o \
./libs/log.o \
./libs/string.o 

C_DEPS += \
./libs/funciones_PLP.d \
./libs/funciones_aux.d \
./libs/log.d \
./libs/string.d 


# Each subdirectory must supply rules for building sources it contributes
libs/%.o: ../libs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -lpthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


