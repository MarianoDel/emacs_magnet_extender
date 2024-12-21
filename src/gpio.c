//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### GPIO.C ################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "stm32f10x.h"
#include "gpio.h"
#include "hard.h"


// Module Private Types Constants and Macros -----------------------------------
// #define USE_EXTI_LINES


#define RCC_GPIOA_CLK    (RCC->APB2ENR & 0x00000004)
#define RCC_GPIOA_CLKEN    (RCC->APB2ENR |= 0x00000004)
#define RCC_GPIOA_CLKDIS    (RCC->APB2ENR &= ~0x00000004)

#define RCC_GPIOB_CLK    (RCC->APB2ENR & 0x00000008)
#define RCC_GPIOB_CLKEN    (RCC->APB2ENR |= 0x00000008)
#define RCC_GPIOB_CLKDIS    (RCC->APB2ENR &= ~0x00000008)

#define RCC_GPIOC_CLK    (RCC->APB2ENR & 0x00000010)
#define RCC_GPIOC_CLKEN    (RCC->APB2ENR |= 0x00000010)
#define RCC_GPIOC_CLKDIS    (RCC->APB2ENR &= ~0x00000010)

#define RCC_GPIOD_CLK    (RCC->APB2ENR & 0x00000020)
#define RCC_GPIOD_CLKEN    (RCC->APB2ENR |= 0x00000020)
#define RCC_GPIOD_CLKDIS    (RCC->APB2ENR &= ~0x00000020)

#define RCC_AFIO_CLK    (RCC->APB2ENR & 0x00000001)
#define RCC_AFIO_CLKEN    (RCC->APB2ENR |= 0x00000001)
#define RCC_AFIO_CLKDIS    (RCC->APB2ENR &= ~0x00000001)


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Functions ------------------------------------------------------------
//------- GPIO REGISTERS ----------//
//
//	GPIOx->CRL	pin 7 - 0
//	3  2  1  0
//	CNF0  MODE0
//	GPIOx->CRH	pin 15 - 8
//
//      En hexa me queda cada nibble es igual a la configuracion de ese pin
//      nibble0 configura completo pin0
//
//	MODEx 00 Input (reset)
//	MODEx 01 Output 10MHz
//	MODEx 10 Output 2MHz
//	MODEx 11 Output 50MHz
//
//	Input Mode
//      las funciones alternativas de los pines estan directamente conectadad al
//      periferico en el modo input
//      CNFx 00 Analog
//      CNFx 01 Input Floating (reset)
//      CNFx 10 Input (pull up / dwn)
//      CNFx 11 Reserved
//
//	Output Mode
//      CNFx 00 Push Pull
//      CNFx 01 Open Drain
//      CNFx 10 Alternate func Push Pull
//      CNFx 11 Alternate func Open Drain
//
//      Pull-Up Pull-Dwn si el pin es Input Pup Pdwn el registro ODR coloca pull-up (1) o pull-dwn (0)
//      GPIOx->ODR 0xNNNN, 1 bit por pin
//

