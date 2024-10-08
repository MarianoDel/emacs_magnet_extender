//----------------------------------------------------------
// #### MAGNET PROJECT - Custom Board ####
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.C ###################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_functions.h"
#include "hard.h"
#include "stm32f10x.h"
#include "gpio.h"
#include "adc.h"
#include "usart_channels.h"
#include "usart.h"
#include "dma.h"
#include "tim.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];
extern volatile unsigned short wait_ms_var;
extern volatile unsigned short timer_standby;
extern volatile unsigned char timer_6_uif_flag;
extern volatile unsigned char rx_int_handler;
// extern volatile unsigned char usart3_have_data;


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Led_Test (void);

void TF_Act_Channels (void);

void TF_UsartChannel1_Loop (void);
void TF_UsartChannel2_Loop (void);
void TF_UsartChannel3_Loop (void);
void TF_UsartChannel4_Loop (void);

// void TF_UsartRs485_Loop (void);
// void TF_UsartRs485_String (void);

void TF_PowerOn_Channel1_Channel2 (void);

// void TF_Adc_Usart1_Tx (void);
// void TF_Adc_Usart1_Voltages (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    TF_Led_Test ();

    // TF_UsartChannel1_Loop ();
    // TF_UsartChannel2_Loop ();
    // TF_UsartChannel3_Loop ();
    // TF_UsartChannel4_Loop ();    

    // TF_UsartRs485_Loop ();
    // TF_UsartRs485_String ();

    // TF_PowerOn_Channel1_Channel2 ();

    // TF_Adc_Usart1_Tx ();
    // TF_Adc_Usart1_Voltages ();

    // TF_Tim6_Int ();

    // TF_Int_Pb6();

}


void TF_Led_Test (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (Led1_Is_On())
        {
            Led1_Off();
            Led2_Off();
            Led3_Off();
        }
        else
        {
            Led1_On();
            Led2_On();
            Led3_On();
        }
    }
}


void TF_UsartChannel1_Loop (void)
{
    char buff [100];
    
    UsartChannel1Config();
    
    while (1)
    {
        if (!timer_standby)
        {
            UsartChannel1Send("Mariano\n");
            timer_standby = 2000;
            if (Led1_Is_On())
                Led1_Off();
        }

        if (UsartChannel1HaveData())
        {
            UsartChannel1HaveDataReset();
            UsartChannel1ReadBuffer(buff, 100);
            if (strncmp(buff, "Mariano", sizeof("Mariano") - 1) == 0)
                Led1_On();
        }
    }
}


void TF_UsartChannel2_Loop (void)
{
    char buff [100];
    
    UsartChannel2Config();
    
    while (1)
    {
        if (!timer_standby)
        {
            UsartChannel2Send("Mariano\n");
            timer_standby = 2000;
            if (Led1_Is_On())
                Led1_Off();
        }

        if (UsartChannel2HaveData())
        {
            UsartChannel2HaveDataReset();
            UsartChannel2ReadBuffer(buff, 100);
            if (strncmp(buff, "Mariano", sizeof("Mariano") - 1) == 0)
                Led1_On();
        }
    }
}


// place a shortcut on IC4 2 & IC3 4
void TF_UsartChannel3_Loop (void)
{
    char buff [100];
    
    UsartChannel3Config();
    
    while (1)
    {
        if (!timer_standby)
        {
            UsartChannel3Send("Mariano\n");
            timer_standby = 2000;
            if (Led1_Is_On())
                Led1_Off();
        }

        if (UsartChannel3HaveData())
        {
            UsartChannel3HaveDataReset();
            UsartChannel3ReadBuffer(buff, 100);
            if (strncmp(buff, "Mariano", sizeof("Mariano") - 1) == 0)
                Led1_On();
        }
    }
}


// place a shortcut on IC4 2 & IC3 4
void TF_UsartChannel4_Loop (void)
{
    char buff [100];
    
    UsartChannel4Config();
    
    while (1)
    {
        if (!timer_standby)
        {
            UsartChannel4Send("Mariano\n");
            timer_standby = 2000;
            if (Led1_Is_On())
                Led1_Off();
        }

        if (UsartChannel4HaveData())
        {
            UsartChannel4HaveDataReset();
            UsartChannel4ReadBuffer(buff, 100);
            if (strncmp(buff, "Mariano", sizeof("Mariano") - 1) == 0)
                Led1_On();
        }
    }
}


// place a shortcut Rx Tx on Rs485 connector
void TF_UsartRs485_Loop (void)
{
    char buff [100];
    
    UsartRs485Config ();
    
    while (1)
    {
        if (!timer_standby)
        {
            UsartRs485Send ("Mariano\n");
            timer_standby = 2000;
            if (Led1_Is_On())
                Led1_Off();
        }

        if (UsartRs485HaveData ())
        {
            UsartRs485HaveDataReset ();
            UsartRs485ReadBuffer (buff, 100);
            if (strncmp(buff, "Mariano", sizeof("Mariano") - 1) == 0)
                Led1_On();
        }
    }
}


// Terminal Looping on Rs485 connector
void TF_UsartRs485_String (void)
{
    char buff [100];

    UsartRs485Config ();
    UsartRs485Send("rs485 usart test... send a string:\n");
    
    while (1)
    {
        if (UsartRs485HaveData())
        {
            UsartRs485HaveDataReset();
            UsartRs485ReadBuffer(buff, 100);

            Wait_ms(1000);

            int i = strlen(buff);
            if (i < 99)
            {
                buff[i] = '\n';
                buff[i+1] = '\0';
                UsartRs485Send(buff);
            }
        }
    }
}


void TF_PowerOn_Channel1_Channel2 (void)
{
    ENA_BOOST_ON;
    ENA_CH1_ON;
    ENA_CH2_ON;
    
    while (1);
}




void TF_Tim6_Int (void)
{
    // char buff [100];
    TIM6_Init ();
    TIM6_Start();
    // Bit_Bang_Init ();
    
    while (1)
    {
        if (timer_6_uif_flag)
        {
            timer_6_uif_flag = 0;
            if (PB7)
                PB7_OFF;
            else
                PB7_ON;
        }
    }
}


void TF_Int_Pb6 (void)
{
    Wait_ms(1000);
    
    EXTIOn();
    
    while (1)
    {
        if (rx_int_handler)
        {
            rx_int_handler = 0;
            if (PB7)
                PB7_OFF;
            else
                PB7_ON;
        }
    }
}



//--- end of file ---//
