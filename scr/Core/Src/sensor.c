/*
*********************************************************************************************************
*									Modulo para Sensores DS18B20, DS1820, DS18S20
*
* 			NOTA: Para funcionar requiere de la libreria para comunicacion 1-Wire (onewire.h)
*
* Filename   : sensor.c
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
*                                       		 VARIABLES
*********************************************************************************************************
*/

Ds18b20Sensor_t	ds18b20[DS18B20_MAX_SENSORS];		// Tabla de sensores
OneWire_t		OneWire;							// Instancia de la variable para el control del Bus
uint8_t			TempSensorCount = 0;				// cantidad de sensores detectados

/*
*********************************************************************************************************
*		                                          FUNCTION
*********************************************************************************************************
* @brief  Inicializa la tabla de sensores segun el resultados de la busqueda realizada.
* 		  NOTA: inicia el bus 1-Wire y lee la ROM de los sensores conectados
* @param  initFull: Si es 1 realiza la secuencia de inicializacion delbus 1-Wire.
* @retval NONE
*/
void DS18B20_Init(uint8_t initFull) {
	uint8_t next = 0, i = 0;

	if(initFull)
		OW_Init(&OneWire, DS18B20_GPIO_Port, DS18B20_Pin); 			// initFull = 1 -> Inicializar 1-Wire

	next = OW_First(&OneWire); 										// buscar el primer dispositivo
	while(next) {
		TempSensorCount++;
		OW_GetFullROM(&OneWire, (uint8_t*)&ds18b20[i++].Address);	// obtener la ROM del sensor
		next = OW_Next(&OneWire);									// buscar si hay mas dispositivos...
		if(TempSensorCount >= DS18B20_MAX_SENSORS)
			break;
	}
}

/*
*********************************************************************************************************
* @brief  Borra la tabla de sensores. Permite reenumerar los dispositivos nuevamente
* @param  Ds18b20Sensor_t*: puntero a la tabla de sensores
* @retval NONE
*/
void	DS18B20_Del(Ds18b20Sensor_t* sensors) {
	uint8_t i, j;

	for (i = 0; i < DS18B20_MAX_SENSORS; i++) {
		for (j = 0; j < 8; j++)
			sensors[i].Address[j] = 0;
		sensors[i].Temperature = 0;
		sensors[i].ValidDataFlag = 0;
	}
	TempSensorCount = 0;
}

/*
*********************************************************************************************************
* @brief  Inicia la conversion de temperatura para el sensor de indice num en la tabla de sensores.
* @param  num: el indice del sensor
* @retval 1: si se ejecutó el comando correctamente
* 		  0: si el indice es mayor que la cantidad de sensores o si no el dispositivo correspondiente no
* 		     es de la familia ds18xxx
*/
uint8_t DS18B20_Start(uint8_t num) {
	if( num >= TempSensorCount)								// compruebo que el indice es valido
		return 0;
	if (!DS18B20_Is18xxx((uint8_t*)&ds18b20[num].Address)) 	// Compruebo si el sensor es de la flia DS18B20
		return 0;

	OW_Reset(&OneWire); 									// Resetear el bus
	OW_Select(&OneWire, (uint8_t*)ds18b20[num].Address);	// Selecciono el sensor por su ROM
	OW_Write(&OneWire, DS18B20_CMD_CONVERTTEMP); 			// inicio Conversion
	return 1;
}

/*
*********************************************************************************************************
* @brief  Inicia la conversión de temperatura para todos los sensores
* @param  NONE
* @retval NONE
*/
void DS18B20_StartAll(void) {
	OW_Reset(&OneWire); 								// Resetear el bus
	OW_Write(&OneWire, CMD_SKIPROM); 					// comando Skip ROM
	OW_Write(&OneWire, DS18B20_CMD_CONVERTTEMP); 		// iniciar conversion
}

/*
*********************************************************************************************************
* @brief  Realiza la lectura del sensor y almacena la temperatura en la tabla de sensores.
* 		  NOTA: La temperatura es almacenada en formato raw 12bits.
* 		  NOTA: Se debe verificar el final de conversion antes de usar esta funcion.
* @param  num: indice del sensor
* @param  Ds18b20Sensor_t*: puntero a la tabla de sensores
* @retval devuelve 1 si el valor de la temperatura es valida o 0 si se produce un error
*/
uint8_t DS18B20_Read(uint8_t num, Ds18b20Sensor_t* sensor) {
	int16_t temp;
	uint8_t i = 0;
#if DS18B20_USE_CRC
	uint8_t crc;
#endif
	CPU_SR   cpu_sr;

	if( num >= TempSensorCount) 							// compruebo que el indice es valido
		return 0;
	if (!DS18B20_Is18xxx(sensor[num].Address)) 				// Compruebo si el sensor es de la flia DS18xxx
		return 0;

	OS_ENTER_CRITICAL();

	OW_Reset(&OneWire);										// Resetear el bus
	OW_Select(&OneWire, sensor[num].Address);				// Seleccionar el sensor por ROM
	OW_Write(&OneWire, CMD_RSCRATCHPAD); 					//  comando: Read Scratchpad
	for (i = 0; i < 9; i++) 								// Leer scratchpad
		sensor[num].ScratchPad[i] = OW_Read(&OneWire);

#if DS18B20_USE_CRC
	crc = OW_CRC8(data, 8); 				// Calcular CRC
	if (crc != &ds18b20[num].ScratchPad[8])
		return 0; 							// CRC invalido
#endif

	//OW_Reset(&OneWire);										// ver la necesidad de esta linea

	temp = (sensor[num].ScratchPad[0] << 3) | (sensor[num].ScratchPad[1] << 11);	// para modelo 18B20

	if(sensor[num].Address[0] == DS18S20MODEL) {			// para los modelos de 9 bits fijos... calculo
		temp = ((temp & 0xfff0) << 3) - 32					// segun hoja de datos la extension a 12bits
				+ (((sensor[num].ScratchPad[COUNT_PER_C] - sensor[num].ScratchPad[COUNT_REMAIN]) << 7)
				/ sensor[num].ScratchPad[COUNT_PER_C]);
	}
	sensor[num].Temperature = temp;							// temperatura en formato raw de 12bits
	OS_EXIT_CRITICAL();
	return 1; 												// temperatura valida
}

