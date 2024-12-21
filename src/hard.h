//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.H #################################
//---------------------------------------------

#ifndef HARD_H_
#define HARD_H_


//----------- Defines For Configuration -------------

//----- Board Configuration -------------------//
//--- Hardware ------------------//
#define HARDWARE_VERSION_1_0    // first prototype


//--- Software ------------------//
#define FIRMWARE_VERSION_1_0    // init version


//-------- Type of Program (depending on software version) ----------------



//-------- Type of Program and Features ----------------
// #define USE_NO_TREATMENT_DETECT    //cuando esta en tratamiento revisa si las potencias tambien
// #define USE_BUZZER_ON_START


//-------- Kind of Reports Sended ----------------

//-------- Others Configurations depending on the formers ------------
#define OVERVOLTAGE    2895    // 6.5V * 0.211 = 1.371V / 3.3V * 4095 = 1702
#define UNDERVOLTAGE    1181    // 4.5 * 0.211 = 0.952V / 3.3V * 4095 = 1181

enum {
    OVERVOLTAGE_STATUS,
    UNDERVOLTAGE_STATUS,
    VOLTAGE_GOOD_STATUS

};

#define LED_OVERVOLTAGE    0
#define LED_UNDERVOLTAGE    1
#define LED_VOLTAGE_GOOD    2
#define OVERVOLTAGE_CMD    LED_OVERVOLTAGE
#define UNDERVOLTAGE_CMD    LED_UNDERVOLTAGE
#define VOLTAGE_GOOD_CMD    LED_VOLTAGE_GOOD


//-------- Oscillator and Crystal selection (Freq in startup_clocks.h) ---
#define HSI_INTERNAL_RC
// #define HSE_CRYSTAL_OSC

#ifdef HSE_CRYSTAL_OSC
// #define CRYSTAL_8MHZ
#define CRYSTAL_12MHZ
#endif

#ifdef HSE_CRYSTAL_OSC
// #define SYSCLK_FREQ_72MHz
#define SYSCLK_FREQ_8MHz
#endif

#ifdef HSI_INTERNAL_RC
#define SYSCLK_FREQ_64MHz
// #define SYSCLK_FREQ_8MHz
#endif

//-------- End Of Defines For Configuration ------




//--- Hardware & Software Messages ------------------//
#ifdef HARDWARE_VERSION_1_0
#define HARD "Hardware Version: 1.0"
#endif
#ifdef FIRMWARE_VERSION_1_0
#define SOFT "Firmware Version: 1.0"
#endif
//--- End of Hardware & Software Messages ------------------//



// Exported Types --------------------------------------------------------------
#ifdef HARDWARE_VERSION_1_0

// PA defines ----
// PA0
// PA1 NC

// PA2 PA3 Alternative Usart2 Tx Rx (Channel 4)

// PA4
// PA5
// PA6
// PA7
// PA8 NC

// PA9 PA10 Alternative Usart1 Tx Rx (Channel 1)

// PA11
// PA12
// PA13 PA14 PA15 NC jtag

// PB defines ----
// PB0 
// PB1 NC

//PB2
#define SW_RX_TX    ((GPIOB->ODR & 0x0004) != 0)
#define SW_RX_TX_ON    (GPIOB->BSRR = 0x00000004)
#define SW_RX_TX_OFF    (GPIOB->BSRR = 0x00040000)

// PB3 PB4 NC jtag
// PB5 
// PB6
// PB7
// PB8
// PB9 NC

// PB10 PB11 Alternative Usart3 Tx Rx (Rs485)

// PB12
#define LED_SLAVE    ((GPIOB->ODR & 0x1000) != 0)
#define LED_SLAVE_ON    (GPIOB->BSRR = 0x00001000)
#define LED_SLAVE_OFF    (GPIOB->BSRR = 0x10000000)

// PB13
#define LED_MASTER    ((GPIOB->ODR & 0x2000) != 0)
#define LED_MASTER_ON    (GPIOB->BSRR = 0x00002000)
#define LED_MASTER_OFF    (GPIOB->BSRR = 0x20000000)

// PB14
#define LED_ERROR    ((GPIOB->ODR & 0x4000) != 0)
#define LED_ERROR_ON    (GPIOB->BSRR = 0x00004000)
#define LED_ERROR_OFF    (GPIOB->BSRR = 0x40000000)

// PB15
#define MASTER_SLAVE    ((GPIOB->IDR & 0x8000) == 0)


// PC defines ----
// PC0
// PC1
// PC2
// PC3 NC

// PC4 Analog input VOLT_DETECT 

// PC5
// PC6
// PC7 
// PC8
// PC9 NC

// PC10 PC11 Alternative Uart4 Tx Rx (Channel 2)

// PC12 Alternative Uart5 Tx (Channel 3)

// PC13 PC14 PC15 NC

// PD defines ----
// PD0 PD1 NC

// PD2 Alternative Uart5 Rx (Channel 3)

#endif //HARDWARE_VERSION_1_0



// Module Exported Functions ---------------------------------------------------
void Enable_DE (void);
void Disable_DE (void);

void Led_Slave_On (void);
void Led_Master_On (void);
void Led_Error_On (void);

void Led_Slave_Off (void);
void Led_Master_Off (void);
void Led_Error_Off (void);

unsigned char Led_Slave_Is_On (void);
unsigned char Led_Master_Is_On (void);
unsigned char Led_Error_Is_On (void);

unsigned char Master_Pin (void);

void Change_Led_Voltage (unsigned char new_status);
void Update_Led_Voltage (void);

#endif
