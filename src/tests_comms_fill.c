//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_COMMS_FILL.C ####################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "comms.h"

//helper modules
#include "tests_ok.h"


#include <stdio.h>
#include <string.h>


// Globals externals for module to test ----------------------------------------


// Externals globals from module to test -----------------------------------------------
#define SIZEOF_BUFFS    100
#define BUFFS_QTTY    5

extern char buff_ch1[BUFFS_QTTY][SIZEOF_BUFFS];
extern char buff_ch2[BUFFS_QTTY][SIZEOF_BUFFS];
extern char buff_ch3[BUFFS_QTTY][SIZEOF_BUFFS];
extern char buff_ch4[BUFFS_QTTY][SIZEOF_BUFFS];


// Globals ---------------------------------------------------------------------



// Module Auxialiary Functions -------------------------------------------------


// Module Functions for testing ------------------------------------------------
void Test_Comms_Parse_Rs485 (void);
void Test_Comms_Fill_Rs485 (void);
void Test_Comms_Send_Rs485 (void);


// Module Functions ------------------------------------------------------------


int main (int argc, char *argv[])
{

    Test_Comms_Parse_Rs485 ();

    // Test_Comms_Fill_Rs485 ();

    // Test_Comms_Send_Rs485 ();

}


void Test_Comms_Parse_Rs485 (void)
{
    printf("\nTest on Comms_Parse_Rs485_Rx_Buff\n");
    Comms_Init();

    char str [500];
    int len = 0;
    
    strcpy (str,"1 ok;2 ok;3 nok;4 ok;");
    len = strlen(str);
    printf("parse len: %d str: %s\n", len, str);
    Comms_Parse_Rs485_Rx_Buff (str, len);

    // strcpy (str,";;;;");
    // len = strlen(str);
    // printf("parse len: %d str: %s\n", len, str);
    // Comms_Parse_Rs485_Rx_Buff (str, len);

    // strcpy (str,"0;0;0;0;");
    // len = strlen(str);
    // printf("parse len: %d str: %s\n", len, str);
    // Comms_Parse_Rs485_Rx_Buff (str, len);

    // strcpy (str,";;0;0;");
    // len = strlen(str);
    // printf("parse len: %d str: %s\n", len, str);
    // Comms_Parse_Rs485_Rx_Buff (str, len);
    
}


void Test_Comms_Fill_Rs485 (void)
{
    printf("\nTest on Comms_Fill_Rs485_Tx_Buff\n");
    Comms_Init();

    printf("\n -Ch1 buffer-\n");
    for (int i = 0; i < 5; i++)
        printf(" [%d][0]: %s\n", i, &buff_ch1[i][0]);
    
    printf("\n Ch1 fill: ");
    int len = 0;
    int error = 0;
    char str [128];


    strcpy(str, "fill 0");
    Comms_Fill_Rs485_Tx_Buff(0, str);
    
    printf("\n -Ch1 buffer-\n");
    for (int i = 0; i < 5; i++)
        printf(" [%d][0]: %s\n", i, &buff_ch1[i][0]);

    strcpy(str, "fill 1");
    Comms_Fill_Rs485_Tx_Buff(0, str);
    
    printf("\n -Ch1 buffer-\n");
    for (int i = 0; i < 5; i++)
        printf(" [%d][0]: %s\n", i, &buff_ch1[i][0]);

    strcpy(str, "fill 2");
    Comms_Fill_Rs485_Tx_Buff(0, str);
    
    printf("\n -Ch1 buffer-\n");
    for (int i = 0; i < 5; i++)
        printf(" [%d][0]: %s\n", i, &buff_ch1[i][0]);
    
    strcpy(str, "fill 3");
    Comms_Fill_Rs485_Tx_Buff(0, str);
    
    printf("\n -Ch1 buffer-\n");
    for (int i = 0; i < 5; i++)
        printf(" [%d][0]: %s\n", i, &buff_ch1[i][0]);

    strcpy(str, "fill 4");
    Comms_Fill_Rs485_Tx_Buff(0, str);
    
    printf("\n -Ch1 buffer-\n");
    for (int i = 0; i < 5; i++)
        printf(" [%d][0]: %s\n", i, &buff_ch1[i][0]);

    // strcpy(str, "fill 5");
    // Comms_Fill_Rs485_Tx_Buff(0, str);
    
    // printf("\n -Ch1 buffer-\n");
    // for (int i = 0; i < 5; i++)
    //     printf(" [%d][0]: %s\n", i, &buff_ch1[i][0]);

    printf("Test all channels fill buffer\n");
    int loopi = 0;
    for (int j = 0; j < 4; j++)
    {
        printf("Testing Ch%d Fill: ",j+1);    
        loopi = 0;
        for (int i = 0; i < 20; i++)
        {
            sprintf(str, "fill %d%d", j,i);
            len = strlen(str);
        
            Comms_Fill_Rs485_Tx_Buff(j, str);

            int turn = i / 5;
            loopi = i - turn * 5;
            
            switch (j)
            {
            case 0:
                if (strncmp(&buff_ch1[loopi][0], str, len) != 0)
                {
                    error = 1;
                    printf("i: %d error str: %s buff: %s\n", i, str, &buff_ch1[i][0]);
                }
                break;

            case 1:
                if (strncmp(&buff_ch2[loopi][0], str, len) != 0)
                {
                    error = 1;
                    printf("i: %d error str: %s buff: %s\n", i, str, &buff_ch2[i][0]);
                }
                break;

            case 2:
                if (strncmp(&buff_ch3[loopi][0], str, len) != 0)
                {
                    error = 1;
                    printf("i: %d error str: %s buff: %s\n", i, str, &buff_ch3[i][0]);
                }
                break;

            case 3:
                if (strncmp(&buff_ch4[loopi][0], str, len) != 0)
                {
                    error = 1;
                    printf("i: %d error str: %s buff: %s\n", i, str, &buff_ch4[i][0]);
                }
                break;
            }
        }

        if (error)
            PrintERR();
        else
            PrintOK();

        printf("\n -Ch%d buffer-\n", j+1);
        for (int i = 0; i < 5; i++)
        {
            switch (j)
            {
            case 0:
                printf(" [%d][0]: %s\n", i, &buff_ch1[i][0]);
                break;

            case 1:
                printf(" [%d][0]: %s\n", i, &buff_ch2[i][0]);                
                break;

            case 2:
                printf(" [%d][0]: %s\n", i, &buff_ch3[i][0]);                
                break;

            case 3:
                printf(" [%d][0]: %s\n", i, &buff_ch4[i][0]);                
                break;
            }
        }
    }
}


