#include <assert.h>
#include <stdio.h>
#include "Motor/drv8871.h"

static bool in1_state,in2_state,sleep_state; static uint8_t pwm_duty;
static void in1_write(bool l){in1_state=l;} static void in2_write(bool l){in2_state=l;}
static void pwm_write(uint8_t d){pwm_duty=d;} static void sleep_write(bool l){sleep_state=l;}

int main(void){
    drv8871_t d; drv8871_pins_t p={in1_write,in2_write,pwm_write,sleep_write};
    drv8871_init(&d,&p);
    assert(!in1_state && !in2_state && pwm_duty==0 && sleep_state);
    drv8871_set(&d,DRV8871_FORWARD); assert(in1_state && !in2_state);
    drv8871_set_speed(&d,180); assert(pwm_duty==180);
    drv8871_sleep(&d,true); assert(!sleep_state);
    printf("[drv8871] testes OK\n");
    return 0;
}
