################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CI2cDevice.cpp \
../src/CPcf8591.cpp \
../src/CThermostat.cpp \
../src/main.cpp 

OBJS += \
./src/CI2cDevice.o \
./src/CPcf8591.o \
./src/CThermostat.o \
./src/main.o 

CPP_DEPS += \
./src/CI2cDevice.d \
./src/CPcf8591.d \
./src/CThermostat.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I/opt/rpi-libs/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


