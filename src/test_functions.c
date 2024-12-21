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
void TF_Master_Input (void);

void TF_Usart_Channel1_Tx (void);
void TF_Usart_Channel2_Tx (void);
void TF_Usart_Channel3_Tx (void);
void TF_Usart_Channel4_Tx (void);

void TF_Usart_Channel1_Rx (void);
void TF_Usart_Channel2_Rx (void);
void TF_Usart_Channel3_Rx (void);
void TF_Usart_Channel4_Rx (void);

void TF_UsartRs485_Tx (void);
void TF_UsartRs485_Rx (void);

void TF_Adc_Led_Error (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // TF_Led_Test ();

    // TF_Master_Input ();

    // TF_Usart_Channel1_Tx ();
    // TF_Usart_Channel2_Tx ();
    // TF_Usart_Channel3_Tx ();
    // TF_Usart_Channel4_Tx ();    

    // TF_Usart_Channel1_Rx ();
    // TF_Usart_Channel2_Rx ();
    // TF_Usart_Channel3_Rx ();
    // TF_Usart_Channel4_Rx ();    

    // TF_UsartRs485_Tx ();
    TF_UsartRs485_Rx ();    

    // TF_Adc_Led_Error ();
}


void TF_Led_Test (void)
{
    while (1)
    {
	Led_Slave_Off();
	Led_Master_Off();
	Led_Error_Off();
        Wait_ms(2000);

	Led_Slave_On();
	Wait_ms(1000);
	Led_Master_On();
	Wait_ms(1000);
	Led_Error_On();
	Wait_ms(1000);
    }
}


void TF_Master_Input (void)
{
    while (1)
    {
	if (Master_Pin())
	{
	    Led_Master_On();
	    Led_Slave_Off();
	}
	else
	{
	    Led_Master_Off();
	    Led_Slave_On();
	}

	Wait_ms(300);
    }
}


void TF_Adc_Led_Error (void)
{
    //-- ADC and DMA configuration
    AdcConfig ();
    DMAConfig();
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
    AdcStart ();

    while (1)
    {
	Led_Slave_On();
	// if (Voltage_Sense < VSENSE_4_5_V)
	if (Voltage_Sense < 1181)
	// if (Voltage_Sense < 2000)
	{
	    Led_Error_On();
	}
	else
	    Led_Error_Off();

	Wait_ms(100);
	Led_Slave_Off();

	Wait_ms(900);
    }
}


void TF_Usart_Channel1_Tx (void)
{
    UsartChannel1Config();

    while (1)
    {
	Led_Slave_On();
	UsartChannel1Send("Channel 1\r\n");
	Wait_ms(100);
	Led_Slave_Off();
	Wait_ms(1900);
    }
}


void TF_Usart_Channel2_Tx (void)
{
    UsartChannel2Config();

    while (1)
    {
	Led_Slave_On();
	UsartChannel2Send("Channel 2\r\n");
	Wait_ms(100);
	Led_Slave_Off();
	Wait_ms(1900);
    }
}


void TF_Usart_Channel3_Tx (void)
{
    UsartChannel3Config();

    while (1)
    {
	Led_Slave_On();
	UsartChannel3Send("Channel 3\r\n");
	Wait_ms(100);
	Led_Slave_Off();
	Wait_ms(1900);
    }
}


void TF_Usart_Channel4_Tx (void)
{
    UsartChannel4Config();

    while (1)
    {
	Led_Slave_On();
	UsartChannel4Send("Channel 4\r\n");
	Wait_ms(100);
	Led_Slave_Off();
	Wait_ms(1900);
    }
}


void TF_Usart_Channel1_Rx (void)
{
    char buff [100];
    
    UsartChannel1Config();

    while (1)
    {
	if (!timer_standby)
	{
	    Led_Slave_Off();
	    
	    if (UsartChannel1HaveData())
	    {
		UsartChannel1HaveDataReset();
		UsartChannel1ReadBuffer(buff, 100);
		if (strncmp(buff, "med", sizeof("med") - 1) == 0)
		    Led_Slave_On();
	 
	    }

	    timer_standby = 2000;
	}
    }
}


void TF_Usart_Channel2_Rx (void)
{
    char buff [100];
    
    UsartChannel2Config();

    while (1)
    {
	if (!timer_standby)
	{
	    Led_Slave_Off();
	    
	    if (UsartChannel2HaveData())
	    {
		UsartChannel2HaveDataReset();
		UsartChannel2ReadBuffer(buff, 100);
		if (strncmp(buff, "med", sizeof("med") - 1) == 0)
		    Led_Slave_On();
	 
	    }

	    timer_standby = 2000;
	}
    }
}


void TF_Usart_Channel3_Rx (void)
{
    char buff [100];
    
    UsartChannel3Config();

    while (1)
    {
	if (!timer_standby)
	{
	    Led_Slave_Off();
	    
	    if (UsartChannel3HaveData())
	    {
		UsartChannel3HaveDataReset();
		UsartChannel3ReadBuffer(buff, 100);
		if (strncmp(buff, "med", sizeof("med") - 1) == 0)
		    Led_Slave_On();
	 
	    }

	    timer_standby = 2000;
	}
    }
}


void TF_Usart_Channel4_Rx (void)
{
    char buff [100];
    
    UsartChannel4Config();

    while (1)
    {
	if (!timer_standby)
	{
	    Led_Slave_Off();
	    
	    if (UsartChannel4HaveData())
	    {
		UsartChannel4HaveDataReset();
		UsartChannel4ReadBuffer(buff, 100);
		if (strncmp(buff, "med", sizeof("med") - 1) == 0)
		    Led_Slave_On();
	 
	    }

	    timer_standby = 2000;
	}
    }
}


// Terminal Looping on Rs485 connector
void TF_UsartRs485_Tx (void)
{
    UsartRs485Config ();
    Led_Master_On();
    
    while (1)
    {
	if (!timer_standby)
	{
	    timer_standby = 2000;

	    Led_Master_Off();
	    Enable_DE();
	    Wait_ms (1);
	    UsartRs485Send("Ponete a Laburar!\r\n");
	    while (!UsartRs485SendFinish());
	    Wait_ms (1);	    
	    Disable_DE();
	    Wait_ms(100);
	    Led_Master_On();
	}	    
    }
}


void TF_UsartRs485_Rx (void)
{
    char buff [100];
    
    UsartChannel1Config();
    UsartRs485Config ();
    Disable_DE();
    Led_Slave_On();
    
    while (1)
    {
	if (UsartRs485HaveData())
	{
	    Led_Slave_Off();
	    UsartRs485HaveDataReset();
	    int len1 = UsartRs485ReadBuffer(buff, 100 - 3);

	    // for tests
	    char btest[100];
	    int len2 = strlen(buff);
	    sprintf(btest, "get len: %d strlen: %d\r\n", len1, len2);
	    UsartChannel1Send(btest);
	    UsartChannel1Send(buff);	
	    UsartChannel1Send("\r\n");
	    // end for tests
	    
	    Wait_ms(100);
	    Led_Slave_On();
	}
    }
}


//--- end of file ---//
