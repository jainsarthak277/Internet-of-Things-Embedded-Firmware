################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/kit/common/bsp/bsp_stk.c 

OBJS += \
./hardware/kit/common/bsp/bsp_stk.o 

C_DEPS += \
./hardware/kit/common/bsp/bsp_stk.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/kit/common/bsp/bsp_stk.o: ../hardware/kit/common/bsp/bsp_stk.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DHAL_CONFIG=1' '-D__StackLimit=0x20000000' '-D__HEAP_SIZE=0xD00' '-D__STACK_SIZE=0x800' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\emdrv\uartdrv\inc" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\halconfig\inc\hal-config" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\bootloader\api" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\hardware\kit\common\drivers" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\app\bluetooth\common\stack_bridge" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\radio\rail_lib\common" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\emdrv\sleep\inc" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\protocol\bluetooth\ble_stack\inc\common" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\emdrv\common\inc" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\protocol\bluetooth\ble_stack\inc\soc" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\emdrv\sleep\src" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\emlib\src" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\radio\rail_lib\chip\efr32" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\emlib\inc" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\bootloader" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\CMSIS\Include" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\hardware\kit\common\bsp" -I"C:\Users\jains\SimplicityStudio\v4_workspace\LETIMER_0\hardware\kit\common\halconfig" -O1 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"hardware/kit/common/bsp/bsp_stk.d" -MT"hardware/kit/common/bsp/bsp_stk.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

