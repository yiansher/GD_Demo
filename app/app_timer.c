#include "app_timer.h"
#include "app_uart.h"

extern void led_write(rt_base_t pin, rt_base_t value);

static void led_alive(void * parm)
{
//    char *str = (char*)parm;
    static int i = 0;
    if(i==0)
    led_write(LED0_PIN, PIN_HIGH);
    else
    led_write(LED0_PIN, PIN_LOW);
    i=1-i;
//    APP_TRACE("%s",str);
}

timer_counter_t timer_counters[TC_NUM] = {
{1000,CNT_0,IS_FUNC,"1s counter",led_alive},
{10,CNT_0,NO_FUNC,"10ms_tc",NULL}
};

/*give up*/
#if 0
timer_counter_node_t tc_1s;
static timer_counter_node_t* timer_counter_createList()
{
    timer_counter_node_t* head = (timer_counter_node_t*)malloc(sizeof(timer_counter_node_t));
    if (head == NULL){
        APP_ERROR("timer_counter_List create fail");
        exit(1);
    }
    head->next = NULL;
    return head;
}

static timer_counter_t* node_data_init(uint16_t delay_time_in)
{
    timer_counter_t *node = (timer_counter_t*)malloc(sizeof(timer_counter_t));
    if(node == NULL)
    {
        APP_ERROR("node.data init fail");
        exit(1);
    }
    node->delay_time = delay_time_in;
    node->cnt = 0;
    node->flag = 0;
    return node;
}

static void timer_counter_addToList()
{
    
}
#endif

static void timer_counter_plusplus(timer_counter_t *timer_counter_list)
{
    uint8_t i = 0;
    for(i=0;i<TC_NUM;i++)
    {
        if(timer_counter_list[i].flag & FLAG_SET)
            continue;
        timer_counter_list[i].cnt+=1;
        if(timer_counter_list[i].cnt<timer_counter_list[i].delay_time)
            continue;
        else
        {
            if(! (timer_counter_list[i].flag & IS_FUNC) )
                timer_counter_list[i].flag |= 0x01;
            else
            {
                void (*cbFunction)(const char *) = (void (*)(const char *))timer_counter_list[i].cbFunction;
                cbFunction(timer_counter_list[i].tcName);
            }
            timer_counter_list[i].cnt = 0;
        }
    }
}

void TIMER1_IRQHandler(void)
{
    if(SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
        timer_counter_plusplus(timer_counters);
    }
}

