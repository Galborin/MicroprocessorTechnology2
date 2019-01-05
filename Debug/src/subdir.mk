################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/arm_fir_data.c \
../src/display.c \
../src/explorer.c \
../src/file_operations.c \
../src/filter.c \
../src/main.c \
../src/math_helper.c \
../src/menu.c \
../src/stm32f7xx_it.c \
../src/syscalls.c \
../src/system_stm32f7xx.c \
../src/ts_calibration.c \
../src/usbh_conf.c \
../src/waveplayer.c \
../src/waverecorder.c 

OBJS += \
./src/arm_fir_data.o \
./src/display.o \
./src/explorer.o \
./src/file_operations.o \
./src/filter.o \
./src/main.o \
./src/math_helper.o \
./src/menu.o \
./src/stm32f7xx_it.o \
./src/syscalls.o \
./src/system_stm32f7xx.o \
./src/ts_calibration.o \
./src/usbh_conf.o \
./src/waveplayer.o \
./src/waverecorder.o 

C_DEPS += \
./src/arm_fir_data.d \
./src/display.d \
./src/explorer.d \
./src/file_operations.d \
./src/filter.d \
./src/main.d \
./src/math_helper.d \
./src/menu.d \
./src/stm32f7xx_it.d \
./src/syscalls.d \
./src/system_stm32f7xx.d \
./src/ts_calibration.d \
./src/usbh_conf.d \
./src/waveplayer.d \
./src/waverecorder.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -DSTM32 '-D__FPU_PRESENT = 1' -DARM_MATH_CM7 -DSTM32F7 -DSTM32F723IEKx -DSTM32F723E_DISCO -DDEBUG -DSTM32F723xx -DUSE_HAL_DRIVER -DINCLUDE_FATFS -DUSE_USB_FS -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/Utilities/STM32F723E-Discovery" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/Utilities/Components/st7789h2" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/Utilities/Components/mx25l512" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/CMSIS/device" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/Utilities/Components/ft6x06" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/HAL_Driver/Inc/Legacy" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/Utilities/Components/Common" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/Middlewares/Third_Party/FatFs/src" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/HAL_Driver/Inc" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/Utilities/Components/wm8994" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/Utilities/Fonts" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/inc" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/Utilities/Log" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/piotr/OneDrive/Pulpit/Eclipse/test4/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


