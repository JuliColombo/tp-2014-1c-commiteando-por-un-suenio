################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PrimitivasEnTest.c \
../Test.c \
../stack.c \
../tests-primitivas.c 

OBJS += \
./PrimitivasEnTest.o \
./Test.o \
./stack.o \
./tests-primitivas.o 

C_DEPS += \
./PrimitivasEnTest.d \
./Test.d \
./stack.d \
./tests-primitivas.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


