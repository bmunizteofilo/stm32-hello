#include <assert.h>
#include <stdio.h>
#include "Motor/vnh2sp30.h"

static bool ina_state,inb_state,en_state; static uint8_t pwm_duty;
static void ina_write(bool l){ina_state=l;} static void inb_write(bool l){inb_state=l;}
static void pwm_write(uint8_t d){pwm_duty=d;} static void en_write(bool l){en_state=l;}

int main(void){
    vnh2sp30_t d; vnh2sp30_pins_t p={ina_write,inb_write,pwm_write,en_write};
    vnh2sp30_init(&d,&p);
    assert(!ina_state && !inb_state && pwm_duty==0 && en_state);
    vnh2sp30_set(&d,VNH2SP30_REVERSE); assert(!ina_state && inb_state);
    vnh2sp30_set_speed(&d,210); assert(pwm_duty==210);
    vnh2sp30_enable(&d,false); assert(!en_state);
    printf("[vnh2sp30] testes OK\n");
    return 0;
}
