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


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
char buff_ch1 [BUFFS_QTTY][SIZEOF_BUFFS];
char buff_ch2 [BUFFS_QTTY][SIZEOF_BUFFS];
char buff_ch3 [BUFFS_QTTY][SIZEOF_BUFFS];
char buff_ch4 [BUFFS_QTTY][SIZEOF_BUFFS];

char buff_tx_485 [SIZEOF_BUFFS * 4];

char * ptx1;
char * ptx2;
char * ptx3;
char * ptx4;

char * prx1;
char * prx2;
char * prx3;
char * prx4;

unsigned char comms_pckt_ready = 0;


// Module Private Functions ----------------------------------------------------
void Comms_Send_Channels_Buffer (unsigned char which_buff, char * buff, unsigned char len);
void Comms_Channel1 (void);
void Comms_Channel2 (void);
void Comms_Channel3 (void);
void Comms_Channel4 (void);


// Module Functions ------------------------------------------------------------
// void Comms_Timeouts (void)
// {
//     if (comms_timeout)
//         comms_timeout--;
// }

void Comms_Init (void)
{
    ptx1 = &buff_ch1[0][0];
    ptx2 = &buff_ch2[0][0];
    ptx3 = &buff_ch3[0][0];
    ptx4 = &buff_ch4[0][0];

    prx1 = &buff_ch1[0][0];
    prx2 = &buff_ch2[0][0];
    prx3 = &buff_ch3[0][0];
    prx4 = &buff_ch4[0][0];        
}


void Comms_Parse_Rs485_Rx_Buff (char * pbuff, unsigned short len)
{
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

            if (qtty == 4)
                break;
        }
    }

    // check semi colon qtty
    if (qtty != 4)
        return;

    // resend to channels each buffer
    for (int i = 0; i < 4; i++)
    {
        // buff start and length
        if (i)
        {
            bs = semi_colon_buff[i - 1] + 1;    // count ';'
            bl = semi_colon_buff[i] - semi_colon_buff[i - 1] - 1;    // count ';'
        }
        else
        {
            bs = 0;
            bl = semi_colon_buff[0];
        }

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
    lbuff[len] = '\n';
    
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


void Comms_Send_Rs485_Tx_Buff (void)
{
    // copy each string to send
    // channel 1
    if (prx1 != ptx1)
    {
        strcpy(buff_tx_485, ptx1);
        strcat(buff_tx_485, ";");

        if (ptx1 < &buff_ch1[4][0])
            ptx1 += SIZEOF_BUFFS;
        else
            ptx1 = &buff_ch1[0][0];
    }
    else
        strcpy(buff_tx_485, ";");

    // channel 2
    if (prx2 != ptx2)
    {
        strcat(buff_tx_485, ptx2);
        strcat(buff_tx_485, ";");

        if (ptx2 < &buff_ch2[4][0])
            ptx2 += SIZEOF_BUFFS;
        else
            ptx2 = &buff_ch2[0][0];
    }
    else
        strcat(buff_tx_485, ";");

    // channel 3
    if (prx3 != ptx3)
    {
        strcat(buff_tx_485, ptx3);
        strcat(buff_tx_485, ";");

        if (ptx3 < &buff_ch3[4][0])
            ptx3 += SIZEOF_BUFFS;
        else
            ptx3 = &buff_ch3[0][0];
    }
    else
        strcat(buff_tx_485, ";");

    // channel 4
    if (prx4 != ptx4)
    {
        strcat(buff_tx_485, ptx4);
        strcat(buff_tx_485, ";");

        if (ptx4 < &buff_ch4[4][0])
            ptx4 += SIZEOF_BUFFS;
        else
            ptx4 = &buff_ch4[0][0];
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
        strncpy(prx1, buff, len);
        
        if (prx1 < &buff_ch1[4][0])
            prx1 += SIZEOF_BUFFS;
        else
            prx1 = &buff_ch1[0][0];

        break;

    case 1:
        strncpy(prx2, buff, len);
        
        if (prx2 < &buff_ch2[4][0])
            prx2 += SIZEOF_BUFFS;
        else
            prx2 = &buff_ch2[0][0];

        break;

    case 2:
        strncpy(prx3, buff, len);
        
        if (prx3 < &buff_ch3[4][0])
            prx3 += SIZEOF_BUFFS;
        else
            prx3 = &buff_ch3[0][0];

        break;

    case 3:
        strncpy(prx4, buff, len);
        
        if (prx4 < &buff_ch4[4][0])
            prx4 += SIZEOF_BUFFS;
        else
            prx4 = &buff_ch4[0][0];

        break;
    }
}


unsigned char Comms_Is_Sending (void)
{
    // check if usart is sending
    return Usart1IsSending ();
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


void Comms_Update_Channels (void)
{
    Comms_Channel1 ();
    Comms_Channel2 ();
    Comms_Channel3 ();
    Comms_Channel4 ();
}


//channel 1, connected to usart2
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


//channel 2, connected to usart3
void Comms_Channel2 (void)
{
    if (UsartChannel2HaveData())
    {
        UsartChannel2HaveDataReset();
        
        UsartChannel2ReadBuffer (ch_buffs, SIZEOF_BUFFS);

        Comms_Fill_Rs485_Tx_Buff(CH2, ch_buffs);
    }
}


//channel 3, connected to uart4
void Comms_Channel3 (void)
{
    if (UsartChannel3HaveData())
    {
        UsartChannel3HaveDataReset();
        
        UsartChannel3ReadBuffer (ch_buffs, SIZEOF_BUFFS);

        Comms_Fill_Rs485_Tx_Buff(CH3, ch_buffs);
    }
}


//channel 4, connected to uart5
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
