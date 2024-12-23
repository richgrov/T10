#include "usart.h"

#include <stdint-gcc.h>

#include "config.h"
#include "gpio.h"
#include "rcc.h"

struct Usart {
   volatile uint32_t status;
   volatile uint32_t data;
   volatile uint32_t baud_rate;
   volatile uint32_t control1;
   volatile uint32_t control2;
   volatile uint32_t control3;
   volatile uint32_t guard_time_prescaler;
};

static volatile Usart *const USART_TABLE[] = {
   (volatile Usart *)0x40011000, (volatile Usart *)0x40004400, (volatile Usart *)0x40004800,
   (volatile Usart *)0x40004C00, (volatile Usart *)0x40005000, (volatile Usart *)0x40011400,
};

#define USART2_ALT_FUNC 7

typedef enum {
   USART_DATA_TRANSMIT_DATA_TRANSFERRED = (1 << 7),
} UsartData;

typedef enum {
   USART_CONTROL1_RECEIVER_ENABLE = (1 << 2),
   USART_CONTROL1_TRANSMITTER_ENABLE = (1 << 3),
   USART_CONTROL1_USART_ENABLE = (1 << 13),
} UsartControl1;

void usart_init(uint8_t usart_num, uint32_t baud) {
   switch (usart_num) {
   case 2:
      gpio_set_mode(GPIOA, 2, GPIO_MODE_ALT);
      gpio_set_af(GPIOA, 2, USART2_ALT_FUNC);
      gpio_set_mode(GPIOA, 3, GPIO_MODE_ALT);
      gpio_set_af(GPIOA, 3, USART2_ALT_FUNC);
      rcc_apb1_enable(APB1_UART2_ENABLE);
      break;
   }

   volatile Usart *usart = USART_TABLE[usart_num - 1];
   usart->control1 = 0;
   usart->baud_rate = CLOCK_SPEED / baud;
   usart->control1 = USART_CONTROL1_RECEIVER_ENABLE | USART_CONTROL1_TRANSMITTER_ENABLE |
                     USART_CONTROL1_USART_ENABLE;
}

void usart_write(uint8_t usart_num, const uint8_t b) {
   volatile Usart *usart = USART_TABLE[usart_num - 1];
   usart->data = b;
   while ((usart->status & USART_DATA_TRANSMIT_DATA_TRANSFERRED) == 0) {
      asm("nop");
   }
}

void usart_write_u32(uint8_t usart_num, uint32_t num) {
   uint32_t subtract = 1000000000;
   bool digit_printed = false;

   while (subtract > 0) {
      char digit = 0;
      while (num >= subtract) {
         num -= subtract;
         ++digit;
      }
      subtract /= 10;

      if (digit_printed || digit != 0 || subtract == 1) {
         usart_write(usart_num, '0' + digit);
         digit_printed = true;
      }
   }
}

void usart_write_str(uint8_t usart_num, const char *str) {
   for (const char *c = str; *c != 0; ++c) {
      usart_write(usart_num, (uint8_t)*c);
   }
}
