/*
*								Libreria de Comunicación Serial
*
*	Filename   : COM.h
*	Programmer : Alejandro Galfrascoli
*********************************************************************************************************
*/
#ifndef INC_COM_H_
#define INC_COM_H_
/*
*********************************************************************************************************
*                                        		LIBRARIES
*********************************************************************************************************
*/
#include "includes.h"
/*
*********************************************************************************************************
*                                        CONFIGURATION CONSTANTS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        		VARIABLES
*********************************************************************************************************
*/

OS_EVENT 		*UARTSem;											// SEMAFORO para compartir la UART

/*
*********************************************************************************************************
*                                          FUNCTIONS PROTOTYPES
*********************************************************************************************************
*/

void COMInit(void);
uint8_t COMPuts(uint8_t* pData);
uint8_t COMGets(uint8_t *pData, uint8_t size);

/*
**********************************************************************************************************
*/
#endif /* INC_COM_H_ */
