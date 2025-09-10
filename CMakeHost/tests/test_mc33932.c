#include <assert.h>
#include <stdio.h>
#include "Motor/mc33932.h"

static bool in1_state,in2_state,in3_state,in4_state; static uint8_t ena_duty,enb_duty;
static void in1_write(bool l){in1_state=l;} static void in2_write(bool l){in2_state=l;}
static void in3_write(bool l){in3_state=l;} static void in4_write(bool l){in4_state=l;}
static void ena_write(uint8_t d){ena_duty=d;} static void enb_write(uint8_t d){enb_duty=d;}

int main(void){
    mc33932_t d; mc33932_pins_t p={in1_write,in2_write,ena_write,in3_write,in4_write,enb_write};
    mc33932_init(&d,&p);
    assert(!in1_state && !in2_state && !in3_state && !in4_state);
    mc33932_motor_a_set(&d,MC33932_FORWARD); assert(in1_state && !in2_state);
    mc33932_motor_b_set(&d,MC33932_BRAKE); assert(in3_state && in4_state);
    mc33932_set_speed_a(&d,50); assert(ena_duty==50);
    mc33932_set_speed_b(&d,200); assert(enb_duty==200);
    printf("[mc33932] testes OK\n");
    return 0;
}
