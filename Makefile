SOURCES = main.c gpio.c
OBJECTS = $(SOURCES:.c=.o)

firmware.elf: $(OBJECTS)
	arm-none-eabi-gcc -T src/link.ld -nostdlib $(OBJECTS) -o firmware.elf

%.o: src/%.c
	arm-none-eabi-gcc -mcpu=cortex-m4 $< -c -Wall -Wextra -Werror -pedantic -Wno-main

flash: firmware.bin
	st-flash --reset write firmware.bin 0x8000000

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary firmware.elf firmware.bin

clean:
	rm -f firmware.* *.o
