//---------------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMMS.C ###########################################
//---------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "comms.h"
#include "answers_defs.h"
#include "hard.h"

#include "usart_channels.h"
#include "usart.h"
#include "channels_defs.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Module Private Types Constants and Macros -----------------------------------
#define SIZEOF_BUFFS    100
#define BUFFS_QTTY    5
#define COMMS_TT_RELOAD    3000

// #define USE_CHANNELS_SINGLE_BUFFERS
#define USE_CHANNELS_MULTIPLE_BUFFERS

// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
char buff_ch1 [BUFFS_QTTY][SIZEOF_BUFFS];
char buff_ch2 [BUFFS_QTTY][SIZEOF_BUFFS];
char buff_ch3 [BUFFS_QTTY][SIZEOF_BUFFS];
char buff_ch4 [BUFFS_QTTY][SIZEOF_BUFFS];

char buff_tx_485 [SIZEOF_BUFFS * 4];

char * ptx_ch1;
char * ptx_ch2;
char * ptx_ch3;
char * ptx_ch4;

char * prx_ch1;
char * prx_ch2;
char * prx_ch3;
char * prx_ch4;

unsigned char comms_pckt_ready = 0;
volatile unsigned char comms_rs232_timeout = 0;


// Module Private Functions ----------------------------------------------------
void Comms_Send_Channels_Buffer (unsigned char which_buff, char * buff, unsigned char len);
void Comms_Channel1 (void);
void Comms_Channel2 (void);
void Comms_Channel3 (void);
void Comms_Channel4 (void);


// Module Functions ------------------------------------------------------------
void Comms_Timeouts (void)
{
    if (comms_rs232_timeout)
        comms_rs232_timeout--;
}


void Comms_Init (void)
{
    ptx_ch1 = &buff_ch1[0][0];
    ptx_ch2 = &buff_ch2[0][0];
    ptx_ch3 = &buff_ch3[0][0];
    ptx_ch4 = &buff_ch4[0][0];

    prx_ch1 = &buff_ch1[0][0];
    prx_ch2 = &buff_ch2[0][0];
    prx_ch3 = &buff_ch3[0][0];
    prx_ch4 = &buff_ch4[0][0];        
}


void Comms_Parse_Rs485_Rx_Buff (char * pbuff, unsigned short len)
{
    // the new string getted is ;;;;; when empty
    unsigned short semi_colon_buff [5] = { 0 };
    unsigned char qtty = 0;
    unsigned char bl = 0;
    unsigned char bs = 0;
    
    // get the 4 strings
    for (unsigned short i = 0; i < len; i++)
    {
        if (*(pbuff + i) == ';')
        {
            semi_colon_buff[qtty] = i;
            qtty++;

            if (qtty == 5)
                break;
        }
    }

    // check semi colon qtty
    if (qtty != 5)
        return;

    // resend to channels each buffer
    for (int i = 0; i < 4; i++)
    {
        // buff start and length
	bs = semi_colon_buff[i] + 1;    // count ';'
	bl = semi_colon_buff[i+1] - semi_colon_buff[i] - 1;    // count ';'
	
        if (bl)
            Comms_Send_Channels_Buffer (i, pbuff + bs, bl);
    }

    Comms_Set_Packet_Ready();
}


void Comms_Send_Channels_Buffer (unsigned char which_buff, char * buff, unsigned char len)
{
    char lbuff [128] = { 0 };

    if (len > 126)    // +'\n\0'
        return;
    
    strncpy(lbuff, buff, len);
    lbuff[len] = '\0';
    strcat(lbuff, "\r\n");
    
    switch (which_buff)
    {
    case 0:
        UsartChannel1Send(lbuff);
        break;

    case 1:
        UsartChannel2Send(lbuff);
        break;

    case 2:
        UsartChannel3Send(lbuff);        
        break;

    case 3:
        UsartChannel4Send(lbuff);        
        break;
    }
}


