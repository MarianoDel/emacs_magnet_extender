//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.C #################################
//---------------------------------------------
#include "hard.h"
#include "stm32f10x.h"

#include "adc.h"
#include "tim.h"
#include "usart.h"

#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];


// Globals ---------------------------------------------------------------------
unsigned char led_status = LED_VOLTAGE_GOOD;
volatile unsigned short led_timer = 0;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Led_Slave_On (void)
{
    LED_SLAVE_ON;
}


void Led_Master_On (void)
{
    LED_MASTER_ON;
}


void Led_Error_On (void)
{
    LED_ERROR_ON;
}


void Led_Slave_Off (void)
{
    LED_SLAVE_OFF;
}


void Led_Master_Off (void)
{
    LED_MASTER_OFF;
}


void Led_Error_Off (void)
{
    LED_ERROR_OFF;
}


unsigned char Led_Slave_Is_On (void)
{
    return LED_SLAVE;
}


unsigned char Led_Master_Is_On (void)
{
    return LED_MASTER;
}


unsigned char Led_Error_Is_On (void)
{
    return LED_ERROR;
}


unsigned char Master_Pin (void)
{
    return MASTER_SLAVE;
}


void Enable_DE (void)
{
    SW_RX_TX_ON;
}


void Disable_DE (void)
{
    SW_RX_TX_OFF;
}


void Change_Led_Voltage (unsigned char new_status)
{
    led_status = new_status;
}


void Update_Led_Voltage (void)
{
    switch (led_status)
    {
    case LED_OVERVOLTAGE:
        if (!led_timer)
        {
            led_timer = 200;
            if (Led_Error_Is_On())
                Led_Error_Off();
            else
                Led_Error_On();
        }
        break;

    case LED_UNDERVOLTAGE:
        Led_Error_On();
        break;

    case LED_VOLTAGE_GOOD:
        Led_Error_Off();
        break;
    }
}


void Hard_Timeouts (void)
{
    if (led_timer)
        led_timer--;
    
}

//--- end of file ---//
