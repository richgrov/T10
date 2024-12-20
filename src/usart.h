#ifndef REMIX_USART_H_
#define REMIX_USART_H_

#include <stdint-gcc.h>

struct Usart;
typedef struct Usart Usart;

void usart_init(uint8_t usart_num, uint32_t baud);
void usart_write(uint8_t usart_num, const uint8_t *buf, uint32_t len);

#endif // !REMIX_USART_H_
