################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/BitIoLdd1.c \
../Generated_Code/BitIoLdd10.c \
../Generated_Code/BitIoLdd2.c \
../Generated_Code/BitIoLdd7.c \
../Generated_Code/BitIoLdd8.c \
../Generated_Code/BitIoLdd9.c \
../Generated_Code/Cpu.c \
../Generated_Code/DB41.c \
../Generated_Code/DB51.c \
../Generated_Code/DB61.c \
../Generated_Code/DB71.c \
../Generated_Code/EN1.c \
../Generated_Code/LCDout.c \
../Generated_Code/MCUC1.c \
../Generated_Code/PE_LDD.c \
../Generated_Code/RS1.c \
../Generated_Code/Vectors.c \
../Generated_Code/WAIT1.c 

OBJS += \
./Generated_Code/BitIoLdd1.o \
./Generated_Code/BitIoLdd10.o \
./Generated_Code/BitIoLdd2.o \
./Generated_Code/BitIoLdd7.o \
./Generated_Code/BitIoLdd8.o \
./Generated_Code/BitIoLdd9.o \
./Generated_Code/Cpu.o \
./Generated_Code/DB41.o \
./Generated_Code/DB51.o \
./Generated_Code/DB61.o \
./Generated_Code/DB71.o \
./Generated_Code/EN1.o \
./Generated_Code/LCDout.o \
./Generated_Code/MCUC1.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/RS1.o \
./Generated_Code/Vectors.o \
./Generated_Code/WAIT1.o 

C_DEPS += \
./Generated_Code/BitIoLdd1.d \
./Generated_Code/BitIoLdd10.d \
./Generated_Code/BitIoLdd2.d \
./Generated_Code/BitIoLdd7.d \
./Generated_Code/BitIoLdd8.d \
./Generated_Code/BitIoLdd9.d \
./Generated_Code/Cpu.d \
./Generated_Code/DB41.d \
./Generated_Code/DB51.d \
./Generated_Code/DB61.d \
./Generated_Code/DB71.d \
./Generated_Code/EN1.d \
./Generated_Code/LCDout.d \
./Generated_Code/MCUC1.d \
./Generated_Code/PE_LDD.d \
./Generated_Code/RS1.d \
./Generated_Code/Vectors.d \
./Generated_Code/WAIT1.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"D:/Users/Flávio Soares/workspace.kds/goodstrackerLCD/Static_Code/PDD" -I"D:/Users/Flávio Soares/workspace.kds/goodstrackerLCD/Static_Code/IO_Map" -I"D:/Users/Flávio Soares/workspace.kds/goodstrackerLCD/Sources" -I"D:/Users/Flávio Soares/workspace.kds/goodstrackerLCD/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


