#include "main.h"

void app_init(void);

extern timer_counter_t timer_counters[TC_NUM];

int main(int argc, char *argv[])
{
    hal_init();
    app_init();
    
    while(1)
    {
        if(timer_counters[1].flag & 0x01)
        {
            uart_data_deal(&data_packet, &data_deal_complete_flag);
            timer_counters[1].flag &= 0x10;
        }
    }
}

void app_init(void)
{
    app_ringbuffer_init(32);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}


