################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Dispatcher.cpp \
../src/LoginAndRegister.cpp \
../src/Md5.cpp \
../src/PeerName.cpp \
../src/Server.cpp 

OBJS += \
./src/Dispatcher.o \
./src/LoginAndRegister.o \
./src/Md5.o \
./src/PeerName.o \
./src/Server.o 

CPP_DEPS += \
./src/Dispatcher.d \
./src/LoginAndRegister.d \
./src/Md5.d \
./src/PeerName.d \
./src/Server.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/user/Desktop/FinalProjectC++/SharedFiles/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


