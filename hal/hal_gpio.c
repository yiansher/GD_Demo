#include "hal_gpio.h"
extern void delay_1ms(uint32_t count);

static const struct pin_index pins[] =
{
#if defined(GPIOA)
    __GD32VF_PIN(0,  A, 0),
    __GD32VF_PIN(1,  A, 1),
    __GD32VF_PIN(2,  A, 2),
    __GD32VF_PIN(3,  A, 3),
    __GD32VF_PIN(4,  A, 4),
    __GD32VF_PIN(5,  A, 5),
    __GD32VF_PIN(6,  A, 6),
    __GD32VF_PIN(7,  A, 7),
    __GD32VF_PIN(8,  A, 8),
    __GD32VF_PIN(9,  A, 9),
    __GD32VF_PIN(10,  A, 10),
    __GD32VF_PIN(11,  A, 11),
    __GD32VF_PIN(12,  A, 12),
    __GD32VF_PIN(13,  A, 13),
    __GD32VF_PIN(14,  A, 14),
    __GD32VF_PIN(15,  A, 15),
#if defined(GPIOB)
    __GD32VF_PIN(16,  B, 0),
    __GD32VF_PIN(17,  B, 1),
    __GD32VF_PIN(18,  B, 2),
    __GD32VF_PIN(19,  B, 3),
    __GD32VF_PIN(20,  B, 4),
    __GD32VF_PIN(21,  B, 5),
    __GD32VF_PIN(22,  B, 6),
    __GD32VF_PIN(23,  B, 7),
    __GD32VF_PIN(24,  B, 8),
    __GD32VF_PIN(25,  B, 9),
    __GD32VF_PIN(26,  B, 10),
    __GD32VF_PIN(27,  B, 11),
    __GD32VF_PIN(28,  B, 12),
    __GD32VF_PIN(29,  B, 13),
    __GD32VF_PIN(30,  B, 14),
    __GD32VF_PIN(31,  B, 15),
#if defined(GPIOC)
    __GD32VF_PIN(32,  C, 0),
    __GD32VF_PIN(33,  C, 1),
    __GD32VF_PIN(34,  C, 2),
    __GD32VF_PIN(35,  C, 3),
    __GD32VF_PIN(36,  C, 4),
    __GD32VF_PIN(37,  C, 5),
    __GD32VF_PIN(38,  C, 6),
    __GD32VF_PIN(39,  C, 7),
    __GD32VF_PIN(40,  C, 8),
    __GD32VF_PIN(41,  C, 9),
    __GD32VF_PIN(42,  C, 10),
    __GD32VF_PIN(43,  C, 11),
    __GD32VF_PIN(44,  C, 12),
    __GD32VF_PIN(45,  C, 13),
    __GD32VF_PIN(46,  C, 14),
    __GD32VF_PIN(47,  C, 15),
#if defined(GPIOD)
    __GD32VF_PIN(48,  D, 0),
    __GD32VF_PIN(49,  D, 1),
    __GD32VF_PIN(50,  D, 2),
    __GD32VF_PIN(51,  D, 3),
    __GD32VF_PIN(52,  D, 4),
    __GD32VF_PIN(53,  D, 5),
    __GD32VF_PIN(54,  D, 6),
    __GD32VF_PIN(55,  D, 7),
    __GD32VF_PIN(56,  D, 8),
    __GD32VF_PIN(57,  D, 9),
    __GD32VF_PIN(58,  D, 10),
    __GD32VF_PIN(59,  D, 11),
    __GD32VF_PIN(60,  D, 12),
    __GD32VF_PIN(61,  D, 13),
    __GD32VF_PIN(62,  D, 14),
    __GD32VF_PIN(63,  D, 15),
#if defined(GPIOE)
    __GD32VF_PIN(64,  E, 0),
    __GD32VF_PIN(65,  E, 1),
    __GD32VF_PIN(66,  E, 2),
    __GD32VF_PIN(67,  E, 3),
    __GD32VF_PIN(68,  E, 4),
    __GD32VF_PIN(69,  E, 5),
    __GD32VF_PIN(70,  E, 6),
    __GD32VF_PIN(71,  E, 7),
    __GD32VF_PIN(72,  E, 8),
    __GD32VF_PIN(73,  E, 9),
    __GD32VF_PIN(74,  E, 10),
    __GD32VF_PIN(75,  E, 11),
    __GD32VF_PIN(76,  E, 12),
    __GD32VF_PIN(77,  E, 13),
    __GD32VF_PIN(78,  E, 14),
    __GD32VF_PIN(79,  E, 15),
#endif /* defined(GPIOE) */
#endif /* defined(GPIOD) */
#endif /* defined(GPIOC) */
#endif /* defined(GPIOB) */
#endif /* defined(GPIOA) */
};


