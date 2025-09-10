#include <assert.h>
#include <stdio.h>
#include "Motor/tmc2209.h"

static bool dir_state,en_state,ms1_state,ms2_state; static int steps;
static void dir_write(bool l){dir_state=l;} static void step_pulse(void){steps++;}
static void en_write(bool l){en_state=l;} static void ms1_write(bool l){ms1_state=l;} static void ms2_write(bool l){ms2_state=l;}

int main(void){
    tmc2209_t d; tmc2209_pins_t p={dir_write,step_pulse,en_write,ms1_write,ms2_write};
    tmc2209_init(&d,&p);
    assert(!dir_state && !en_state && !ms1_state && !ms2_state);
    tmc2209_set_microstep(&d,TMC2209_QUARTER); assert(!ms1_state && ms2_state);
    tmc2209_set_dir(&d,true); assert(dir_state);
    tmc2209_step(&d); assert(steps==1);
    tmc2209_enable(&d,false); assert(en_state);
    printf("[tmc2209] testes OK\n");
    return 0;
}
