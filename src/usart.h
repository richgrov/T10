#ifndef REMIX_USART_H_
#define REMIX_USART_H_

#include <stdint-gcc.h>

struct Usart;
typedef struct Usart Usart;

#define USART1 ((volatile Usart *)0x40011000)
#define USART2 ((volatile Usart *)0x40004400)
#define USART3 ((volatile Usart *)0x40004800)
#define USART4 ((volatile Usart *)0x40004C00)
#define USART5 ((volatile Usart *)0x40005000)
#define USART6 ((volatile Usart *)0x40011400)

void usart_init(volatile Usart *usart, uint32_t baud);
void usart_write(volatile Usart *usart, const uint8_t *buf, uint32_t len);

#endif // !REMIX_USART_H_
