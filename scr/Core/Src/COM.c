/*
*								Libreria de Comunicación Serial
*
*	Filename   : COM.C
*	Programmer : Alejandro Galfrascoli
*********************************************************************************************************
*/
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
extern UART_HandleTypeDef huart1;

/*
*********************************************************************************************************
*                                          		FUNCTIONS
*********************************************************************************************************
* @brief  Inicializa la instancia de UART1 y crea el Semaforo de acceso exclusivo 'UARTSem'.
* @param  None
* @retval None
*/
void COMInit(void) {								/* Inicializacion de la UART */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}

	UARTSem = OSSemCreate(1);						/* creo un semaforo para acceso exclusivo a UART */
}
/*
*********************************************************************************************************
* @brief  Envía una cadena de caracteres por el puerto serial.
* @param  pData: puntero al string que se enviará.
* @retval stat: es el codigo de error definido por las libreria HAL para HAL_UART_Transmit() y vale:
* 				HAL_OK       = 0x00U
* 				HAL_ERROR    = 0x01U
* 				HAL_BUSY     = 0x02U
* 				HAL_TIMEOUT  = 0x03U
*/
uint8_t COMPuts(uint8_t* pData) {
	uint8_t err, stat;

	OSSemPend(UARTSem, 50, &err);							/* Aseguro el acceso exlusivo a UART */
	if(err == OS_ERR_NONE){
		stat = HAL_UART_Transmit(&huart1, pData, strlen((char*)pData), HAL_MAX_DELAY);
		OSSemPost(UARTSem);
	}
	return stat;
}

/*
*********************************************************************************************************
* @brief  Recibe una cadena de caracteres por el puerto serial.
* @param  *pData: puntero donde se guarda el dato recibido.
* @param  size: cantidad de caracteres a recibir.
* @retval Retorna el estado de la recepcion definidos en libreria HAL al igual que para la transmision
*/
uint8_t COMGets(uint8_t *pData, uint8_t size) {
	uint8_t stat;

	stat = HAL_UART_Receive(&huart1, pData, size, 1*size);		// TO de 1ms por caracter
	return stat;
}
/*
*********************************************************************************************************
*/