//-- GPIO Configuration --------------------
void GpioInit (void)
{
    unsigned long temp;

    //--- GPIO Clocks ---//
    if (!RCC_GPIOA_CLK)
        RCC_GPIOA_CLKEN;

    if (!RCC_GPIOB_CLK)
        RCC_GPIOB_CLKEN;

    if (!RCC_GPIOC_CLK)
        RCC_GPIOC_CLKEN;

    if (!RCC_GPIOD_CLK)
        RCC_GPIOD_CLKEN;

    //--- GPIOA Low Side ------------------//
    // PA0 NC
    // PA1 NC
    // PA2 Alternative Tx Usart2 open drain
    // PA3 Alternative Rx Usart2
    // PA4 NC
    // PA5 NC
    // PA6 NC
    // PA7 NC
    temp = GPIOA->CRL;
    temp &= 0xFFFF00FF;
    temp |= 0x00004F00;
    GPIOA->CRL = temp;

    //--- GPIOA High Side ------------------//
    //PA8 NC
    //PA9 alternative Tx Usart1 open drain
    //PA10 alternative Rx Usart1
    //PA11 NC
    //PA12 NC
    //PA13 NC
    //PA14 NC
    //PA15 NC
    temp = GPIOA->CRH;
    temp &= 0xFFFFF00F;
    temp |= 0x000004F0;
    GPIOA->CRH = temp;

    //--- GPIOA Pull-Up Pull-Dwn ------------------//
    temp = GPIOA->ODR;    //
    temp &= 0xFFFF;       //
    temp |= 0x0000;
    GPIOA->ODR = temp;

    //--- GPIOB Low Side -------------------//
    //PB0 NC
    //PB1 NC
    //PB2 SW_RX_TX
    //PB3 NC jtag
    //PB4 NC jtag
    //PB5 NC
    //PB6 NC
    //PB7 NC
    temp = GPIOB->CRL;
    temp &= 0xFFFFF0FF;
    temp |= 0x00000200;
    GPIOB->CRL = temp;

    //--- GPIOB High Side -------------------//
    //PB8 NC
    //PB9 NC
    //PB10 Alternative Tx Usart3
    //PB11 Alternative Rx Usart3
    //PB12 LED_SLAVE
    //PB13 LED_MASTER
    //PB14 LED_ERROR
    //PB15 MASTER_SLAVE
    temp = GPIOB->CRH;
    temp &= 0x000000FF;
    temp |= 0x82224A00;
    GPIOB->CRH = temp;    
    
    //--- GPIOB Pull-Up Pull-Dwn ------------------//
    temp = GPIOB->ODR;    //PB15 pull-up
    temp &= 0x7FFF;
    temp |= 0x8000;
    GPIOB->ODR = temp;

    //--- GPIOC Low Side -------------------//
    //PC0 NC
    //PC1 NC
    //PC2 NC
    //PC3 NC
    //PC4 Analog input
    //PC5 NC
    //PC6 NC
    //PC7 NC
    temp = GPIOC->CRL;
    temp &= 0xFFF0FFFF;
    temp |= 0x00000000;
    GPIOC->CRL = temp;

    //--- GPIOC High Side -------------------//
    //PC8 NC
    //PC9 NC
    //PC10 Alternative Uart4 Tx
    //PC11 Alternative Uart4 Rx
    //PC12 Alternative Uart5 Tx
    //PC13 NC
    //PC14 NC    oscillator
    //PC15 NC    oscillator
    temp = GPIOC->CRH;   
    temp &= 0xFFF000FF;
    temp |= 0x000F4F00;
    GPIOC->CRH = temp;

    //--- GPIOC Pull-Up Pull-Dwn ------------------//
    temp = GPIOC->ODR;    //
    temp &= 0xFFFF;
    temp |= 0x0000;
    GPIOC->ODR = temp;

    //--- GPIOD Low Side -------------------//
    //PD0 NC
    //PD1 NC
    //PD2 Alternative Uart5 Rx
    //PD3 No implemented
    //PD4 No implemented
    //PD5 No implemented
    //PD6 No implemented
    //PD7 No implemented    
    temp = GPIOD->CRL;   
    temp &= 0xFFFFF0FF;
    temp |= 0x00000400;
    GPIOD->CRL = temp;

    //--- GPIOD Pull-Up Pull-Dwn ------------------//
    temp = GPIOD->ODR;    //
    temp &= 0xFFFF;
    temp |= 0x0000;
    GPIOD->ODR = temp;
    
#ifdef USE_EXTI_LINES
    //Interrupts on:
    // PB6 Rx bit bang pin
    if (!RCC_AFIO_CLK)
        RCC_AFIO_CLKEN;

    // EXTI6 Select Port B & Pin6 for external interrupt
    temp = AFIO->EXTICR[1];
    temp &= ~AFIO_EXTICR2_EXTI6;
    temp |= AFIO_EXTICR2_EXTI6_PB;
    AFIO->EXTICR[1] = (unsigned short) temp;

    // EXTI->IMR |= 0x00000001;    //Corresponding mask bit for interrupts EXTI2 EXTI4 EXTI13 EXTI15
    // EXTI->EMR |= 0x00000000;    //Corresponding mask bit for events
    EXTI->RTSR |= 0x00000000;
    EXTI->FTSR |= EXTI_FTSR_TR6;    //Interrupt line on falling edge

    // Enable NVIC for EXTIs
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn, 3);
    
#endif    // USE_EXTI_LINES
}


#ifdef USE_EXTI_LINES
void EXTIOff (void)
{
    // EXTI->IMR &= ~(EXTI_IMR_MR2 | EXTI_IMR_MR4 | EXTI_IMR_MR13 | EXTI_IMR_MR15);
    EXTI->IMR &= ~(EXTI_IMR_MR6);
}

void EXTIOn (void)
{
    // EXTI->IMR |= (EXTI_IMR_MR2 | EXTI_IMR_MR4 | EXTI_IMR_MR13 | EXTI_IMR_MR15);
    EXTI->IMR |= (EXTI_IMR_MR6);
}
#endif


//--- end of file ---//
