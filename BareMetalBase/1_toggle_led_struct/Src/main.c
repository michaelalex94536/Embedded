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

#include <stdint.h>


#define PERIPH_BASE				0x40000000UL 								// Base address for all peripherals (Page 54 of data sheet)
#define AHB1_PERIPH_OFFSET		0x00020000UL								// Offset (from base) for AHB1 peripherals
#define AHB1_PERIPH_BASE		(PERIPH_BASE + AHB1_PERIPH_OFFSET)
#define GPIOA_OFFSET			0x0UL
#define GPIOA_BASE				(AHB1_PERIPH_BASE + GPIOA_OFFSET)			// Can be found directly in reference manual, page 38

#define RCC_OFFSET				0x3800UL
#define RCC_BASE				(AHB1_PERIPH_BASE + RCC_OFFSET)				// Can be found directly in reference manual, page 38

// Symbolic links for the AHB1EN register
//#define AHB1EN_R_OFFSET			0x30UL										// Offset for AHB1 enable register (Page 136 of reference manual)
//#define RCC_AHB1EN_R			(*(volatile unsigned int *)(RCC_BASE + AHB1EN_R_OFFSET))		// AHB1 bus peripheral clock enable register

#define GPIOAEN					(1U << 0)		// Enable GPIOA							// Set bit 0 of the RCC_AHB1EN_R register to enable GPIOA
#define PIN5					(1U << 5)		// Enable Pin 5 of GPIOA
#define LED_PIN					PIN5



// Create a structure for the GPIO register, which has ten (10) members [but we only use 3]
// Check the "stm32f411xe.h" file for the full structure
typedef struct
{
	volatile uint32_t MODER;		/*  GPIO port mode register, Address offser: 0x00    */
	volatile uint32_t DUMMY[4];		/*  Four dummy registers to assure ODER memory location is correct    */
	volatile uint32_t ODER;			/*  GPIO port output data register, Address offset: 0x14    */

}GPIO_TypeDef;


// Create a structure for the RCC register, which has 24 members [but we only use 1]
// Check the "stm32f411xe.h" file for the full structure
typedef struct
{
	volatile uint32_t DUMMY[12];	/*  Twelve (12) dummy registers to assure AHB1ENR memory location is correct    */
	volatile uint32_t AHB1ENR;		/*  AHB1ENR register, Address offset: 0x30   */

}RCC_TypeDef;

#define RCC			((RCC_TypeDef*) RCC_BASE)
#define GPIOA		((GPIO_TypeDef*) GPIOA_BASE)

int main(void)
{
	// Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;


	// Set PA5 as an output pin, which requires setting bits 10 and 11
	GPIOA->MODER &= ~(1U << 11); // Set bit 11 to 0
	GPIOA->MODER |= (1U << 10); // Set bit 10 to 1


	while(1)
	{
	// Toggle LED by alternating the ODR value
		GPIOA->ODER ^= PIN5;
		for(int i = 0; i < 1600000; i++){}

	}

}
