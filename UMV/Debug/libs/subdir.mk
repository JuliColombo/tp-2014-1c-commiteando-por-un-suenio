################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/funciones_UMV.c \
../libs/stack.c 

OBJS += \
./libs/funciones_UMV.o \
./libs/stack.o 

C_DEPS += \
./libs/funciones_UMV.d \
./libs/stack.d 


# Each subdirectory must supply rules for building sources it contributes
libs/%.o: ../libs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


