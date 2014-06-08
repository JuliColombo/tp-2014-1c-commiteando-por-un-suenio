################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/libs/so-commons-library/src/commons/bitarray.c \
../src/libs/so-commons-library/src/commons/config.c \
../src/libs/so-commons-library/src/commons/error.c \
../src/libs/so-commons-library/src/commons/log.c \
../src/libs/so-commons-library/src/commons/process.c \
../src/libs/so-commons-library/src/commons/string.c \
../src/libs/so-commons-library/src/commons/temporal.c \
../src/libs/so-commons-library/src/commons/txt.c 

OBJS += \
./src/libs/so-commons-library/src/commons/bitarray.o \
./src/libs/so-commons-library/src/commons/config.o \
./src/libs/so-commons-library/src/commons/error.o \
./src/libs/so-commons-library/src/commons/log.o \
./src/libs/so-commons-library/src/commons/process.o \
./src/libs/so-commons-library/src/commons/string.o \
./src/libs/so-commons-library/src/commons/temporal.o \
./src/libs/so-commons-library/src/commons/txt.o 

C_DEPS += \
./src/libs/so-commons-library/src/commons/bitarray.d \
./src/libs/so-commons-library/src/commons/config.d \
./src/libs/so-commons-library/src/commons/error.d \
./src/libs/so-commons-library/src/commons/log.d \
./src/libs/so-commons-library/src/commons/process.d \
./src/libs/so-commons-library/src/commons/string.d \
./src/libs/so-commons-library/src/commons/temporal.d \
./src/libs/so-commons-library/src/commons/txt.d 


# Each subdirectory must supply rules for building sources it contributes
src/libs/so-commons-library/src/commons/%.o: ../src/libs/so-commons-library/src/commons/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/utnso/tp-2014-1c-commiteando-por-un-suenio/src/libs" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


