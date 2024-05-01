// Q: Where is the user LED?
// A: It's on Port A, pin 5 [PA5] on the AHB1 bus

/* So we need to enable the AHB1 bus clock and the settings for Port A, pin 5 */

// Construct memory addresses for LED based on memory map in data sheet

#define PERIPH_BASE				0x40000000UL
#define AHB1_PERIPH_OFFSET		0x00020000UL
#define AHB1_PERIPH_BASE		(PERIPH_BASE + AHB1_PERIPH_OFFSET)
#define GPIOA_OFFSET			0x0UL
#define GPIOA_BASE				(AHB1_PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET				0x3800UL
#define RCC_BASE				(AHB1_PERIPH_BASE + RCC_OFFSET)

#define AHB1EN_R_OFFSET			0x30UL
#define RCC_AHB1EN_R			(RCC_BASE + AHB1EN_R_OFFSET)

int main(void)
{
    /* Loop forever */
	for(;;);
}
