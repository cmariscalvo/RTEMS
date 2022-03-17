################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/driver.c \
../src/fifo.c \
../src/main.c 

OBJS += \
./src/driver.o \
./src/fifo.o \
./src/main.o 

C_DEPS += \
./src/driver.d \
./src/fifo.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SPARC RTEMS C Compiler'
	sparc-rtems-gcc -I/opt/rtems-4.8/sparc-rtems/leon3/lib/include -I"/home/parraman/workspace_rtems/serial_driver_rtems_solved/include" -O0 -g3 -Wall -msoft-float -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


