#include <assert.h>
#include <stdio.h>
#include "Motor/drv8833.h"

static bool ain1_state, ain2_state, bin1_state, bin2_state, sleep_state;
static uint8_t pwma_duty, pwmb_duty;

static void ain1_write(bool l){ain1_state=l;}
static void ain2_write(bool l){ain2_state=l;}
static void bin1_write(bool l){bin1_state=l;}
static void bin2_write(bool l){bin2_state=l;}
static void pwma_write(uint8_t d){pwma_duty=d;}
static void pwmb_write(uint8_t d){pwmb_duty=d;}
static void sleep_write(bool l){sleep_state=l;}

int main(void){
    drv8833_t dev; drv8833_pins_t pins={ain1_write,ain2_write,pwma_write,bin1_write,bin2_write,pwmb_write,sleep_write};
    drv8833_init(&dev,&pins);
    assert(!ain1_state && !ain2_state && !bin1_state && !bin2_state);
    assert(pwma_duty==0 && pwmb_duty==0 && sleep_state);
    drv8833_motor_a_set(&dev,DRV8833_FORWARD);
    assert(ain1_state && !ain2_state);
    drv8833_set_speed_a(&dev,100);
    assert(pwma_duty==100);
    drv8833_sleep(&dev,true);
    assert(!sleep_state);
    printf("[drv8833] testes OK\n");
    return 0;
}
