#ifndef __STM32L476R_NUCLEO_ULTRASONIC_H
#define __STM32L476R_NUCLEO_ULTRASONIC_H

double sensor_data_to_hertz(unsigned char dataH, unsigned char dataL);
void ultrasonic_quick_sample(void);
void ultrasonic_pin_init(void);
void ultrasonic_average_sample(void);


#endif
