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
void Led1_On (void)
{
    ENA_CH1_ON;
}


void Led2_On (void)
{
    ENA_CH2_ON;
}


void Led3_On (void)
{
    ENA_CH3_ON;
}


void Led1_Off (void)
{
    ENA_CH1_OFF;
}


void Led2_Off (void)
{
    ENA_CH2_OFF;
}


void Led3_Off (void)
{
    ENA_CH3_OFF;
}


unsigned char Led1_Is_On (void)
{
    return ENA_CH1;
}


unsigned char Led2_Is_On (void)
{
    return ENA_CH2;
}


unsigned char Led3_Is_On (void)
{
    return ENA_CH3;
}


unsigned char Master_Pin (void)
{
    return ENA_CH3;
}


void Enable_DE (void)
{
    ENA_CH3_ON;
}


void Disable_DE (void)
{
    ENA_CH3_OFF;
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
            if (Led3_Is_On())
                Led3_Off();
            else
                Led3_On();
        }
        break;

    case LED_UNDERVOLTAGE:
        Led3_On();
        break;

    case LED_VOLTAGE_GOOD:
        Led3_Off();
        break;
    }
}


void Hard_GetHardSoft (char * buff)
{
    sprintf(buff, "%s %s\n", HARD, SOFT);
}


void Hard_Timeouts (void)
{
    if (led_timer)
        led_timer--;
    
}
// void Hard_GetVoltages (char * buff)
// {
//     ADC_START;
//     Wait_ms(1);

//     sprintf(buff, "%d %d %d %d %d %d\n",
//             SENSE_POWER,
//             SENSE_MEAS,
//             V_SENSE_28V,
//             V_SENSE_25V,
//             V_SENSE_11V,
//             V_SENSE_8V);

// }


// void Hard_GetVoltages_Complete (void)
// {
//     char buff [50];
//     int calc_int, calc_dec;
    
//     ADC_START;
//     Wait_ms(1);

//     sprintf(buff, "%d %d %d %d %d %d\n",
//             SENSE_POWER,
//             SENSE_MEAS,
//             V_SENSE_28V,
//             V_SENSE_25V,
//             V_SENSE_11V,
//             V_SENSE_8V);

//     Usart1Send (buff);

//     // SENSE_POWER resistor multiplier 11
//     calc_int = SENSE_POWER * 330 * 11;
//     calc_int >>= 12;
//     calc_dec = calc_int;
//     calc_int = calc_int / 100;
//     calc_dec = calc_dec - calc_int * 100;
//     sprintf(buff, "Power: %d.%02dV, ", calc_int, calc_dec);
//     Usart1Send (buff);

//     // SENSE_MEAS resistor multiplier 2
//     calc_int = SENSE_MEAS * 330 * 2;
//     calc_int >>= 12;
//     calc_dec = calc_int;
//     calc_int = calc_int / 100;
//     calc_dec = calc_dec - calc_int * 100;
//     sprintf(buff, "Meas: %d.%02dV, ", calc_int, calc_dec);
//     Usart1Send (buff);

//     // V_SENSE_28V resistor multiplier 11
//     calc_int = V_SENSE_28V * 330 * 11;
//     calc_int >>= 12;
//     calc_dec = calc_int;
//     calc_int = calc_int / 100;
//     calc_dec = calc_dec - calc_int * 100;
//     sprintf(buff, "V28: %d.%02dV, ", calc_int, calc_dec);
//     Usart1Send (buff);

//     // V_SENSE_25V resistor multiplier 11
//     calc_int = V_SENSE_25V * 330 * 11;
//     calc_int >>= 12;
//     calc_dec = calc_int;
//     calc_int = calc_int / 100;
//     calc_dec = calc_dec - calc_int * 100;
//     sprintf(buff, "V25: %d.%02dV, ", calc_int, calc_dec);
//     Usart1Send (buff);

//     // V_SENSE_11V resistor multiplier 11
//     calc_int = V_SENSE_11V * 330 * 11;
//     calc_int >>= 12;
//     calc_dec = calc_int;
//     calc_int = calc_int / 100;
//     calc_dec = calc_dec - calc_int * 100;
//     sprintf(buff, "V11: %d.%02dV, ", calc_int, calc_dec);
//     Usart1Send (buff);

//     // V_SENSE_8V resistor multiplier 11
//     calc_int = V_SENSE_8V * 330 * 11;
//     calc_int >>= 12;
//     calc_dec = calc_int;
//     calc_int = calc_int / 100;
//     calc_dec = calc_dec - calc_int * 100;
//     sprintf(buff, "V8: %d.%02dV\n", calc_int, calc_dec);
//     Usart1Send (buff);
    
// }

void Tx_Pin_On (void)
{
    PB7_ON;
}


void Tx_Pin_Off (void)
{
    PB7_OFF;
}

unsigned char Rx_Pin (void)
{
    return PB6;
}
//--- end of file ---//
