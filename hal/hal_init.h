#ifndef __HAL_INIT__
#define __HAL_INIT__

#include "gd32f10x.h"
#include "hal_gpio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define UART_COM    USART0

#define USER_MAIN_DEBUG 1
#ifdef USER_MAIN_DEBUG
#define	APP_ERROR(fmt, ...) do {printf("[ERROR][%s,%d]: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);} while(0)
#define	APP_TRACE(fmt, ...) do {printf("[TRACE][%s,%d]: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);} while(0)
#define	APP_DEBUG(fmt, ...) do {printf("[DEBUG][%s,%d]: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);} while(0)
#else
#define	APP_ERROR(fmt, ...)
#define	APP_TRACE(fmt, ...)
#define	APP_DEBUG(fmt, ...)
#endif

void hal_init(void);
/* delay a time in milliseconds */
void delay_1ms(uint32_t count);
/* delay decrement */
void delay_decrement(void);

#endif

