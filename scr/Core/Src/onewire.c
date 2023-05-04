/*
*********************************************************************************************************
*									Libreria para comunicacion 1-Wire
*
* NOTAS: algunas funciones basadas en la Nota de Aplicaciones 187 de Maxim (1-WIRE SEARCH ALGORITHM)
* 		 https://www.maximintegrated.com/en/design/technical-documents/app-notes/1/187.html
* 		 y en las hojas de datos de DS18B20, DS1820, DS18S20
*
* Filename   : onewire.h
* Programmer : Alejandro Galfrascoli
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
*                                        		VARIABLES
*********************************************************************************************************
*/
extern TIM_HandleTypeDef htim1;				// instancia del Timer1, necesario para los retardos del Bus

/*
*********************************************************************************************************
*                                          		FUNCTION
*********************************************************************************************************
* @brief  Esta funcion debe llamarse antes que ninguna para poder utilizar el bus 1-Wire
*         Configura el pin a utilizar e inicia el timer1 y la estructura de datos.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @param  GPIOx: es un puntero con la estructura del puerto del microcontrolador.
* @param  GPIO_Pin: es la definicion del pin que sera utilizado como linea DQ para el bus 1-Wire.
* @retval None
*/
void OW_Init(OneWire_t* onewire, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {

	OW_Config_Timer();							// No usada, el timer se setea desde CubeMx
	HAL_TIM_Base_Start(&htim1);					// inicia el  timer

	onewire->GPIOx = GPIOx;						// configuro el pin 1-wire
	onewire->GPIO_Pin = GPIO_Pin;
	OW_Set_Pin_Input(onewire);

	OW_Reset_Search(onewire);					// reinicia las variables para busqueda de dispositivos
}

/*
*********************************************************************************************************
* @brief  Genera la secuencia de reinicio en el bus 1-Wire, y queda a la espera del pulso de presencia
* 		  emitido por los dispositivos.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @retval 1 cuando detecta presencia de dispositivos y 0 cuando no se detecta.
*/
uint8_t OW_Reset(OneWire_t* onewire) {
	uint8_t response;
//	uint8_t retries = 125;

	OW_Set_Pin_Input(onewire);
/*
	do {												//espera hasta que el bus esté alto... por si acaso
		if(--retries == 0)
			return 0;
		OW_Delay(2);
	} while( !HAL_GPIO_ReadPin(onewire->GPIOx, onewire->GPIO_Pin));
*/
	OW_PinLow(onewire);
	OW_Set_Pin_Output(onewire);
	OW_Delay(480);   									// delay de acuerdo al datasheet 480us

	OW_Set_Pin_Input(onewire);   						// seteo el pin como input
	OW_Delay (70);
	response = !(HAL_GPIO_ReadPin (onewire->GPIOx, onewire->GPIO_Pin));

	OW_Delay(410); 										// otros 480us delay en total
	return response;
}

/*
*********************************************************************************************************
* @brief  Escribe un bit en el bus 1-Wire.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @param  bit: es el bit a enviar.
* @retval None
*/
void OW_Write_bit(OneWire_t* onewire, uint8_t bit) {

	if(bit & 1) {						// envio 1
		OW_PinLow(onewire);
		OW_Set_Pin_Output(onewire);
		OW_Delay(2);
		OW_Set_Pin_Input(onewire);
		OW_Delay(57);
	}
	else {								// envio 0
		OW_PinLow(onewire);
		OW_Set_Pin_Output(onewire);
		OW_Delay(60);
		OW_Set_Pin_Input(onewire);
		//OW_Delay(1);
	}
}
/*
*********************************************************************************************************
* @brief  Lee un bit en el bus 1-Wire.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @retval el valor del bit leido.
*/
uint8_t OW_Read_bit(OneWire_t* onewire) {
	uint8_t bit;

	OW_PinLow(onewire);
	OW_Set_Pin_Output(onewire);
	OW_Delay(2);
	OW_Set_Pin_Input(onewire);
	bit = HAL_GPIO_ReadPin(onewire->GPIOx, onewire->GPIO_Pin);
	OW_Delay(55);
	return bit;
}
/*
*********************************************************************************************************
* @brief  Escribe un byte completo en el bus 1-Wire.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @param  data: byte a enviar.
* @retval None
*/
void OW_Write(OneWire_t* onewire, uint8_t data) {
	uint8_t i = 8;

	//OW_Set_Pin_Output(onewire);
	do {
		OW_Write_bit(onewire, data & 1); 			// LSB Primero
		data >>= 1;
	} while(--i);

	OW_Set_Pin_Input(onewire);
}

/*
*********************************************************************************************************
* @brief  Lee un byte completo en el bus 1-Wire.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @retval el byte leido del bus
*/
uint8_t OW_Read(OneWire_t* onewire) {
	uint8_t i = 8, byte = 0;

	do {
		byte >>= 1;
		byte |= (OW_Read_bit(onewire) << 7); 		// LSB Primero
	} while(--i);
	return byte;
}

/*
*********************************************************************************************************
* @brief  Reinicia la estructura de busqueda.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @retval None
*/
void OW_Reset_Search(OneWire_t* onewire) {  						// resetear el estado de Search

	onewire->LastDiscrepancy = 0;
	onewire->LastDeviceFlag = 0;
	onewire->LastFamilyDiscrepancy = 0;

	for(int i = 7; ; i--) {
		onewire->ROM_NO[i] = 0;
		if( i == 0)
			break;
	}
}

/*
*********************************************************************************************************
* @brief Permite realizar la busqueda de dispositivos en el bus.
* @param *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @param command: Debe ser 0xF0: NORMAL SEARCH  - 0xEC: CONDITIONAL SEARCH.
* 				  Para mas datos remitirse a Maxim APPLICATION NOTE 187
* @retval 0 si la busqueda fue fallida y 1 si el resultado de la busqueda fue exitoso.
*/
uint8_t OW_Search(OneWire_t* onewire, uint8_t command) {
	CPU_SR   cpu_sr;						// para las macros OS_ENTER_CRITICAL() y OS_EXIT_CRITICAL()
	uint8_t id_bit_number;
	uint8_t last_zero, rom_byte_number, search_result;
	uint8_t id_bit, cmp_id_bit;
	uint8_t rom_byte_mask, search_direction;

	id_bit_number = 1;							// inicializo las variables para la busqueda
	last_zero = 0;
	rom_byte_number = 0;
	rom_byte_mask = 1;
	search_result = 0;

	if(!onewire->LastDeviceFlag) {
    	if(!OW_Reset(onewire)) {				// Si hay error en OW_Reset() - restablecer los resultados de búsqueda
    		onewire->LastDiscrepancy = 0;
    		onewire->LastDeviceFlag = 0;
    		onewire->LastFamilyDiscrepancy = 0;
    		return 0;
    	}
    OS_ENTER_CRITICAL();    									// Maxim APPLICATION NOTE 187
	OW_Write(onewire, command);									// 0xF0: NORMAL SEARCH  - 0xEC: CONDITIONAL SEARCH
      	  	  	  	  	  	  	 // *** bucle de busqueda ***
	do {
		id_bit = OW_Read_bit(onewire);							// leer un bit y luego su complemento
		cmp_id_bit = OW_Read_bit(onewire);

		if ((id_bit == 1) && (cmp_id_bit == 1)) 				// Si no hay dispositivos (condicion 11)... salir
			break;
		else {
            if (id_bit != cmp_id_bit)
            	search_direction = id_bit;  							// bit para busqueda
            else {														// condicion 00, hay 2 dispositivos
            	if (id_bit_number < onewire->LastDiscrepancy) 			// Tabla 3. Search Path Direction
            		search_direction = ((onewire->ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
            	else
            		search_direction = (id_bit_number == onewire->LastDiscrepancy);	// Si el bit es igual al último, 1 Si no, 0

               if (search_direction == 0) {
            	   last_zero = id_bit_number;

            	   if (last_zero < 9)									// comprobar la última discrepancia de familia
            		   onewire->LastFamilyDiscrepancy = last_zero;
               }
            }

            if (search_direction == 1)									// poner 1 o 0 en ROM
            	onewire->ROM_NO[rom_byte_number] |= rom_byte_mask;
            else
            	onewire->ROM_NO[rom_byte_number] &= ~rom_byte_mask;

            OW_Write_bit(onewire, search_direction);					// El master contesta con el bit recibido

            id_bit_number++;											// Incremento del contador
            rom_byte_mask <<= 1;										// Cambio la máscara para el próximo bit

            if (rom_byte_mask == 0) {									// Si la mascara es 0 restablezco para el proximo byte
                rom_byte_number++;
                rom_byte_mask = 1;
            }
		}
	} // fin del do {
	while(rom_byte_number < 8);  										// bucle hasta obtener los 8 bytes

	if (!(id_bit_number < 65)) {										// si la busqueda ha concluido...
		onewire->LastDiscrepancy = last_zero;

		if (onewire->LastDiscrepancy == 0) {							// Si LastDiscrepancy es 0, se encontró el último dispositivo
        	onewire->LastDeviceFlag = 1;
		}
		search_result = 1;							//  *** Busqueda exitosa ***
	}

	} 	// fin de if(!onewire->LastDeviceFlag)

	if(!search_result || !onewire->ROM_NO[0]) {		// Si no se encuentra ningún dispositivo...
		onewire->LastDiscrepancy = 0;
		onewire->LastDeviceFlag = 0;
		onewire->LastFamilyDiscrepancy = 0;
		search_result = 0;							// restablecer los datos de búsqueda... para devolver 0
	}

	else {
			// #### Aqui puedo devolder ROM_NO[]... ####
			// NO usada se crea la funcion OW_GetFullROM() para ello
	}
	OS_EXIT_CRITICAL();
	return search_result;
}

/*
*********************************************************************************************************
* @brief  Realiza la busqueda del primer dispositivo en el bus
* 		  por lo que primero reinicia la estructura de busqueda llamando a OW_Reset_Search().
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @retval 0 si la busqueda fue fallida y 1 si el resultado de la busqueda fue exitoso.
*/
uint8_t OW_First(OneWire_t* onewire) {

	OW_Reset_Search(onewire);
	return OW_Search(onewire, CMD_SEARCHROM);
}
/*
*********************************************************************************************************
* @brief  Realiza la busqueda de los siguientes dispositivos en el bus.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @retval 0 si la busqueda fue fallida y 1 si el resultado de la busqueda fue exitoso.
*/
uint8_t OW_Next(OneWire_t* onewire) {
	return OW_Search(onewire, CMD_SEARCHROM);
}

/*
*********************************************************************************************************
* @brief  Envia el comando MATCHROM		(Emparejar con la ROM dada)
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @param  *addr: puntero a la variable que contiene la ROM que será utilizada.
* @retval None
*/
void OW_Select(OneWire_t* onewire, uint8_t* addr) {
    uint8_t i;

    OW_Write(onewire, CMD_MATCHROM); 					// Comando: Match ROM - 0x55
    for (i = 0; i < 8; i++)
    	OW_Write(onewire, *(addr + i));
}

/*
*********************************************************************************************************
* @brief  Envia el comando SKIPROM		(Omitir la ROM)
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @retval None
*/
void OW_Skip(OneWire_t* onewire) {
    OW_Write(onewire, CMD_SKIPROM);           			// comando: Skip ROM  - 0xCC
}

/*
*********************************************************************************************************
* @brief  Almacena la ROM encontrada [con las funciones OW_Search(), OW_First(), OW_Next()] en un puntero
* 		  pasado como parametro.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @param  *newAddr: puntero la variable para almacenar la ROM.
* @retval None
*/
void OW_GetFullROM(OneWire_t* onewire, uint8_t* newAddr) {
	uint8_t i;
	for (i = 0; i < 8; i++)
		*(newAddr + i) = onewire->ROM_NO[i];
}

/*
*********************************************************************************************************
*/
uint8_t OW_CRC8(uint8_t *addr, uint8_t len) {
	uint8_t crc = 0, inbyte, i, mix;

	while(len--) {
		inbyte = *addr++;
		for(i = 8; i; i--) {
			mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix)
				crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}
/*
*********************************************************************************************************
*                                          		FUNCTION
*                                           HARDWARE SPECIFIC
*********************************************************************************************************
* @brief  Configuracion del Timer1 necesario para los retardos en el bus.
*
* 		  #### No utilizada actualmente, se configura el timer en el CubeMX ####
* @param  None
* @retval None
*/
void OW_Config_Timer(void) {
/*
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 72-1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 0xffff-1;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
*/
}

/*
 * *********************************************************************************************************
 * @brief  Genera retardos de us necesarios para el control del bus 1-Wire.
 * @note   Utiliza la instancia del Timer 1.
 * @param  us:	retardo en micro segundos.
 * @retval None
 */
void OW_Delay(uint16_t us) {
	htim1.Instance -> CNT = 0 ;
	while (htim1.Instance -> CNT <= us);
}

/*
**********************************************************************************************************
* @brief  Configuracion del GPIO como salida.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @retval None
*/
void OW_Set_Pin_Output(OneWire_t *onewire) {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;			// open-drain
	GPIO_InitStruct.Pull = GPIO_NOPULL;					// pull-up es externa 4k7
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = onewire->GPIO_Pin;
	HAL_GPIO_Init(onewire->GPIOx, &GPIO_InitStruct);
}
/*
*********************************************************************************************************
* @brief  Configuracion del GPIO como entrada.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @retval None
*/
void OW_Set_Pin_Input(OneWire_t *onewire) {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;				// input
	GPIO_InitStruct.Pull = GPIO_NOPULL;					// pull-up es externa 4k7
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = onewire->GPIO_Pin;
	HAL_GPIO_Init(onewire->GPIOx, &GPIO_InitStruct);
}
/*
*********************************************************************************************************
* @brief  Pone el bus 1-Wire a low.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @retval None
*/
void OW_PinLow(OneWire_t *onewire) {
	onewire->GPIOx->BSRR = onewire->GPIO_Pin << 16;			// Reset el pin 1-Wire
}
/*
*********************************************************************************************************
* @brief  Pone el bus 1-Wire a High.
* @param  *onewire: puntero a la estructura que contiene la definicion del puerto y pin a utilizar.
* @retval None
*/
void OW_PinHigh(OneWire_t *onewire) {
	onewire->GPIOx->BSRR = onewire->GPIO_Pin;				// Set el pin 1-Wire
}
/*
*********************************************************************************************************
*/
