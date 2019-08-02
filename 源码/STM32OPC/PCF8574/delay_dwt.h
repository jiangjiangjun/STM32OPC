#ifndef __DELAY_DWT_H
#define __DELAY_DWT_H
#include <stdint.h>
#include "system_stm32f7xx.h"

void bsp_InitDWT(void);
void bsp_DelayUS(uint32_t _ulDelayTime);
void bsp_DelayMS(uint32_t _ulDelayTime);

#endif
