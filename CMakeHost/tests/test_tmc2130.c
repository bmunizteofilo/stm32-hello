#include <assert.h>
#include <stdio.h>
#include "Motor/tmc2130.h"

static bool dir_state,en_state,cs_state,sdi_state; static int steps; static int sck_pulses;
static void dir_write(bool l){dir_state=l;} static void step_pulse(void){steps++;}
static void en_write(bool l){en_state=l;} static void cs_write(bool l){cs_state=l;}
static void sck_write(bool l){ if(l) sck_pulses++; }
static void sdi_write(bool l){sdi_state=l;}

int main(void){
    tmc2130_t d; tmc2130_pins_t p={dir_write,step_pulse,en_write,cs_write,sck_write,sdi_write};
    tmc2130_init(&d,&p);
    assert(!dir_state && !en_state && cs_state && sck_pulses==32);
    tmc2130_set_dir(&d,true); assert(dir_state);
    tmc2130_step(&d); assert(steps==1);
    tmc2130_enable(&d,false); assert(en_state);
    printf("[tmc2130] testes OK\n");
    return 0;
}
