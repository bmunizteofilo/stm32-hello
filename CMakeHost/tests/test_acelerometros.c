#include <assert.h>
#include "Acelerometros/mpu6050.h"
#include "Acelerometros/lis3dh.h"
#include "Acelerometros/icm20948.h"
#include "Acelerometros/adxl345.h"
#include "Acelerometros/lsm6ds3.h"
#include "Acelerometros/bmi160.h"
#include "Acelerometros/bma280.h"
#include "Acelerometros/adxl362.h"

int main(void) {
    (void)mpu6050_init;
    (void)mpu6050_read;
    (void)mpu6050_example;
    (void)lis3dh_init;
    (void)lis3dh_read;
    (void)lis3dh_example;
    (void)icm20948_init;
    (void)icm20948_read;
    (void)icm20948_example;
    (void)adxl345_init;
    (void)adxl345_read;
    (void)adxl345_example;
    (void)lsm6ds3_init;
    (void)lsm6ds3_read;
    (void)lsm6ds3_example;
    (void)bmi160_init;
    (void)bmi160_read;
    (void)bmi160_example;
    (void)bma280_init;
    (void)bma280_read;
    (void)bma280_example;
    (void)adxl362_init;
    (void)adxl362_read;
    (void)adxl362_example;
    return 0;
}
