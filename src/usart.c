#include "usart.h"

#include <stdint-gcc.h>

#include "config.h"
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
      rcc_apb1_enable(APB1_UART2_ENABLE);
      break;
   }

   volatile Usart *usart = USART_TABLE[usart_num - 1];
   usart->control1 = 0;
   usart->baud_rate = CLOCK_SPEED / baud;
   usart->control1 = USART_CONTROL1_RECEIVER_ENABLE | USART_CONTROL1_TRANSMITTER_ENABLE |
                     USART_CONTROL1_USART_ENABLE;
}

void usart_write(uint8_t usart_num, const uint8_t *buf, uint32_t len) {
   volatile Usart *usart = USART_TABLE[usart_num - 1];
   for (uint32_t i = 0; i < len; ++i) {
      usart->data = buf[i];

      while ((usart->status & USART_DATA_TRANSMIT_DATA_TRANSFERRED) == 0) {
         asm("nop");
      }
   }
}
