firmware.elf: main.o
	arm-none-eabi-gcc -T src/link.ld -nostdlib main.o -o firmware.elf

main.o: src/main.c
	arm-none-eabi-gcc -mcpu=cortex-m4 src/main.c -c -Wall -Wextra -Werror -pedantic -Wno-main

flash: firmware.bin
	st-flash --reset write firmware.bin 0x8000000

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary firmware.elf firmware.bin

clean:
	rm firmware.* *.o
