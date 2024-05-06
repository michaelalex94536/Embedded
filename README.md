The "Embedded" repo contains assorted STM32 projects

The "BareMetalBase" sub-repo contains projects created while taking the Udemy class "Embedded Systems Bare-Metal Programming From the Ground Up" and the "BareMetalAdv" sub-repo has projects created while taking the Udemy class "Embedded Systems Advanced Bare-Metal Programming".  "Bare-metal" means the courses don't use the massive ST Micro HAL (hardware abstraction layer) or LL (lower level) APIs.  Using the device reference manual, data sheet, and the Nucleo board user manual, you program the device at the register level, bit-by-bit.  It's tedious at times, but this is the best way to learn embedded programming; nothing is hidden or abstracted away in an API or library.  I first learned STM32 microcontroller programming using the HAL APIs provided by ST Micro, and just like the Arduino, so much stuff is abstracted away and buried from the user when using the HAL APIs, and it may prevent you from understanding the details of how the microprocessor works.  On the other hand, using HAL and the other tools available in the STMCube IDE can really make your project move along faster, especially if you are a beginner or don't need to get into the finde details...   