#ifdef USE_CHANNELS_SINGLE_BUFFERS
void Comms_Send_Rs485_Tx_Buff (void)
{
    unsigned char len = 0;
    
    // copy each string to send
    // channel 1
    len = strlen(&buff_ch1[0][0]);
    if (len)
    {
	strcpy(buff_tx_485, ";");
        strcat(buff_tx_485, &buff_ch1[0][0]);
        strcat(buff_tx_485, ";");
	buff_ch1[0][0] = '\0';
    }
    else
        strcpy(buff_tx_485, ";;");

    // channel 2
    len = strlen(&buff_ch2[0][0]);
    if (len)
    {
        strcat(buff_tx_485, &buff_ch2[0][0]);
        strcat(buff_tx_485, ";");
	buff_ch2[0][0] = '\0';
    }
    else
        strcat(buff_tx_485, ";");

    // channel 3
    len = strlen(&buff_ch3[0][0]);
    if (len)
    {
        strcat(buff_tx_485, &buff_ch3[0][0]);
        strcat(buff_tx_485, ";");
	buff_ch3[0][0] = '\0';
    }
    else
        strcat(buff_tx_485, ";");

    // channel 4
    len = strlen(&buff_ch4[0][0]);
    if (len)
    {
        strcat(buff_tx_485, &buff_ch4[0][0]);
        strcat(buff_tx_485, ";");
	buff_ch4[0][0] = '\0';
    }
    else
        strcat(buff_tx_485, ";");

    strcat(buff_tx_485, "\n");    

    // send the buffer
    UsartRs485Send(buff_tx_485);
}


void Comms_Fill_Rs485_Tx_Buff (unsigned char which_ch, char * buff)
{
    // copy string to send
    int len = 0;

    len = strlen(buff);
    
    if (len > SIZEOF_BUFFS - 1)
        return;
    
    switch (which_ch)
    {
    case 0:
	strcpy(&buff_ch1[0][0], buff);
        break;

    case 1:
	strcpy(&buff_ch2[0][0], buff);	
        break;

    case 2:
	strcpy(&buff_ch3[0][0], buff);	
        break;

    case 3:
	strcpy(&buff_ch4[0][0], buff);	
        break;
    }
}
#endif    // USE_CHANNELS_SINGLE_BUFFERS


#ifdef USE_CHANNELS_MULTIPLE_BUFFERS
void Comms_Send_Rs485_Tx_Buff (void)
{
    // copy each string to send
    // channel 1
    if (prx_ch1 != ptx_ch1)
    {
	strcpy(buff_tx_485, ";");
        strcat(buff_tx_485, ptx_ch1);
        strcat(buff_tx_485, ";");

        if (ptx_ch1 < &buff_ch1[4][0])
            ptx_ch1 += SIZEOF_BUFFS;
        else
            ptx_ch1 = &buff_ch1[0][0];
    }
    else
        strcpy(buff_tx_485, ";;");

    // channel 2
    if (prx_ch2 != ptx_ch2)
    {
        strcat(buff_tx_485, ptx_ch2);
        strcat(buff_tx_485, ";");

        if (ptx_ch2 < &buff_ch2[4][0])
            ptx_ch2 += SIZEOF_BUFFS;
        else
            ptx_ch2 = &buff_ch2[0][0];
    }
    else
        strcat(buff_tx_485, ";");

    // channel 3
    if (prx_ch3 != ptx_ch3)
    {
        strcat(buff_tx_485, ptx_ch3);
        strcat(buff_tx_485, ";");

        if (ptx_ch3 < &buff_ch3[4][0])
            ptx_ch3 += SIZEOF_BUFFS;
        else
            ptx_ch3 = &buff_ch3[0][0];
    }
    else
        strcat(buff_tx_485, ";");

    // channel 4
    if (prx_ch4 != ptx_ch4)
    {
        strcat(buff_tx_485, ptx_ch4);
        strcat(buff_tx_485, ";");

        if (ptx_ch4 < &buff_ch4[4][0])
            ptx_ch4 += SIZEOF_BUFFS;
        else
            ptx_ch4 = &buff_ch4[0][0];
    }
    else
        strcat(buff_tx_485, ";");

    strcat(buff_tx_485, "\n");    

    // send the buffer
    UsartRs485Send(buff_tx_485);
}


