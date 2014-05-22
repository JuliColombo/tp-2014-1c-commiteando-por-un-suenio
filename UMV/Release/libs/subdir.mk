################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/funciones_UMV.c \
../libs/log.c \
../libs/socket.c 

OBJS += \
./libs/funciones_UMV.o \
./libs/log.o \
./libs/socket.o 

C_DEPS += \
./libs/funciones_UMV.d \
./libs/log.d \
./libs/socket.d 


# Each subdirectory must supply rules for building sources it contributes
libs/%.o: ../libs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


