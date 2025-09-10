#include <assert.h>
#include <stdio.h>
#include "Motor/bts7960.h"

static uint8_t rpwm_duty, lpwm_duty; static bool ren_state, len_state;
static void rpwm_write(uint8_t d){rpwm_duty=d;} static void lpwm_write(uint8_t d){lpwm_duty=d;}
static void ren_write(bool l){ren_state=l;} static void len_write(bool l){len_state=l;}

int main(void){
    bts7960_t d; bts7960_pins_t p={rpwm_write,lpwm_write,ren_write,len_write};
    bts7960_init(&d,&p);
    assert(rpwm_duty==0 && lpwm_duty==0 && ren_state && len_state);
    bts7960_set_speed(&d,180);
    bts7960_set(&d,BTS7960_FORWARD);
    assert(rpwm_duty==180 && lpwm_duty==0);
    bts7960_enable(&d,false); assert(!ren_state && !len_state);
    printf("[bts7960] testes OK\n");
    return 0;
}
