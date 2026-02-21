// test_sensor.c
// Prueba simulada de integración de sensores

#include <stdio.h>

void leer_MAX30102(void){
    printf("MAX30102 OK\n");
}

void leer_IMU(void){
    printf("IMU OK\n");
}

int main(void){
    printf("Test inicial sensores\n");
    leer_MAX30102();
    leer_IMU();
    return 0;
}
