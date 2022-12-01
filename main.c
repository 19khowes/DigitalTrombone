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
	Set up GPIO for USART2 and 1
	Enable USART2 for USB communication
	Enable USART1
	intialize buzzer
	Initialize SysTick with 0 (paused)
	Initialize keypad
	setup ultra sonic tx pin
	Set USART1 TX pin to 0 (low) to quick sample
	
	Loop forever
		Read data from ultrasonic sensor
		Convert sensor data to frequency
		Read octave scale from keypad
		If octave scale is not 0
		Then
			Load the SysTick reload with the converted scaled frequency
		Else
		EndIf
	EndLoop
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
		// Read data from ultrasonic sensor
		USART_Read(USART1, ultraData, 4);
		// Convert sensor data to frequency
		audFreq = sensor_data_to_hertz(ultraData[1], ultraData[2]);
		// Read octave scale from keypad
		octaveScale = keypad_listen();
		// If octave scale is not 0
		if (octaveScale != 0) {
			// Load the SysTick reload with the converted scaled frequency
			hertz_to_load(octaveScale * audFreq);
		}
	}
}

void SysTick_Handler() {
	if (octaveScale != 0) {
		toggle_buzzer_pin();
	}
}


