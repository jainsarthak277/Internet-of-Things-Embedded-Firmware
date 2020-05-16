################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include protocol/bluetooth/lib/EFR32BG13P/GCC/subdir.mk
-include platform/emlib/src/subdir.mk
-include platform/emdrv/sleep/src/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG13P/Source/subdir.mk
-include hardware/kit/common/drivers/subdir.mk
-include hardware/kit/common/bsp/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Blinky_SDK_2_9.axf

# Tool invocations
Blinky_SDK_2_9.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "C:\Users\jains\SimplicityStudio\v4_workspace\Blinky_SDK_2_9\efr32bg13p632f512gm48.ld" -Xlinker --gc-sections -Xlinker -Map="Blinky_SDK_2_9.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o Blinky_SDK_2_9.axf -Wl,--start-group "./application_properties.o" "./gatt_db.o" "./init_app.o" "./init_board.o" "./init_mcu.o" "./main.o" "./pti.o" "./hardware/kit/common/bsp/bsp_stk.o" "./hardware/kit/common/drivers/i2cspm.o" "./hardware/kit/common/drivers/mx25flash_spi.o" "./hardware/kit/common/drivers/si7013.o" "./hardware/kit/common/drivers/tempsens.o" "./hardware/kit/common/drivers/udelay.o" "./platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.o" "./platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.o" "./platform/emdrv/sleep/src/sleep.o" "./platform/emlib/src/em_cmu.o" "./platform/emlib/src/em_core.o" "./platform/emlib/src/em_cryotimer.o" "./platform/emlib/src/em_crypto.o" "./platform/emlib/src/em_emu.o" "./platform/emlib/src/em_gpio.o" "./platform/emlib/src/em_i2c.o" "./platform/emlib/src/em_msc.o" "./platform/emlib/src/em_rmu.o" "./platform/emlib/src/em_rtcc.o" "./platform/emlib/src/em_system.o" "./platform/emlib/src/em_timer.o" "./platform/emlib/src/em_usart.o" "./src/cmu.o" "./src/gpio.o" "C:\Users\jains\SimplicityStudio\v4_workspace\Blinky_SDK_2_9\platform\radio\rail_lib\autogen\librail_release\librail_efr32xg13_gcc_release.a" "C:\Users\jains\SimplicityStudio\v4_workspace\Blinky_SDK_2_9\protocol\bluetooth\lib\EFR32BG13P\GCC\binapploader.o" "C:\Users\jains\SimplicityStudio\v4_workspace\Blinky_SDK_2_9\protocol\bluetooth\lib\EFR32BG13P\GCC\libbluetooth.a" "C:\Users\jains\SimplicityStudio\v4_workspace\Blinky_SDK_2_9\protocol\bluetooth\lib\EFR32BG13P\GCC\libpsstore.a" "C:\Users\jains\SimplicityStudio\v4_workspace\Blinky_SDK_2_9\protocol\bluetooth\lib\EFR32BG13P\GCC\libmbedtls.a" -lm -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: Blinky_SDK_2_9.hex'
	arm-none-eabi-objcopy -O ihex "Blinky_SDK_2_9.axf" "Blinky_SDK_2_9.hex"
	@echo ' '

	@echo 'Building bin file: Blinky_SDK_2_9.bin'
	arm-none-eabi-objcopy -O binary "Blinky_SDK_2_9.axf" "Blinky_SDK_2_9.bin"
	@echo ' '

	@echo 'Building s37 file: Blinky_SDK_2_9.s37'
	arm-none-eabi-objcopy -O srec "Blinky_SDK_2_9.axf" "Blinky_SDK_2_9.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "Blinky_SDK_2_9.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Blinky_SDK_2_9.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
