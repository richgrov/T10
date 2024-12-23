SOURCES := firmware/adv_ctl_timer.c \
			  firmware/gpio.c \
			  firmware/rcc.c \
			  firmware/systick.c \
			  firmware/usart.c \
			  main.c \
			  stepper.c

OBJECTS = $(SOURCES:%.c=build/%.o)

build/firmware.elf: $(OBJECTS)
	arm-none-eabi-gcc -T src/link.ld -nostdlib $(OBJECTS) -o build/firmware.elf

build/%.o: src/%.c build/%.d | build/
	arm-none-eabi-gcc -mcpu=cortex-m4 $< -c -o $@ -Isrc -Wall -Wextra -Werror -pedantic -Wno-main -MMD -MT $@ -MF build/$*.d -MP

flash: build/firmware.bin
	st-flash --reset write build/firmware.bin 0x8000000

build/firmware.bin: build/firmware.elf
	arm-none-eabi-objcopy -O binary build/firmware.elf build/firmware.bin

build/:
	@mkdir -p build

clean:
	rm -rf build/

DEPENDENCIES := $(SOURCES:%.c=build/%.d)
$(DEPENDENCIES):
	@mkdir -p $(@D)

include $(wildcard $(DEPENDENCIES))
