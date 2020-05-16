################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include protocol/bluetooth/lib/EFR32XG13X/GCC/subdir.mk
-include protocol/bluetooth/bt_mesh/src/subdir.mk
-include platform/emlib/src/subdir.mk
-include platform/emdrv/sleep/src/subdir.mk
-include platform/emdrv/gpiointerrupt/src/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG13P/Source/subdir.mk
-include lcdGraphics/glib/subdir.mk
-include lcdGraphics/drivers/subdir.mk
-include lcdGraphics/dmd/subdir.mk
-include lcdGraphics/subdir.mk
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
all: IoT_FP_Parker.axf

# Tool invocations
IoT_FP_Parker.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "C:\Users\jains\SimplicityStudio\v4_workspace\IoT_FP_Parker\efr32bg13p632f512gm48.ld" -Wl,--undefined,bg_application_properties,--undefined,__Vectors,--undefined,__aeabi_uldivmod,--undefined,ceil -Xlinker -no-enum-size-warning -Xlinker -no-wchar-size-warning -Xlinker --gc-sections -Xlinker -Map="IoT_FP_Parker.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o IoT_FP_Parker.axf -Wl,--start-group "./platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.o" "./calendar.o" "./dcd.o" "./gatt_db.o" "./init_app.o" "./init_board.o" "./init_mcu.o" "./main.o" "./pti.o" "./hardware/kit/common/bsp/bsp_stk.o" "./hardware/kit/common/drivers/mx25flash_spi.o" "./hardware/kit/common/drivers/retargetio.o" "./hardware/kit/common/drivers/retargetserial.o" "./hardware/kit/common/drivers/udelay.o" "./lcdGraphics/dmd/dmd_display.o" "./lcdGraphics/drivers/display.o" "./lcdGraphics/drivers/displayls013b7dh03.o" "./lcdGraphics/drivers/displaypalemlib.o" "./lcdGraphics/glib/bmp.o" "./lcdGraphics/glib/glib.o" "./lcdGraphics/glib/glib_bitmap.o" "./lcdGraphics/glib/glib_circle.o" "./lcdGraphics/glib/glib_font_narrow_6x8.o" "./lcdGraphics/glib/glib_font_normal_8x8.o" "./lcdGraphics/glib/glib_font_number_16x20.o" "./lcdGraphics/glib/glib_line.o" "./lcdGraphics/glib/glib_polygon.o" "./lcdGraphics/glib/glib_rectangle.o" "./lcdGraphics/glib/glib_string.o" "./lcdGraphics/graphics.o" "./lcdGraphics/lcd_driver.o" "./platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.o" "./platform/emdrv/gpiointerrupt/src/gpiointerrupt.o" "./platform/emdrv/sleep/src/sleep.o" "./platform/emlib/src/em_cmu.o" "./platform/emlib/src/em_core.o" "./platform/emlib/src/em_cryotimer.o" "./platform/emlib/src/em_crypto.o" "./platform/emlib/src/em_emu.o" "./platform/emlib/src/em_gpio.o" "./platform/emlib/src/em_i2c.o" "./platform/emlib/src/em_msc.o" "./platform/emlib/src/em_rmu.o" "./platform/emlib/src/em_rtcc.o" "./platform/emlib/src/em_system.o" "./platform/emlib/src/em_timer.o" "./platform/emlib/src/em_usart.o" "./protocol/bluetooth/bt_mesh/src/bg_application_properties.o" "./protocol/bluetooth/bt_mesh/src/mesh_lib.o" "./protocol/bluetooth/bt_mesh/src/mesh_serdeser.o" "./src/CCS811.o" "./src/cmu.o" "./src/gpio.o" "./src/i2c0_tempsense.o" "./src/rtcc.o" "C:\Users\jains\SimplicityStudio\v4_workspace\IoT_FP_Parker\protocol\bluetooth\lib\EFR32XG13X\GCC\libcoex.a" "C:\Users\jains\SimplicityStudio\v4_workspace\IoT_FP_Parker\protocol\bluetooth\lib\EFR32XG13X\GCC\libbluetooth_mesh.a" "C:\Users\jains\SimplicityStudio\v4_workspace\IoT_FP_Parker\protocol\bluetooth\lib\EFR32XG13X\GCC\binapploader.o" -lm -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: IoT_FP_Parker.hex'
	arm-none-eabi-objcopy -O ihex "IoT_FP_Parker.axf" "IoT_FP_Parker.hex"
	@echo ' '

	@echo 'Building bin file: IoT_FP_Parker.bin'
	arm-none-eabi-objcopy -O binary "IoT_FP_Parker.axf" "IoT_FP_Parker.bin"
	@echo ' '

	@echo 'Building s37 file: IoT_FP_Parker.s37'
	arm-none-eabi-objcopy -O srec "IoT_FP_Parker.axf" "IoT_FP_Parker.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "IoT_FP_Parker.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) IoT_FP_Parker.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
