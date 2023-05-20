#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

#include "gd32f10x.h"
#include "pin.h"
#include "rtdef.h"

struct pin_index
{
    int index;
    uint32_t gpio_periph;
    uint32_t pin;
};

#define __GD32VF_PIN(index, gpio, gpio_index)      \
    {                                              \
        index, GPIO##gpio, GPIO_PIN_##gpio_index   \
    }

#define LED0_PIN    20u
#define SPI0_CLK    5u
#define SPI0_MISO   6u
#define SPI0_MOSI   7u
#define SPI0_DC     33u
#define SPI0_CS     34u
#define SPI0_RESET  35u
#define SPI0_BLK    32u

uint8_t hal_gpio_init(void);
void led_write(rt_base_t pin, rt_base_t value);

#endif

