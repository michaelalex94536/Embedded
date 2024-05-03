// Toggle Nucleo-F411RE board user LED using memory register addresses directly


// Q: Where is the user LED?
// A: It's on Port A, pin 5 [PA5] on the AHB1 bus

/*
 *
 * 1) We need to enable the AHB1 bus GPIOA peripheral clock (GPIOAEN) [bit 0]
 * 2) Configure the settings for Port A, pin 5
 *
 * */

// Construct relevant memory addresses based on memory map in data sheet and reference manuals
// Final "R" in the symbolic link name denotes a specific register

#define PERIPH_BASE				0x40000000UL 								// Base address for all peripherals (Page 54 of data sheet)
#define AHB1_PERIPH_OFFSET		0x00020000UL								// Offset (from base) for AHB1 peripherals
#define AHB1_PERIPH_BASE		(PERIPH_BASE + AHB1_PERIPH_OFFSET)
#define GPIOA_OFFSET			0x0UL
#define GPIOA_BASE				(AHB1_PERIPH_BASE + GPIOA_OFFSET)			// Can be found directly in reference manual, page 38

#define RCC_OFFSET				0x3800UL
#define RCC_BASE				(AHB1_PERIPH_BASE + RCC_OFFSET)				// Can be found directly in reference manual, page 38

// Symbolic links for the AHB1EN register
#define AHB1EN_R_OFFSET			0x30UL										// Offset for AHB1 enable register (Page 136 of reference manual)
#define RCC_AHB1EN_R			(*(volatile unsigned int *)(RCC_BASE + AHB1EN_R_OFFSET))		// AHB1 bus peripheral clock enable register

// Symbolic links for GPIOA port mode register
#define MODE_R_OFFSET			0x0UL
#define GPIOA_MODE_R			(*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))		// Needed to set the pin as an output

// Links for output data register, ODR
#define OD_R_OFFSET				(0x14)
#define GPIOA_OD_R				(*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET))			// Set to 0 or 1 to set pin value to 0 or 1

#define GPIOAEN					(1U << 0)		// Enable GPIOA							// Set bit 0 of the RCC_AHB1EN_R register to enable GPIOA
#define PIN5					(1U << 5)		// Enable Pin 5 of GPIOA
#define LED_PIN					PIN5



int main(void)
{
	// Enable clock access to GPIOA
	RCC_AHB1EN_R |= GPIOAEN;

	// Set PA5 as an output pin, which requires setting bits 10 and 11
	GPIOA_MODE_R &= ~(1U << 11); // Set bit 11 to 0
	GPIOA_MODE_R |= (1U << 10);  // Set bit 10 to 1


	while(1)
	{
	// Set ODR to one to turn on the LED
	//	GPIOA_OD_R |= PIN5;

	// Toggle LED by alternating the ODR value
		GPIOA_OD_R ^= PIN5;
		for(int i = 0; i < 1600000; i++){}

	}

}