/*
*********************************************************************************************************
* @brief  Realiza la lectura de todos los sensores en la tabla de sensores.
* @param  NONE
* @retval NONE
*/
void DS18B20_ReadAll(void) {
	uint8_t i;

	if(DS18B20_AllDone(&OneWire)) {										// veifico final de conversion
		for(i = 0; i < TempSensorCount; i++) {
			ds18b20[i].ValidDataFlag = 0;
			ds18b20[i].ValidDataFlag = DS18B20_Read(i, ds18b20); 		// Leer cada sensor
		}
	}
}

/*
*********************************************************************************************************
* @brief  Verifica el primer byte de la Rom para saber si corresponde con el codigo de familia
* 		  de los dispositivos 18xxx soportados.
* @param  *ROM:	es un puntero a la ROM de dispositivo que se desea verificar.
* @retval    0: si NO es un modelo soportado.
* 			 1: SI es un modelo soportado.
*/

uint8_t DS18B20_Is18xxx(uint8_t* ROM) {
	switch(*ROM) {
		case DS18S20MODEL:
		case DS18B20MODEL:
		case DS1822MODEL:
		case DS1825MODEL:
				return 1;
		default:
				return 0;
		}
}

/*
*********************************************************************************************************
* @brief  Verifica el bus 1-Wire par saber si esta libre (indica final de conversion de los sensores).
* 		  NOTA: realiza una espera max de 800us (tiempo de convesrion del sensor en 12bits de resolucion)
* @param  OneWire_t*: es un puntero a la estructura del bus 1-Wire.
* @retval 0: si esta ocupado - 1: si esta liberado
*/
uint8_t DS18B20_AllDone(OneWire_t* onewire) {
/*	uint8_t retries = 9;
	do {											// espera hasta 800us a que el bus esté alto
		if(--retries == 0)
			return 0;
		OW_Delay(100);
	} while( !HAL_GPIO_ReadPin(onewire->GPIOx, onewire->GPIO_Pin));
*/
	OSTimeDly(1);
	return 1;
}

