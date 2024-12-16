#include <stdbool.h>

void main() {}

__attribute__((naked, noreturn)) void _reset() {
   extern long _sbss, _ebss, _sdata, _edata, _sidata;
   for (long *cursor = &_sbss; cursor < &_ebss; ++cursor) {
      *cursor = 0;
   }

   for (long *memory = &_sdata, *flash_data = &_sidata; memory < &_edata;) {
      *memory++ = *flash_data++;
   }

   main();

   while (true) {
   }
}

extern void _estack(void);

__attribute__((section(".vectors"))) void (*const vector_table[16 + 96])(void) = {
   _estack,
   _reset,
};
