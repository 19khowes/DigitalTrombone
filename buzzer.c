#include "stm32l476xx.h"
#include "buzzer.h"

#define CLOCK_FREQ 16000000 

#define BUZZER_PORT GPIOB
#define RCC_AHB2ENR_BUZZER_PORTEN RCC_AHB2ENR_GPIOBEN 

#define BUZZER_PIN 12


/**
 * @brief Set up systick 
 * 
 * Disable SysTick
 * Set reload register
 * Set interrupt priority of SysTick to least urgency (i.e., largest priority value)
 * Reset the SysTick counter value
 * Select processor clock: 1 = processor clock; 0 = external clock
 * Enables SysTick interrupt, 1 = Enable, 0 = Disable
 * Enable SysTick
 * 
 * @param ticks 
 */
void SysTick_Initialize(unsigned int ticks) {
    SysTick->CTRL = 0;            // Disable SysTick
    SysTick->LOAD = ticks;    // Set reload register

    // Set interrupt priority of SysTick to least urgency (i.e., largest priority value)
    NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);

    SysTick->VAL = 0;             // Reset the SysTick counter value

    // Select processor clock: 1 = processor clock; 0 = external clock
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    // Enables SysTick interrupt, 1 = Enable, 0 = Disable
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

    // Enable SysTick
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/**
 * @brief Set up systick 
 * 
 * Reset the SysTick counter value
 * 
 * @param ticks 
 */
void SysTick_Load(unsigned int ticks) {
    SysTick->LOAD = ticks;    // Set reload register
}


/** 
BEGIN
	Enable clock for buzzer port
	Set buzzer pin to output mode 
END
*/
void buzzer_init(void) {
		// Enable clock for buzzer port
    RCC->AHB2ENR |= RCC_AHB2ENR_BUZZER_PORTEN;
		// set buzzer pin to output mode (BUZZER_PIN)
		BUZZER_PORT->MODER &= ~(0x3U << (BUZZER_PIN * 2));
    BUZZER_PORT->MODER |= (0x1U << (BUZZER_PIN * 2));
}

/** 
Desc: takes in a frequency and computes the SysTick Load value 
which will sound the buzzer at that frequency
BEGIN
	Clock frequency / Desired frequency - 1 is the systick load value
	Store load value into SysTick load
END
*/
void hertz_to_load(double freq) {
	// Clock frequency / Desired frequency - 1 is the systick load value
	unsigned int freqLoad = (unsigned int)(CLOCK_FREQ / freq) - 1;
	// Store load value into SysTick load
	SysTick_Load(freqLoad);
}
/**
BEGIN
	IF buzzer pin is 0
	Then
		Set buzzer pin to 1
	Else
		Set buzzer pin to 1
END
*/
void toggle_buzzer_pin() {
	// if buzzer pin is 0
	if ((BUZZER_PORT->ODR & (0x1U << BUZZER_PIN)) == 0) {
		// set buzzer pin to 1
		BUZZER_PORT->ODR |= (0x1U << BUZZER_PIN);
	} else {
		// set buzzer pin to 0
		BUZZER_PORT->ODR &= ~(0x1U << BUZZER_PIN);
	}
}

void synthesize(void) {
// read from UART continuously
	uint8_t msgRecieve;
	double audFreq = 100;
	while(1) {
		USART_Read(USART2, &msgRecieve, 1);
		if (msgRecieve >= 'a' && msgRecieve <= 'g') {
			switch (msgRecieve) {
				case 'a': 
					audFreq = 440;
					break;
				case 'b': 
					audFreq = 493.883;
					break;
				case 'c': 
					audFreq = 523.251;
					break;
				case 'd': 
					audFreq = 587.330;
					break;
				case 'e': 
					audFreq = 659.255;
					break;
				case 'f': 
					audFreq = 698.456;
					break;
				case 'g': 
					audFreq = 783.991;
					break;
			}
			hertz_to_load(audFreq);
		}
	}
}