################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/FIFO.c \
../src/alloc.c \
../src/cmd.c \
../src/main.c \
../src/mem_test.c \
../src/nhc_amba.c \
../src/platform.c \
../src/ring_buffer.c \
../src/simple_dma.c \
../src/xllfifo_polling_example.c 

OBJS += \
./src/FIFO.o \
./src/alloc.o \
./src/cmd.o \
./src/main.o \
./src/mem_test.o \
./src/nhc_amba.o \
./src/platform.o \
./src/ring_buffer.o \
./src/simple_dma.o \
./src/xllfifo_polling_example.o 

C_DEPS += \
./src/FIFO.d \
./src/alloc.d \
./src/cmd.d \
./src/main.d \
./src/mem_test.d \
./src/nhc_amba.d \
./src/platform.d \
./src/ring_buffer.d \
./src/simple_dma.d \
./src/xllfifo_polling_example.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../1249_bsp/microblaze_1/include -mlittle-endian -mcpu=v11.0 -mxl-soft-mul -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