void Test_Comms_Send_Rs485 (void)
{
    char str [128];

    printf("\nTest on Comms_Send_Rs485_Tx_Buff\n");
    Comms_Init();

    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            sprintf(str, "fill %d%d", j,i);
            Comms_Fill_Rs485_Tx_Buff(j, str);
        }
    }

    for (int i = 0; i < 10; i++)
        Comms_Send_Rs485_Tx_Buff();
    
}



// Module Mocked Functions -----------------------------------------------------
void Usart1Send (char * b)
{
    int len = strlen(b);
    printf("rpi usart1 len: %d buff: %s\n", len, b);

    printf("buff hex: ");
    for (int i = 0; i < len; i++)
        printf("0x%02x ", *(b + i));

    printf("\n\n");
}


void Usart2Send (char * b)
{
    int len = strlen(b);
    printf("ch1 usart2 len: %d buff: %s\n", len, b);

    printf("buff hex: ");
    for (int i = 0; i < len; i++)
        printf("0x%02x ", *(b + i));

    printf("\n\n");    
}


void Usart3Send (char * b)
{
    int len = strlen(b);
    printf("ch2 usart3 len: %d buff: %s\n", len, b);

    printf("buff hex: ");
    for (int i = 0; i < len; i++)
        printf("0x%02x ", *(b + i));

    printf("\n\n");
}


void Uart4Send (char * b)
{
    int len = strlen(b);
    printf("ch3 uart4 len: %d buff: %s\n", len, b);

    printf("buff hex: ");
    for (int i = 0; i < len; i++)
        printf("0x%02x ", *(b + i));

    printf("\n\n");
}


void Uart5Send (char * b)
{
    int len = strlen(b);
    printf("ch4 uart5 len: %d buff: %s\n", len, b);

    printf("buff hex: ");
    for (int i = 0; i < len; i++)
        printf("0x%02x ", *(b + i));

    printf("\n\n");
}


unsigned char Usart1HaveData (void)
{
    return 1;
}

unsigned char Usart2HaveData (void)
{
    return 1;
}

unsigned char Usart3HaveData (void)
{
    return 1;
}

unsigned char Uart4HaveData (void)
{
    return 1;
}

unsigned char Uart5HaveData (void)
{
    return 1;
}


void Usart1HaveDataReset (void)
{
}

void Usart2HaveDataReset (void)
{
}

void Usart3HaveDataReset (void)
{
}

void Uart4HaveDataReset (void)
{
}

void Uart5HaveDataReset (void)
{
}


unsigned char Usart3SendFinish (void)
{
    return 1;
}


unsigned short Usart1ReadBuffer (char * bout, unsigned short max_len)
{
    return 0;
}


unsigned short Usart2ReadBuffer (char * bout, unsigned short max_len)
{
    return 0;
}


unsigned short Usart3ReadBuffer (char * bout, unsigned short max_len)
{
    return 0;
}


unsigned short Uart4ReadBuffer (char * bout, unsigned short max_len)
{
    return 0;
}

unsigned short Uart5ReadBuffer (char * bout, unsigned short max_len)
{
    return 0;
}

int led_master = 0;
unsigned char Led_Master_Is_On (void)
{
    return (unsigned char) led_master;
}

void Led_Master_On (void)
{
    led_master = 1;
    printf("  led master ON\n");
}

void Led_Master_Off (void)
{
    led_master = 0;
    printf("  led master OFF\n");    
}


//--- end of file ---//


