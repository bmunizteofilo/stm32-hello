#include <assert.h>
#include "Sensores/dht11.h"
#include "Sensores/dht22.h"
#include "Sensores/sht31.h"
#include "Sensores/sht21.h"
#include "Sensores/bme280.h"
#include "Sensores/bme680.h"
#include "Sensores/htu21d.h"
#include "Sensores/si7021.h"
#include "Sensores/am2320.h"
#include "Sensores/sht11.h"

int main(void) {
    (void)dht11_init;
    (void)dht11_read;
    (void)dht11_example;
    (void)dht22_init;
    (void)dht22_read;
    (void)dht22_example;
    (void)sht31_init;
    (void)sht31_read;
    (void)sht31_example;
    (void)sht21_init;
    (void)sht21_read;
    (void)sht21_example;
    (void)bme280_init;
    (void)bme280_read;
    (void)bme280_example;
    (void)bme680_init;
    (void)bme680_read;
    (void)bme680_example;
    (void)htu21d_init;
    (void)htu21d_read;
    (void)htu21d_example;
    (void)si7021_init;
    (void)si7021_read;
    (void)si7021_example;
    (void)am2320_init;
    (void)am2320_read;
    (void)am2320_example;
    (void)sht11_init;
    (void)sht11_read;
    (void)sht11_example;
    return 0;
}
