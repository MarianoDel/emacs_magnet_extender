//------------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### MASTER_SLAVE.C #################################
//------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "master_slave.h"
#include "hard.h"
#include "comms.h"


// Private Types Constants and Macros ------------------------------------------
typedef enum {
    master_ready_to_send_data,
    master_sending_data,
    master_wait_finish_data,
    master_wait_free_bus,
    master_waiting_slave_data,
    master_slave_data_getted
    
} master_states_e;

typedef enum {
    slave_wait_to_get_data,
    slave_space_before_send,
    slave_sending_data,
    slave_wait_finish_data,
    slave_wait_free_bus
    
} slave_states_e;

// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
master_slave_e master_config = MASTER_CONFIG;
unsigned short master_timer = 0;
unsigned short slave_timer = 0;
master_states_e master_state = master_ready_to_send_data;
slave_states_e slave_state = slave_wait_to_get_data;


// Module Private Functions ----------------------------------------------------
void Master_Slave_In_Master (void);
void Master_Slave_In_Slave (void);


// Module Functions ------------------------------------------------------------
void Master_Slave_Config (master_slave_e config)
{
    master_config = config;
}


unsigned char Master_Is_On (void)
{
    return Master_Pin();
}


void Master_Slave (void)
{
    if (master_config == MASTER_CONFIG)
        Master_Slave_In_Master ();
    else
        Master_Slave_In_Slave ();

    // always get channels data
    Comms_Update_Channels();
}


void Master_Slave_In_Master (void)
{
    switch (master_state)
    {
    case master_ready_to_send_data:
        Led_Master_Off();
        master_timer = 1;
        Enable_DE();
        master_state++;
        break;

    case master_sending_data:
        if (master_timer)
	    break;
	
	Comms_Send_Rs485_Tx_Buff();
	master_state++;
        break;

    case master_wait_finish_data:
        if (!Comms_Is_Sending ())
        {
            master_timer = 1;
            master_state++;            
        }
        break;

    case master_wait_free_bus:
        if (master_timer)
	    break;

	Disable_DE();
	Led_Master_On();
	// master_timer = 150;
	master_timer = 200;	
	// master_timer = 1000;
	master_state++;            
        break;
        
    case master_waiting_slave_data:
	// exit by timeout
        if (!master_timer)
	{
	    // end for no data or timeout
	    if (Comms_Rs232_Get_Timeout() == 0)
		master_state = master_ready_to_send_data;
	}
	
	// exit with packet processed
	if (Comms_Update_Rs485())
	{
	    // packet getted and process
	    master_state++;
	}

	// always out on timeout
	// master_state++;
        break;

    case master_slave_data_getted:
        if (master_timer)
	    break;

	master_state = master_ready_to_send_data;
	
        break;
    }
}


void Master_Slave_In_Slave (void)
{
    // always get channels data
    switch (slave_state)
    {
    case slave_wait_to_get_data:
	// check for new packet
	Comms_Update_Rs485 ();
	
        if (Comms_Get_Packet_Ready())
        {
            Comms_Reset_Packet_Ready();
            Led_Slave_Off();
            slave_timer = 5;
            slave_state++;
        }
        break;

    case slave_space_before_send:
	if (slave_timer)
	    break;

	slave_timer = 1;
	Enable_DE();
	slave_state++;	    
	break;
	
    case slave_sending_data:
        if (slave_timer)
	    break;

	Comms_Send_Rs485_Tx_Buff();
	slave_state++;
        break;

    case slave_wait_finish_data:
        if (!Comms_Is_Sending ())
        {
            slave_timer = 1;
            slave_state++;            
        }        
        break;

    case slave_wait_free_bus:
        if (slave_timer)
	    break;

	Disable_DE();
	Led_Slave_On();
	slave_state = slave_wait_to_get_data;
        break;
    }    
}


void Master_Slave_Timeouts (void)
{
    if (master_timer)
        master_timer--;

    if (slave_timer)
        slave_timer--;
}

//---- end of file ----//
