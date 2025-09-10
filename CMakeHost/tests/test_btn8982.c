#include <assert.h>
#include <stdio.h>
#include "Motor/btn8982.h"

static bool in1_state,in2_state,en_state; static uint8_t pwm_duty;
static void in1_write(bool l){in1_state=l;} static void in2_write(bool l){in2_state=l;}
static void pwm_write(uint8_t d){pwm_duty=d;} static void en_write(bool l){en_state=l;}

int main(void){
    btn8982_t d; btn8982_pins_t p={in1_write,in2_write,pwm_write,en_write};
    btn8982_init(&d,&p);
    assert(!in1_state && !in2_state && pwm_duty==0 && en_state);
    btn8982_set(&d,BTN8982_FORWARD); assert(in1_state && !in2_state);
    btn8982_set_speed(&d,150); assert(pwm_duty==150);
    btn8982_enable(&d,false); assert(!en_state);
    printf("[btn8982] testes OK\n");
    return 0;
}
