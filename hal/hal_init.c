#include "hal_init.h"

static void sysclk_init(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    /* select IRC8M as system clock source, deinitialize the RCU */
    rcu_system_clock_source_config(RCU_CKSYSSRC_IRC8M);
    rcu_deinit();
    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;
    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do
    {
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    } while ((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));

    rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV1);

    /* select HXTAL/2 as clock source */
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PREDV0);
    RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_CFG0_PREDV0);

    /* CK_PLL = (CK_HXTAL/2) * 9 = 36 MHz */
    RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
    RCU_CFG0 |= RCU_PLL_MUL27;

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while (0U == (RCU_CTL & RCU_CTL_PLLSTB))
    {
    }

    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLL;

    /* wait until PLL is selected as system clock */
    while (RCU_SCSS_PLL != (RCU_CFG0 & RCU_CFG0_SCSS))
    {
    }

    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        /* capture error */
        while (1)
        {
        }
    }

    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}
volatile static uint32_t delay;
void delay_1ms(uint32_t count)
{
    delay = count;

    while (0U != delay)
    {
    }
}
void delay_decrement(void)
{
    if (0U != delay)
    {
        delay--;
    }
}

static void hal_rcu_init(void)
{
    /* AHB = SYSCLK */
    rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV1);
    /* APB1 = AHB/2 */
    rcu_apb1_clock_config(RCU_APB1_CKAHB_DIV2);
    /* APB2 = AHB/1 */
    rcu_apb2_clock_config(RCU_APB2_CKAHB_DIV1);
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* enable AF clock */
    rcu_periph_clock_enable(RCU_AF);
    /* enable SPI0 clock */
    rcu_periph_clock_enable(RCU_SPI0);
    /* enable DMA0 clock */
    rcu_periph_clock_enable(RCU_DMA0);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);
}

static void hal_uart_init(void)
{

    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    /* USART0 remapping */
    gpio_pin_remap_config(GPIO_USART0_REMAP, ENABLE);
    /* connect port to USARTx_Tx */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

    /* USART configure */
    usart_deinit(UART_COM);
    usart_baudrate_set(UART_COM, 115200U);
    usart_word_length_set(UART_COM, USART_WL_8BIT);
    usart_stop_bit_set(UART_COM, USART_STB_1BIT);
    usart_parity_config(UART_COM, USART_PM_NONE);
    usart_hardware_flow_rts_config(UART_COM, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(UART_COM, USART_CTS_DISABLE);
    usart_receive_config(UART_COM, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART_COM, USART_TRANSMIT_ENABLE);
    usart_enable(UART_COM);
    
    usart_interrupt_enable( UART_COM, USART_INT_RBNE ); 
    nvic_irq_enable( USART0_IRQn, 0, 0 );
}


static void hal_timer_init(void)
{
    /* ----------------------------------------------------------------------------
    TIMER1 Configuration: 
    TIMER1CLK = SystemCoreClock/10800 = 10KHz, the period is 1s(period/TIMER1CLK = 1s).
    ---------------------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER1);

    timer_deinit(TIMER1);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 10800-1;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 10-1;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER1, &timer_initpara);

    timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    timer_enable(TIMER1);
    
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(TIMER1_IRQn, 1, 1);
}

void hal_init(void)
{
    sysclk_init();
    hal_rcu_init();
    hal_gpio_init();
    hal_uart_init();
    hal_timer_init();
}
