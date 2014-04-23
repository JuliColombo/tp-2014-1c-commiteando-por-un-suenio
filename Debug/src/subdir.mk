################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Primitivas.c \
../src/UMV.c \
../src/funcionesInterprete.c \
../src/interprete.c 

OBJS += \
./src/Primitivas.o \
./src/UMV.o \
./src/funcionesInterprete.o \
./src/interprete.o 

C_DEPS += \
./src/Primitivas.d \
./src/UMV.d \
./src/funcionesInterprete.d \
./src/interprete.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/utnso/SisOp/tp-2014-1c-commiteando-por-un-suenio/src/libs" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


