################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/BitIoLdd1.c \
../Generated_Code/BitIoLdd10.c \
../Generated_Code/BitIoLdd14.c \
../Generated_Code/BitIoLdd15.c \
../Generated_Code/BitIoLdd16.c \
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
../Generated_Code/LEDB.c \
../Generated_Code/LEDG.c \
../Generated_Code/LEDR.c \
../Generated_Code/LEDpin1.c \
../Generated_Code/LEDpin2.c \
../Generated_Code/LEDpin3.c \
../Generated_Code/MCUC1.c \
../Generated_Code/PE_LDD.c \
../Generated_Code/RS1.c \
../Generated_Code/TSSin.c \
../Generated_Code/Vectors.c \
../Generated_Code/WAIT1.c 

OBJS += \
./Generated_Code/BitIoLdd1.o \
./Generated_Code/BitIoLdd10.o \
./Generated_Code/BitIoLdd14.o \
./Generated_Code/BitIoLdd15.o \
./Generated_Code/BitIoLdd16.o \
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
./Generated_Code/LEDB.o \
./Generated_Code/LEDG.o \
./Generated_Code/LEDR.o \
./Generated_Code/LEDpin1.o \
./Generated_Code/LEDpin2.o \
./Generated_Code/LEDpin3.o \
./Generated_Code/MCUC1.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/RS1.o \
./Generated_Code/TSSin.o \
./Generated_Code/Vectors.o \
./Generated_Code/WAIT1.o 

C_DEPS += \
./Generated_Code/BitIoLdd1.d \
./Generated_Code/BitIoLdd10.d \
./Generated_Code/BitIoLdd14.d \
./Generated_Code/BitIoLdd15.d \
./Generated_Code/BitIoLdd16.d \
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
./Generated_Code/LEDB.d \
./Generated_Code/LEDG.d \
./Generated_Code/LEDR.d \
./Generated_Code/LEDpin1.d \
./Generated_Code/LEDpin2.d \
./Generated_Code/LEDpin3.d \
./Generated_Code/MCUC1.d \
./Generated_Code/PE_LDD.d \
./Generated_Code/RS1.d \
./Generated_Code/TSSin.d \
./Generated_Code/Vectors.d \
./Generated_Code/WAIT1.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"D:/Users/Flávio Soares/Projetos/SENAI/GoodsTracker/platform/goodstrackerLCD/Static_Code/PDD" -I"D:/Users/Flávio Soares/Projetos/SENAI/GoodsTracker/platform/goodstrackerLCD/Static_Code/IO_Map" -I"D:/Users/Flávio Soares/Projetos/SENAI/GoodsTracker/platform/goodstrackerLCD/Sources" -I"D:/Users/Flávio Soares/Projetos/SENAI/GoodsTracker/platform/goodstrackerLCD/Generated_Code" -I"D:/Users/Flávio Soares/Projetos/SENAI/GoodsTracker/platform/goodstrackerLCD/Sources/TSS" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