#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])
static const struct pin_index *get_pin(uint8_t pin)
{
    const struct pin_index *index;

    if (pin < ITEM_NUM(pins))
    {
        index = &pins[pin];
        if (index->index == -1)
            index = RT_NULL;
    }
    else
    {
        index = RT_NULL;
    }

    return index;
};

static void gd32vf_pin_write(rt_base_t pin, rt_base_t value)
{
    const struct pin_index *index;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return;
    }

    gpio_bit_write(index->gpio_periph, index->pin, (bit_status)value);
}

int gd32vf_pin_read(rt_base_t pin)
{
    int value;
    const struct pin_index *index;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return PIN_LOW;
    }

    value = gpio_input_bit_get(index->gpio_periph, index->pin);
    return value;
}

static void gd32vf_pin_mode(rt_base_t pin, rt_base_t mode)
{
    const struct pin_index *index;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return;
    }

    /* Configure GPIO_InitStructure */
    if (mode == PIN_MODE_OUTPUT)
    {
        /* output setting */
        gpio_init(index->gpio_periph, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, index->pin);
        gpio_bit_reset(index->gpio_periph, index->pin);
    }
    else if (mode == PIN_MODE_INPUT)
    {
        /* input setting: not pull. */
        gpio_init(index->gpio_periph, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, index->pin);
        gpio_bit_reset(index->gpio_periph, index->pin);
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        /* input setting: pull up. */
        gpio_init(index->gpio_periph, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, index->pin);
        gpio_bit_reset(index->gpio_periph, index->pin);
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
        /* input setting: pull down. */
        gpio_init(index->gpio_periph, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, index->pin);
        gpio_bit_reset(index->gpio_periph, index->pin);
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        /* output setting: od. */
        gpio_init(index->gpio_periph, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, index->pin);
        gpio_bit_reset(index->gpio_periph, index->pin);
    }
    else if (mode == PIN_MODE_AF)
    {
        /* output setting: od. */
        gpio_init(index->gpio_periph, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, index->pin);
        gpio_bit_reset(index->gpio_periph, index->pin);
    }
    else if (mode == PIN_MODE_AF_OD)
    {
        /* output setting: od. */
        gpio_init(index->gpio_periph, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, index->pin);
        gpio_bit_reset(index->gpio_periph, index->pin);
    }

}



uint8_t hal_gpio_init(void)
{
    uint8_t ret = 0;
    delay_1ms(100);
    gpio_pin_remap_config(GPIO_SWJ_NONJTRST_REMAP, ENABLE);//PB4管脚默认是NJTRST，要当GPIO，需要重映射
    gd32vf_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    gd32vf_pin_write(LED0_PIN, PIN_LOW);
    delay_1ms(100);
    gd32vf_pin_write(LED0_PIN, PIN_HIGH);
    delay_1ms(100);
    gd32vf_pin_write(LED0_PIN, PIN_LOW);
    ret |= gd32vf_pin_read(LED0_PIN);
    return ret;
}

void led_write(rt_base_t pin, rt_base_t value)
{
    gd32vf_pin_write( pin,  value);
}

