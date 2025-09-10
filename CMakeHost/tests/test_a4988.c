#include <assert.h>
#include <stdio.h>
#include "Motor/a4988.h"

static bool dir_state, en_state, ms1_state, ms2_state, ms3_state; static int steps;
static void dir_write(bool l){dir_state=l;} static void step_pulse(void){steps++;}
static void en_write(bool l){en_state=l;} static void ms1_write(bool l){ms1_state=l;}
static void ms2_write(bool l){ms2_state=l;} static void ms3_write(bool l){ms3_state=l;}

int main(void){
    a4988_t d; a4988_pins_t p={dir_write,step_pulse,en_write,ms1_write,ms2_write,ms3_write};
    a4988_init(&d,&p);
    assert(!dir_state && !en_state && !ms1_state && !ms2_state && !ms3_state);
    a4988_set_dir(&d,true); assert(dir_state);
    a4988_set_microstep(&d,A4988_EIGHTH); assert(ms1_state && ms2_state && !ms3_state);
    a4988_step(&d); a4988_step(&d); assert(steps==2);
    a4988_enable(&d,false); assert(en_state);
    printf("[a4988] testes OK\n");
    return 0;
}
