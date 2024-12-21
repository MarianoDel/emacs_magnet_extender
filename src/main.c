//---------------------------------------------------------------
// #### PROJECT MICRO-CURRENTS SUPPLY-BRD F103 - Custom Board ###
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### MAIN.C ##################################################
//---------------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "stm32f10x.h"
#include "hard.h"

#include "adc.h"
#include "dma.h"
#include "dac.h"
#include "tim.h"
#include "gpio.h"
#include "usart_channels.h"
#include "usart.h"

#include "comms.h"
#include "master_slave.h"
#include "test_functions.h"

#include <stdio.h>
#include <string.h>


// Private Types Constants and Macros ------------------------------------------


// Externals -------------------------------------------------------------------
//--- Externals from timers
volatile unsigned short timer_standby = 0;
volatile unsigned short timer_for_batt_report = 0;
volatile unsigned short wait_ms_var = 0;
volatile unsigned char probe_detect_timer = 0;
volatile unsigned short probe_ch1_detect_filter = 0;

//--- Externals from adc
volatile unsigned short adc_ch [ADC_CHANNEL_QUANTITY];


// Globals ---------------------------------------------------------------------
// volatile unsigned short timer_sync_xxx_ms = 0;
// volatile unsigned short timer_out4 = 0;

// parameters_typedef * pmem = (parameters_typedef *) (unsigned int *) FLASH_PAGE_FOR_BKP;	//en flash
// parameters_typedef mem_conf;


// Module Private Functions ----------------------------------------------------
void TimingDelay_Decrement(void);
void SysTickError (void);
unsigned char Probe_Detect_Ch1 (void);
void Probe_Detect_Update (void);
void Probe_Detect_Ch1_SM (void);
void Starts_Everything (void);
void Shutdown_Everything (void);
void Full_Working_Loop (void);


// Module Functions ------------------------------------------------------------
int main (void)
{
    // Gpio Configuration.
    GpioInit();
    
    // Systick Timer Activation
    if (SysTick_Config(64000))
        SysTickError();

    // Hardware Tests
    // TF_Hardware_Tests ();
    // End of Hardware Tests
    
    // --- main program inits. ---
    Disable_DE();
    
    //-- ADC and DMA configuration
    AdcConfig ();
    DMAConfig();
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
    AdcStart ();

    // -- Enable Comms Module
    Comms_Init ();
    
    // -- Enable all Usarts
    UsartChannel1Config();
    UsartChannel2Config();
    UsartChannel3Config();
    UsartChannel4Config();
    UsartRs485Config();
    
    if (Master_Is_On ())
    {
        Master_Slave_Config(MASTER_CONFIG);

        Led_Slave_On();
        Led_Master_On();
        Led_Error_On();
        Wait_ms (500);
        Led_Slave_Off();
        Led_Master_Off();
        Led_Error_Off();
        
    }
    else
    {
        Master_Slave_Config(SLAVE_CONFIG);

        for (int i = 0; i < 3; i++)
        {
            Led_Slave_On();
            Led_Master_On();
            Led_Error_On();
            Wait_ms (500);
            Led_Slave_Off();
            Led_Master_Off();
            Led_Error_Off();
            Wait_ms (500);	    
        }
        
    }

    //-- Main Loop --------------------------
    unsigned char voltage_status = 0;
    
    while (1)
    {
        // loop comms
        Master_Slave ();

        // check voltage status every 500ms
	if (!timer_standby)
	{
	    timer_standby = 500;
	    if ((Voltage_Sense > OVERVOLTAGE) &&
		(voltage_status != OVERVOLTAGE_STATUS))
	    {
		Change_Led_Voltage(OVERVOLTAGE_CMD);
		voltage_status = OVERVOLTAGE_STATUS;
	    }
	    else if ((Voltage_Sense < UNDERVOLTAGE) &&
		     (voltage_status != UNDERVOLTAGE_STATUS))
	    {
		Change_Led_Voltage(UNDERVOLTAGE_CMD);
		voltage_status = UNDERVOLTAGE_STATUS;            
	    }
	    else if (voltage_status != VOLTAGE_GOOD_STATUS)
	    {
		Change_Led_Voltage(VOLTAGE_GOOD_CMD);
		voltage_status = VOLTAGE_GOOD_STATUS;            
	    }
	}
	
        // update the led
        Update_Led_Voltage ();
    }
}

//--- End of Main ---//


// Other Module Functions ------------------------------------------------------
void TimingDelay_Decrement(void)
{
    if (wait_ms_var)
        wait_ms_var--;

    if (timer_standby)
        timer_standby--;

    Master_Slave_Timeouts ();

    Comms_Timeouts ();

    UsartTxTimeouts ();
}


void SysTickError (void)
{
    //Capture systick error...
    while (1)
    {
        // if (LED)
        //     LED_OFF;
        // else
        //     LED_ON;

        for (unsigned char i = 0; i < 255; i++)
        {
            asm ("nop \n\t"
                 "nop \n\t"
                 "nop \n\t" );
        }
    }
}

//--- end of file ---//