void Comms_Fill_Rs485_Tx_Buff (unsigned char which_ch, char * buff)
{
    // copy string to send
    int len = 0;

    len = strlen(buff);
    
    if (len > SIZEOF_BUFFS - 1)
        return;
    
    switch (which_ch)
    {
    case 0:
        strcpy(prx_ch1, buff);
        
        if (prx_ch1 < &buff_ch1[4][0])
            prx_ch1 += SIZEOF_BUFFS;
        else
            prx_ch1 = &buff_ch1[0][0];

        break;

    case 1:
        strcpy(prx_ch2, buff);
        
        if (prx_ch2 < &buff_ch2[4][0])
            prx_ch2 += SIZEOF_BUFFS;
        else
            prx_ch2 = &buff_ch2[0][0];

        break;

    case 2:
        strcpy(prx_ch3, buff);
        
        if (prx_ch3 < &buff_ch3[4][0])
            prx_ch3 += SIZEOF_BUFFS;
        else
            prx_ch3 = &buff_ch3[0][0];

        break;

    case 3:
        strcpy(prx_ch4, buff);
        
        if (prx_ch4 < &buff_ch4[4][0])
            prx_ch4 += SIZEOF_BUFFS;
        else
            prx_ch4 = &buff_ch4[0][0];

        break;
    }
}
#endif    // USE_CHANNELS_MULTIPLE_BUFFERS

unsigned char Comms_Is_Sending (void)
{
    // check if usart rs485 is sending
    return (!UsartRs485SendFinish ());
}


void Comms_Set_Packet_Ready (void)
{
    comms_pckt_ready = 1;
}


void Comms_Reset_Packet_Ready (void)
{
    comms_pckt_ready = 0;
}


unsigned char Comms_Get_Packet_Ready (void)
{
    return comms_pckt_ready;
}


void Comms_Rs232_Kick (void)
{
    comms_rs232_timeout = 5;
}


unsigned char Comms_Rs232_Get_Timeout (void)
{
    return comms_rs232_timeout;
}


void Comms_Update_Channels (void)
{
    Comms_Channel1 ();
    Comms_Channel2 ();
    Comms_Channel3 ();
    Comms_Channel4 ();
}


unsigned char Comms_Update_Rs485 (void)
{
    char buff [4*128];
    unsigned short len = 0;
    unsigned char process = 0;

    if (UsartRs485HaveData())
    {
	UsartRs485HaveDataReset();
	len = UsartRs485ReadBuffer(buff, 4*128);

	// if (len == 5)
	// {
	//     if (Led_Master_Is_On())
	// 	Led_Master_Off();
	//     else
	// 	Led_Master_On();
	// }

	// for tests
	// char btest[100];
	// int len2 = strlen(buff);
	// sprintf(btest, "get len: %d strlen: %d\r\n", len, len2);
	// UsartChannel1Send(btest);
	// UsartChannel1Send(buff);	
	// UsartChannel1Send("\r\n");
	// end for tests

	Comms_Parse_Rs485_Rx_Buff(buff, len);
	process = 1;
    }

    return process;
}


//channel 1
char ch_buffs[SIZEOF_BUFFS];
void Comms_Channel1 (void)
{
    if (UsartChannel1HaveData())
    {
        UsartChannel1HaveDataReset();

        UsartChannel1ReadBuffer (ch_buffs, SIZEOF_BUFFS);

        Comms_Fill_Rs485_Tx_Buff(CH1, ch_buffs);
    }
}


//channel 2
void Comms_Channel2 (void)
{
    if (UsartChannel2HaveData())
    {
        UsartChannel2HaveDataReset();
        
        UsartChannel2ReadBuffer (ch_buffs, SIZEOF_BUFFS);

        Comms_Fill_Rs485_Tx_Buff(CH2, ch_buffs);
    }
}


//channel 3
void Comms_Channel3 (void)
{
    if (UsartChannel3HaveData())
    {
        UsartChannel3HaveDataReset();
        
        UsartChannel3ReadBuffer (ch_buffs, SIZEOF_BUFFS);

        Comms_Fill_Rs485_Tx_Buff(CH3, ch_buffs);
    }
}


//channel 4
void Comms_Channel4 (void)
{
    if (UsartChannel4HaveData())
    {
        UsartChannel4HaveDataReset();
        
        UsartChannel4ReadBuffer (ch_buffs, SIZEOF_BUFFS);

        Comms_Fill_Rs485_Tx_Buff(CH4, ch_buffs);
    }
}

//---- End of File ----//
