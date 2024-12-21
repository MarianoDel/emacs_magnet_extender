//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### USART_CHANNELS.H ######################
//---------------------------------------------

#ifndef _USART_CHANNELS_H_
#define _USART_CHANNELS_H_


// Module Exported Macros ------------------------------------------------------
#define UsartChannel1Config()    Usart1Config()
#define UsartChannel1Send(X)    Usart1Send(X)
#define UsartChannel1ReadBuffer(X,Y)    Usart1ReadBuffer(X,Y)
#define UsartChannel1HaveData()    Usart1HaveData()
#define UsartChannel1HaveDataReset()    Usart1HaveDataReset()
#define UsartChannel1Shutdownn()    Usart1Shutdown()

#define UsartChannel2Config()    Uart4Config()
#define UsartChannel2Send(X)    Uart4Send(X)
#define UsartChannel2ReadBuffer(X,Y)    Uart4ReadBuffer(X,Y)
#define UsartChannel2HaveData()    Uart4HaveData()
#define UsartChannel2HaveDataReset()    Uart4HaveDataReset()
#define UsartChannel2Shutdownn()    Uart4Shutdown()

#define UsartChannel3Config()    Uart5Config()
#define UsartChannel3Send(X)    Uart5Send(X)
#define UsartChannel3ReadBuffer(X,Y)    Uart5ReadBuffer(X,Y)
#define UsartChannel3HaveData()    Uart5HaveData()
#define UsartChannel3HaveDataReset()    Uart5HaveDataReset()
#define UsartChannel3Shutdownn()    Uart5Shutdown()

#define UsartChannel4Config()    Usart2Config()
#define UsartChannel4Send(X)    Usart2Send(X)
#define UsartChannel4ReadBuffer(X,Y)    Usart2ReadBuffer(X,Y)
#define UsartChannel4HaveData()    Usart2HaveData()
#define UsartChannel4HaveDataReset()    Usart2HaveDataReset()
#define UsartChannel4Shutdownn()    Usart2Shutdown()

#define UsartRs485Config()    Usart3Config()
#define UsartRs485Send(X)    Usart3Send(X)
#define UsartRs485SendFinish()    Usart3SendFinish()
#define UsartRs485ReadBuffer(X,Y)    Usart3ReadBuffer(X,Y)
#define UsartRs485HaveData()    Usart3HaveData()
#define UsartRs485HaveDataReset()    Usart3HaveDataReset()
#define UsartRs485Shutdown()    Usart3Shutdown()


#endif     /* _USART_CHANNELS_H_ */
