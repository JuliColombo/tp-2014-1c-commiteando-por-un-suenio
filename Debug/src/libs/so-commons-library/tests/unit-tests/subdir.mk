################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/libs/so-commons-library/tests/unit-tests/test_bitarray.c \
../src/libs/so-commons-library/tests/unit-tests/test_config.c \
../src/libs/so-commons-library/tests/unit-tests/test_dictionary.c \
../src/libs/so-commons-library/tests/unit-tests/test_list.c \
../src/libs/so-commons-library/tests/unit-tests/test_queue.c \
../src/libs/so-commons-library/tests/unit-tests/test_string.c \
../src/libs/so-commons-library/tests/unit-tests/tests.c 

OBJS += \
./src/libs/so-commons-library/tests/unit-tests/test_bitarray.o \
./src/libs/so-commons-library/tests/unit-tests/test_config.o \
./src/libs/so-commons-library/tests/unit-tests/test_dictionary.o \
./src/libs/so-commons-library/tests/unit-tests/test_list.o \
./src/libs/so-commons-library/tests/unit-tests/test_queue.o \
./src/libs/so-commons-library/tests/unit-tests/test_string.o \
./src/libs/so-commons-library/tests/unit-tests/tests.o 

C_DEPS += \
./src/libs/so-commons-library/tests/unit-tests/test_bitarray.d \
./src/libs/so-commons-library/tests/unit-tests/test_config.d \
./src/libs/so-commons-library/tests/unit-tests/test_dictionary.d \
./src/libs/so-commons-library/tests/unit-tests/test_list.d \
./src/libs/so-commons-library/tests/unit-tests/test_queue.d \
./src/libs/so-commons-library/tests/unit-tests/test_string.d \
./src/libs/so-commons-library/tests/unit-tests/tests.d 


# Each subdirectory must supply rules for building sources it contributes
src/libs/so-commons-library/tests/unit-tests/%.o: ../src/libs/so-commons-library/tests/unit-tests/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/utnso/tp-2014-1c-commiteando-por-un-suenio/src/libs" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


