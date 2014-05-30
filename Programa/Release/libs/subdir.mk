################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/funcionesInterprete.c \
../libs/log.c \
../libs/package.c \
../libs/primitivas.c \
../libs/socket.c 

OBJS += \
./libs/funcionesInterprete.o \
./libs/log.o \
./libs/package.o \
./libs/primitivas.o \
./libs/socket.o 

C_DEPS += \
./libs/funcionesInterprete.d \
./libs/log.d \
./libs/package.d \
./libs/primitivas.d \
./libs/socket.d 


# Each subdirectory must supply rules for building sources it contributes
libs/%.o: ../libs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


