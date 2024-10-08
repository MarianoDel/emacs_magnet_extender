//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_MASTER_SLAVE.C ##################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "master_slave.h"

//helper modules
#include "tests_ok.h"


#include <stdio.h>
#include <string.h>


// Globals externals for module to test ----------------------------------------


// Externals globals from module to test -----------------------------------------------


// Globals ---------------------------------------------------------------------



// Module Auxialiary Functions -------------------------------------------------


// Module Functions for testing ------------------------------------------------
void Test_Master_Sending (void);
void Test_Slave_Sending (void);


// Module Functions ------------------------------------------------------------


int main (int argc, char *argv[])
{

    // Test_Master_Sending ();

    Test_Slave_Sending ();    

}


int turn_cnt = 0;
int waiting_send = 0;
int pckt_ready = 0;
int master_pin = 1;
int wait_in_tx = 0;
void Test_Master_Sending (void)
{
    printf("\nTest on Master_Slave_In_Master\n");
    Master_Slave_Config(MASTER_CONFIG);

    for (int i = 0; i < 300; i++)
    {
        turn_cnt = i;
        Master_Slave_Timeouts();
        Master_Slave ();
    }
}


void Test_Slave_Sending (void)
{
    printf("\nTest on Master_Slave_In_Slave\n");
    Master_Slave_Config(SLAVE_CONFIG);

    for (int i = 0; i < 300; i++)
    {
        turn_cnt = i;

        if ((i == 20) ||
            (i == 100))
            pckt_ready = 1;

        Master_Slave_Timeouts();
        Master_Slave ();
    }
}





// Module Mocked Functions -----------------------------------------------------
void Comms_Send_Rs485_Tx_Buff (void)
{
    wait_in_tx = 20;
    printf("sending on rs485 turn: %d wait for: %d\n", turn_cnt, wait_in_tx);
}


void Led1_On (void)
{
    printf("led1 is on \n");
}


void Led1_Off (void)
{
    printf("led1 is off\n");
}


void Led2_On (void)
{
    printf("led2 is on\n");
}


void Led2_Off (void)
{
    printf("led2 is off\n");
}


void Enable_DE (void)
{
    printf("DE enable turn: %d\n", turn_cnt);
}


void Disable_DE (void)
{
    printf("DE disable turn: %d\n", turn_cnt);
}


unsigned char Comms_Is_Sending (void)
{
    if (wait_in_tx)
    {
        wait_in_tx--;
        return 1;
    }
    
    return 0;
}


unsigned char Comms_Get_Packet_Ready (void)
{
    if (pckt_ready)
        printf("packet ready on turn: %d\n", turn_cnt);
    
    return pckt_ready;
}


void Comms_Reset_Packet_Ready (void)
{
    pckt_ready = 0;
}


unsigned char Master_Pin (void)
{
    return master_pin;
}
//--- end of file ---//


