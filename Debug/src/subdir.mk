################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HelloWolrd.c \
../src/funcionesInterprete.c \
../src/interprete.c 

OBJS += \
./src/HelloWolrd.o \
./src/funcionesInterprete.o \
./src/interprete.o 

C_DEPS += \
./src/HelloWolrd.d \
./src/funcionesInterprete.d \
./src/interprete.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


