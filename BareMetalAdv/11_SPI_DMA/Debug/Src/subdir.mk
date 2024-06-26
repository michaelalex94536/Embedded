################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc_dma.c \
../Src/dma.c \
../Src/main.c \
../Src/spi_dma.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/uart.c \
../Src/uart_dma.c 

OBJS += \
./Src/adc_dma.o \
./Src/dma.o \
./Src/main.o \
./Src/spi_dma.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/uart.o \
./Src/uart_dma.o 

C_DEPS += \
./Src/adc_dma.d \
./Src/dma.d \
./Src/main.d \
./Src/spi_dma.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/uart.d \
./Src/uart_dma.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DSTM32F446xx -c -I../Inc -I"/home/michael/Documents/UdemyEtc/AdvancedBareMetal/11_SPI_DMA/chip_headers/CMSIS/Include" -I"/home/michael/Documents/UdemyEtc/AdvancedBareMetal/11_SPI_DMA/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DSTM32F446xx -c -I../Inc -I/home/michael/Documents/UdemyEtc/AdvancedBareMetal/0_Test/chip_headers/CMSIS/Include -I/home/michael/Documents/UdemyEtc/AdvancedBareMetal/0_Test/chip_headers/CMSIS/Device/ST/STM32F4xx/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/adc_dma.d ./Src/adc_dma.o ./Src/adc_dma.su ./Src/dma.d ./Src/dma.o ./Src/dma.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/spi_dma.d ./Src/spi_dma.o ./Src/spi_dma.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/uart.d ./Src/uart.o ./Src/uart.su ./Src/uart_dma.d ./Src/uart_dma.o ./Src/uart_dma.su

.PHONY: clean-Src

