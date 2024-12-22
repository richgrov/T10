#include "rcc.h"

typedef struct {
   volatile uint32_t clock_ctl;
   volatile uint32_t config;
   volatile uint32_t clock_config;
   volatile uint32_t clock_interrupt;
   volatile uint32_t ahb1_reset;
   volatile uint32_t ahb2_reset;
   volatile uint32_t ahb3_reset;
   volatile uint32_t _reserved1;
   volatile uint32_t apb1_reset;
   volatile uint32_t apb2_reset;
   volatile uint32_t _reserved2;
   volatile uint32_t _reserved3;
   volatile uint32_t ahb1_enable;
   volatile uint32_t ahb2_enable;
   volatile uint32_t ahb3_enable;
   volatile uint32_t _reserved4;
   volatile uint32_t apb1en;
   volatile uint32_t apb2en;
} ResetAndClockControl;

#define RCC ((volatile ResetAndClockControl *)0x40023800)

void rcc_ahb1_enable(uint32_t enable) {
   RCC->ahb1_enable |= enable;
}

void rcc_apb1_enable(uint32_t enable) {
   RCC->apb1en |= enable;
}

void rcc_apb2_enable(uint32_t enable) {
   RCC->apb2en |= enable;
}
