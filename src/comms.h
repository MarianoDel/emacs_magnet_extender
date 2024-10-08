//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### COMMS.H ###############################
//---------------------------------------------

#ifndef _COMMS_H_
#define _COMMS_H_

//---- Includes to help the Defines ----------


//---- Configurations Defines --------------------


// Module Exported Functions ---------------------------------------------------
void Comms_Init (void);
void Comms_Parse_Rs485_Rx_Buff (char * pbuff, unsigned short len);
void Comms_Parse_Rs485_Rx_Buff (char * pbuff, unsigned short len);
void Comms_Send_Rs485_Tx_Buff (void);
void Comms_Fill_Rs485_Tx_Buff (unsigned char which_ch, char * buff);

void Comms_Set_Packet_Ready (void);
void Comms_Reset_Packet_Ready (void);
unsigned char Comms_Get_Packet_Ready (void);

void Comms_Update_Channels (void);
unsigned char Comms_Is_Sending (void);



#endif    /* _COMMS_H_ */
