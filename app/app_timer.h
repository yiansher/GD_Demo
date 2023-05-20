#ifndef __APP_TIMER_H__
#define __APP_TIMER_H__

#include "hal_init.h"

typedef struct{
    uint16_t delay_time;
    uint16_t cnt;
    uint8_t flag;   /* bit:0 --> if cnt>delay, bit:4 --> if function */
    const char * const tcName;
    void *cbFunction;
}timer_counter_t;

#define CNT_0 0u
#define IS_FUNC 0x10
#define NO_FUNC 0x00
#define FLAG_SET  0x01
    
typedef struct node {
    timer_counter_t tc_data;   
    struct node* next;    
}timer_counter_node_t;

#define TC_NUM  2

#endif