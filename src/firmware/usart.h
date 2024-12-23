#ifndef REMIX_FIRMWARE_USART_H_
#define REMIX_FIRMWARE_USART_H_

#include <stdint-gcc.h>

struct Usart;
typedef struct Usart Usart;

void usart_init(uint8_t usart_num, uint32_t baud);
void usart_write(uint8_t usart_num, uint8_t b);
void usart_write_str(uint8_t usart_num, const char *str);
void usart_write_u32(uint8_t usart_num, uint32_t num);

#endif // !REMIX_FIRMWARE_USART_H_
