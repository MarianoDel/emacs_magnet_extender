//---------------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### MASTER_SLAVE.H ####################################
//---------------------------------------------------------
#ifndef _MASTER_SLAVE_H_
#define _MASTER_SLAVE_H_


// Exported Types Constants and Macros -----------------------------------------
typedef enum {
    MASTER_CONFIG,
    SLAVE_CONFIG
    
} master_slave_e;


// Module Exported Functions ---------------------------------------------------
void Master_Slave (void);
void Master_Slave_Config (master_slave_e config);
unsigned char Master_Is_On (void);
void Master_Slave_Timeouts (void);


#endif    /* _MASTER_SLAVE_H_ */

//--- end of file ---//

