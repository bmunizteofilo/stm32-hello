#include <assert.h>
#include <stdio.h>
#include "Motor/drv8876.h"

static bool in1_state,in2_state,sleep_state; static uint8_t pwm_duty;
static void in1_write(bool l){in1_state=l;} static void in2_write(bool l){in2_state=l;}
static void pwm_write(uint8_t d){pwm_duty=d;} static void sleep_write(bool l){sleep_state=l;}

int main(void){
    drv8876_t d; drv8876_pins_t p={in1_write,in2_write,pwm_write,sleep_write};
    drv8876_init(&d,&p);
    assert(!in1_state && !in2_state && pwm_duty==0 && sleep_state);
    drv8876_set(&d,DRV8876_REVERSE); assert(!in1_state && in2_state);
    drv8876_set_speed(&d,90); assert(pwm_duty==90);
    drv8876_sleep(&d,true); assert(!sleep_state);
    printf("[drv8876] testes OK\n");
    return 0;
}
