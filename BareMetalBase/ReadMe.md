### All projects in "BareMetalBase" were run using the STM NUCLEO-F411RE development board.  

#### 0_toggle_led
In this project, the user LED on pin PA5 is toggled on and off by writing directly to the appropriate registers:
1) Enable the GPIOA clock on the AHB1 bus
2) Set pin 5 on GPIOA to be an output
3) Alternate the value written to the PA5 output data register (ODR) with a time delay between each write in order to see the LED toggle on and off

   
The register addresses are found using the memory map in the F411 data sheet and reference manuals.  Doing it this way is really tedious and the most brute force approach.  
