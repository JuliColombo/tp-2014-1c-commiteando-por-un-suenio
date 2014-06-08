################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/libs/so-commons-library/src/commons/collections/dictionary.c \
../src/libs/so-commons-library/src/commons/collections/list.c \
../src/libs/so-commons-library/src/commons/collections/queue.c 

OBJS += \
./src/libs/so-commons-library/src/commons/collections/dictionary.o \
./src/libs/so-commons-library/src/commons/collections/list.o \
./src/libs/so-commons-library/src/commons/collections/queue.o 

C_DEPS += \
./src/libs/so-commons-library/src/commons/collections/dictionary.d \
./src/libs/so-commons-library/src/commons/collections/list.d \
./src/libs/so-commons-library/src/commons/collections/queue.d 


# Each subdirectory must supply rules for building sources it contributes
src/libs/so-commons-library/src/commons/collections/%.o: ../src/libs/so-commons-library/src/commons/collections/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/utnso/tp-2014-1c-commiteando-por-un-suenio/src/libs" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