/*
*********************************************************************************************************
* @brief  Obtiene el valor de la resolucion del dispositivo de indice num en la tabla de sensores.
* 		  NOTA: Si corresponde a la familia DS18S20MODEL devuelve 12.
* @param  num: indice del dispositivo
* @retval el valor de resolucion o 0 si hay error
*/
#if DS18B20_USE_GET_RESOLUTION
uint8_t DS18B20_GetResolution(uint8_t num, Ds18b20Sensor_t* sensor) {

	if( num >= TempSensorCount)								// compruebo que el indice es valido
		return 0;
	if(!DS18B20_Is18xxx((uint8_t*)sensor[num].Address)) 	// Compruebo si el sensor es de la flia DS18B20
		return 0;
	if(sensor[num].Address[0] == DS18S20MODEL)				// modelos no configurable
		return 12;

	switch(sensor[num].ScratchPad[4]) {						// Si es un DS18B20...
		case TEMP_12_BIT:
			return 12;
		case TEMP_11_BIT:
			return 11;
		case TEMP_10_BIT:
			return 10;
		case TEMP_9_BIT:
			return 9;
		default:
			return 12;
		}
}
#endif
/*
*********************************************************************************************************
* @brief  Configura la resolucion del dispositivo de indice num
* @param  num: 			indice del dispositivo
* @param  resolution:	valor de resolucion
* @retval 1 Si se ejecuta correctamente o 0 si hay error
*/
#if DS18B20_USE_SET_RESOLUTION
uint8_t DS18B20_SetResolution(uint8_t num, DS18B20_Resolution_t resolution) {
	if( num >= TempSensorCount)								// compruebo que el indice es valido
		return 0;
	uint8_t th, tl, res;
	if (!DS18B20_Is18xxx((uint8_t*)&ds18b20[num].Address))	// Compruebo si el sensor es de la flia DS18B20
		return 0;

	OW_Reset(&OneWire); 									// Resetear el bus
	OW_Select(&OneWire, (uint8_t*)&ds18b20[num].Address);	// Selecciono el sensor por ROM
	OW_Write(&OneWire, CMD_RSCRATCHPAD); 					// comando: Read scratchpad

	OW_Read(&OneWire);
	OW_Read(&OneWire);

	th = OW_Read(&OneWire); 	// Writing to scratchpad begins from the temperature alarms bytes
	tl = OW_Read(&OneWire); 	// 	so i have to store them.
	res = OW_Read(&OneWire);	// Config byte

	if (resolution == DS18B20_Resolution_9bits)	{			// x00x xxxx
		res &= ~(1 << DS18B20_RESOLUTION_R1);
		res &= ~(1 << DS18B20_RESOLUTION_R0);
	}
	else if (resolution == DS18B20_Resolution_10bits) {		// x01x xxxx
		res &= ~(1 << DS18B20_RESOLUTION_R1);
		res |= 1 << DS18B20_RESOLUTION_R0;
	}
	else if (resolution == DS18B20_Resolution_11bits) {		// x10x xxxx
		res |= 1 << DS18B20_RESOLUTION_R1;
		res &= ~(1 << DS18B20_RESOLUTION_R0);
	}
	else if (resolution == DS18B20_Resolution_12bits) {		// x11x xxxx
		res |= 1 << DS18B20_RESOLUTION_R1;
		res |= 1 << DS18B20_RESOLUTION_R0;
	}

	OW_Reset(&OneWire); 									// Resetear el bus
	OW_Select(&OneWire, (uint8_t*)&ds18b20[num].Address);
	OW_Write(&OneWire, CMD_WSCRATCHPAD); 					// Comando: Write Scratchpad

	OW_Write(&OneWire, th);
	OW_Write(&OneWire, tl);
	OW_Write(&OneWire, res);

	OW_Reset(&OneWire); 									// Resetear el bus
	OW_Select(&OneWire, (uint8_t*)&ds18b20[num].Address);
	OW_Write(&OneWire,CMD_CPYSCRATCHPAD); 					// Copiar Scratchpad a la EEPROM

	return 1;
}
#endif
/*
*********************************************************************************************************
* @brief  Devuelve el valor de la ROM (Address) del sensor de indice num en la tabla de sensores.
* @param  num: indice de la tabla de sensores
* @param  *ROM: puntero para devolver el codigo de ROM del sensor
* @retval NONE
*/
void DS18B20_GetROM(uint8_t num, uint8_t* ROM) {
	uint8_t i;

	if( num >= TempSensorCount)					// si el indice es mayor que TempSensorCount...
		num = TempSensorCount;					// leo el ultimo
	for(i = 0; i < 8; i++)
		ROM[i] = ds18b20[num].Address[i];
}

/*
*********************************************************************************************************
* @brief  Escribe un valor de ROM (Address) a la tabla de sensores
* @param  num: indice de la estructura de sensores a escribir
* @param  *ROM: puntero al valor de rom que se desea utilizar
* @retval NONE
*/
void DS18B20_WriteROM(uint8_t num, uint8_t* ROM) {
	uint8_t i;

	if( num >= TempSensorCount)
		return;
	for(i = 0; i < 8; i++)
		ds18b20[num].Address[i] = ROM[i];
}

/*
*********************************************************************************************************
* @brief  Devuelve la cantidad de sensores detectados en la busqueda. Limitado por DS18B20_MAX_SENSORS
* @param  NONE
* @retval TempSensorCount: cantidad de sensores
*/
uint8_t DS18B20_Quantity(void) {
	return TempSensorCount;
}

/*
*********************************************************************************************************
* @brief  Devuelve la temperatura en grados Celcuis del sensor de indice num en la tabla de sensores,
* 		  si ésta es valida.
* @param  num: indice del sensor
* @param  *res: puntero donde se guardará el valor de temperatura
* @retval 0: si la temperatura no es valida y 1 si el dato almacenado es correcto
*/
uint8_t DS18B20_GetTempC(uint8_t num, float* res) {
	if(!ds18b20[num].ValidDataFlag)
		return 0;
	*res = ds18b20[num].Temperature * 0.0078125;				// temp°C = temp_raw * 1/128
	return 1;
}

/*
*********************************************************************************************************
* @brief  Devuelve la temperatura en grados Fahrenheit del sensor de indice num en la tabla de sensores,
* 		  si ésta es valida.
* @param  num: indice del sensor.
* @param  *res: puntero donde se guardará el valor de temperatura.
* @retval 0 si la temperatura no es valida y 1 si el dato almacenado es correcto.
*/
uint8_t DS18B20_GetTempF(uint8_t num, float* res) {					// °F = °C * 9/5 + 32
	if(!ds18b20[num].ValidDataFlag)
		return 0;
	*res = ds18b20[num].Temperature * 0.0140625 + 32;				// 0.0078125 * 9/5 = 0.0140625
	return 1;
}

/*
*********************************************************************************************************
*/
