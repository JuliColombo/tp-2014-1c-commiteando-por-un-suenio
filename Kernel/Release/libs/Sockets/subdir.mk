################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/Sockets/package.c \
../libs/Sockets/socket.c 

OBJS += \
./libs/Sockets/package.o \
./libs/Sockets/socket.o 

C_DEPS += \
./libs/Sockets/package.d \
./libs/Sockets/socket.d 


# Each subdirectory must supply rules for building sources it contributes
libs/Sockets/%.o: ../libs/Sockets/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -lpthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


