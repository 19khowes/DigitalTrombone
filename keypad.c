#include "keypad.h"
#include "stm32l476xx.h"

#define KEYPAD_PORT GPIOB
#define RCC_AHB2ENR_KEYPAD_PORTEN RCC_AHB2ENR_GPIOBEN 

#define KEYPAD_ROW_OFFSET 0

#define KEYPAD_C0 4
#define KEYPAD_C1 5
#define KEYPAD_C2 8 // red
#define KEYPAD_C3 9	// brown

void delay_ms(unsigned int ms) {
    // cycles = 16000 * ms for delay
		volatile unsigned int i = 0;
		volatile unsigned int cycles = 1600 * ms;
		for (; i < cycles; i++);
}

void delay_10us(unsigned int us) {
    // cycles = 8 * us for delay
	volatile unsigned int i = 0;
	volatile unsigned int cycles = 8 * us;
	for (; i < cycles; i++);
}

/**
 * @brief set up keypad to be ready for listening
 * 
 * Psedocode:
 * Begin
 *  Enable the clock for keypad pin port
 *  Set column pins to input mode
 *  Set row pins to output mode
 *  Set row pins to open-drain
 *  Set row pins to pull up resistors
 * End
 */
void keypad_init(void) {
    // enable clock for GPIO port B
    RCC->AHB2ENR |= RCC_AHB2ENR_KEYPAD_PORTEN;
    // set column pins to input mode
    KEYPAD_PORT->MODER &= ~((0x3U << (KEYPAD_C0*2)) + (0x3U << (KEYPAD_C1*2)) + (0x3U << (KEYPAD_C2*2)) + (0x3U << (KEYPAD_C3*2)));
    // set row pins to output mode
    KEYPAD_PORT->MODER &= 0xFFFFFF00;
    KEYPAD_PORT->MODER |= 0x00000055;
    // set row pins to open-drain
    KEYPAD_PORT->OTYPER |= 0x0000000F;
    // set row pins to pull up resistors
    KEYPAD_PORT->PUPDR &= 0xFFFFFF00;
    KEYPAD_PORT->PUPDR |= 0x00000055;
}

/**
 * @brief listen for keypad button pushes
 * 
 * Pseudocode:
 * Begin
 *  Set counter to 0
 *  Loop forever
 *      Set all row output pins to 0 
 *      Delay x time for ouput to settle
 *      If (any column input is 0)
 *      Then
 *        Delay x time (debounce)
 *        Set columnPressed to be 3,2,1,0 (whichever column was 0)
 *        If columnPressed is 3,2,1,0
 *        Then
 *          Set i = 0
 *          Loop while i < 4
 *            Set all others rows to be 1 (pull up)
 *            Set row i to be 0 (ground)
 *            Delay x time for output to settle
 *            If (any column is 0)
 *            Then 
 *              Set rowPressed to be i
 *              Lookup in table[rowPressed][columnPressed]
 *							If character matches specific stage
 * 							Then
 * 								Write that character to LCD
 *								Increment counter 
 * 								Write command to write on line 2 if counter is 16
 *               	Loop while any column is 0 (button held)
 * 								Return that character
 *								Else
 *              EndLoop 
 *          EndLoop 
 *        Else
 *  EndLoop
 * End
 */
unsigned char keypad_listen(void) {
    unsigned int columnPressed;
    unsigned int rowPressed;
    unsigned int loadMask = ((1 << KEYPAD_C0) + (1 << KEYPAD_C1) + (1 << KEYPAD_C2) + (1 << KEYPAD_C3));
    unsigned char keymap[4][4] = {
        {1, 2, 4, 'A'},
        {8, 16, 32, 'B'},
        {64, 128, 255, 'C'},
        {'*', 0, '#', 'D'}
    };
			// Set all row output pins to 0
			KEYPAD_PORT->ODR &= (0xFFFFFFF0 << KEYPAD_ROW_OFFSET);
			// Delay 30 us time for output to settle
			delay_10us(30);
			// If (any column input is 0)
			if ((KEYPAD_PORT->IDR & loadMask) != loadMask) {
					// some button was pressed
					// delay 500 us
					delay_10us(500);
					columnPressed = 4;
					// Set columnPressed to be 3,2,1,0 (whichever column was 0)
					if (((KEYPAD_PORT->IDR & loadMask) & (1 << KEYPAD_C0)) == 0) {
							columnPressed = 0;
					} 
					if (((KEYPAD_PORT->IDR & loadMask) & (1 << KEYPAD_C1)) == 0) {
							columnPressed = 1;
					}
					if (((KEYPAD_PORT->IDR & loadMask) & (1 << KEYPAD_C2)) == 0) {
							columnPressed = 2;
					}
					if (((KEYPAD_PORT->IDR & loadMask) & (1 << KEYPAD_C3)) == 0) {
							columnPressed = 3;
					}
					// If columnPressed is 3,2,1,0 (still pressed after debounced)
					if (columnPressed != 4) {
							// loop through setting each row to 0 
							for (unsigned char i = 0; i < 4; i++) {
									// Set all others rows to be 1 (pull up)
									KEYPAD_PORT->ODR |= (0xF << KEYPAD_ROW_OFFSET);
									// Set row i to be 0 (grounded)
									KEYPAD_PORT->ODR &= ~((1 << i) + KEYPAD_ROW_OFFSET);
									// Delay 30 us time for output to settle
									delay_10us(30);
									// If (any column input is 0)
									if ((KEYPAD_PORT->IDR & loadMask) != loadMask) {
											// Set rowPressed to be i
											rowPressed = i;
											// return table[rowPressed][columnPressed]
											return keymap[rowPressed][columnPressed];
									}
							}
					}
			}
			return 0;
}

