#include "stm32l476xx.h"
#include "USART.h"
#include "buzzer.h" 
#include "keypad.h"
#include "ultrasonic.h"

void SysTick_Handler(void);
unsigned char octaveScale = 0;
unsigned char ultraData[4];

/**

Psedocode 
Begin
	Set up GPIO for USART2
	Enable USART2
End
*/
int main(void){
	// Set up GPIO for USART2 and 1
	GPIO_USART_Init();
	// Enable USART2 for USB communication
	USART_Init(USART2);
	// Enable USART1
	USART_Init(USART1);
	// intialize buzzer 
	buzzer_init();
	// Initialize SysTick with 0 (paused)
	SysTick_Initialize(0);
	// Initialize keypad 
	keypad_init();
	// setup ultra sonic tx pin
	ultrasonic_pin_init();
	// Set USART1 TX pin to 0 (low) to quick sample
	ultrasonic_quick_sample();
	// Set USART1 TX pin to 1 (high) to average sample
	//ultrasonic_average_sample();
	
	// read from UART continuously
	uint8_t msgRecieve;
	double audFreq = 65.406;
	while(1) {
		USART_Read(USART1, ultraData, 4);
		// audFreq = sensor_data_to_hertz(0x01, 0x0E);
		audFreq = sensor_data_to_hertz(ultraData[1], ultraData[2]);
		octaveScale = keypad_listen();
		// button pushed
		if (octaveScale != 0) {
			//USART_Write(USART2, ultraData, 4);
			hertz_to_load(octaveScale * audFreq);
		}
		//USART_Read(USART1, ultraData, 1);
	}
}

void SysTick_Handler() {
	if (octaveScale != 0) {
		toggle_buzzer_pin();
	}
}


