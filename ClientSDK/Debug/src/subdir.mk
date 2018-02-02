################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Client.cpp \
../src/Display.cpp \
../src/Game.cpp \
../src/TCPClient.cpp \
../src/UDPClient.cpp \
../src/UDPSocket.cpp 

OBJS += \
./src/Client.o \
./src/Display.o \
./src/Game.o \
./src/TCPClient.o \
./src/UDPClient.o \
./src/UDPSocket.o 

CPP_DEPS += \
./src/Client.d \
./src/Display.d \
./src/Game.d \
./src/TCPClient.d \
./src/UDPClient.d \
./src/UDPSocket.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/user/Desktop/FinalProjectC++/SharedFiles/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


