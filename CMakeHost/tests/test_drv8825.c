#include <assert.h>
#include <stdio.h>
#include "Motor/drv8825.h"

static bool dir_state,en_state,ms1_state,ms2_state,ms3_state; static int steps;
static void dir_write(bool l){dir_state=l;} static void step_pulse(void){steps++;}
static void en_write(bool l){en_state=l;} static void ms1_write(bool l){ms1_state=l;}
static void ms2_write(bool l){ms2_state=l;} static void ms3_write(bool l){ms3_state=l;}

int main(void){
    drv8825_t d; drv8825_pins_t p={dir_write,step_pulse,en_write,ms1_write,ms2_write,ms3_write};
    drv8825_init(&d,&p);
    assert(!dir_state && !en_state && !ms1_state && !ms2_state && !ms3_state);
    drv8825_set_microstep(&d,DRV8825_THIRTYSECOND); assert(ms1_state && !ms2_state && ms3_state);
    drv8825_set_dir(&d,true); assert(dir_state);
    drv8825_step(&d); assert(steps==1);
    drv8825_enable(&d,false); assert(en_state);
    printf("[drv8825] testes OK\n");
    return 0;
}
