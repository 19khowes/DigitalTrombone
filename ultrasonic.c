#include "stm32l476xx.h"
#include "ultrasonic.h"

#define ULTRA_PORT GPIOB
#define ULTRA_TX_PIN 10

/**
	Compute the mm from the dataH and dataL value from ultrasonic sensor
	Return the frequency mapped from the mm range 
*/
double sensor_data_to_hertz(unsigned char dataH, unsigned char dataL) {
	unsigned int mm = (256 * dataH) + dataL;
	return (mm-30)*(-0.12097) + 123.471;
}

/**
*/
void ultrasonic_pin_init() {
	// set ultra tx pin to output mode
	ULTRA_PORT->MODER &= ~(0x3U << (ULTRA_TX_PIN * 2));
  ULTRA_PORT->MODER |= (0x1U << (ULTRA_TX_PIN * 2));
	
	// Set ultra TX pin to push-pull
	ULTRA_PORT->OTYPER &= ~(0x1U << ULTRA_TX_PIN);
	
}

/**
*/
void ultrasonic_quick_sample() {
	ULTRA_PORT->ODR &= ~(0x1U << ULTRA_TX_PIN);
}

void ultrasonic_average_sample() {
	ULTRA_PORT->ODR |= 0x1U << ULTRA_TX_PIN;
}