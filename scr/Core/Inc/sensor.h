/*
*********************************************************************************************************
*									Modulo para Sensores DS18B20, DS1820, DS18S20
*
* 			NOTA: Para funcionar requiere de la libreria para comunicacion 1-Wire (onewire.h)
*
* Filename   : sensor.h
* Programmer : Alejandro Galfrascoli
*********************************************************************************************************
*/

#ifndef INC_18B20_H_
#define INC_18B20_H_
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
#define DS18B20_MAX_SENSORS		    4				// Define Num max. de sensores en el Bus
#define DS18B20_Pin 				GPIO_PIN_15		// Define Pin a utilizar
#define DS18B20_GPIO_Port			GPIOA			// Define Puerto a utilizar

#define DS18B20_USE_CRC				0				// Habilita la compilacion de codigo para CRC
#define DS18B20_USE_SET_RESOLUTION	0				// Habilita la compilacion de codigo para GetResolution()
#define DS18B20_USE_GET_RESOLUTION	0				// Habilita la compilacion de codigo para SetResolution()
//----------------------------------------------
#define DS18B20_CMD_ALARMSEARCH		0xEC
#define DS18B20_CMD_CONVERTTEMP		0x44
// Resolution
#define DS18B20_STEP_12BIT			0.0625
#define DS18B20_STEP_11BIT			0.125
#define DS18B20_STEP_10BIT			0.25
#define DS18B20_STEP_9BIT			0.5
// Device resolution
#define TEMP_9_BIT  				0x1F 					//  9 bit
#define TEMP_10_BIT 				0x3F 					// 10 bit
#define TEMP_11_BIT	 				0x5F 					// 11 bit
#define TEMP_12_BIT 				0x7F 					// 12 bit
// IDs de modelos soportados
#define DS18S20MODEL 				0x10  					// Tambien para el DS1820
#define DS18B20MODEL				0x28  					// Tambien para el MAX31820
#define DS1822MODEL  				0x22
#define DS1825MODEL  				0x3B
// Scratchpad locations
#define TEMP_LSB       				0
#define TEMP_MSB        			1
#define HIGH_ALARM_TEMP 			2
#define LOW_ALARM_TEMP  			3
#define CONFIGURATION   			4
#define INTERNAL_BYTE   			5
#define COUNT_REMAIN    			6
#define COUNT_PER_C     			7
#define SCRATCHPAD_CRC  			8

#define DS18B20_RESOLUTION_R1		6 						// Resolucion bit R1
#define DS18B20_RESOLUTION_R0		5 						// Resolucion bit R0
/*
*********************************************************************************************************
*                                            DATA TYPES
*********************************************************************************************************
*/
typedef struct {
	uint8_t 	Address[8];						// direccion ROM del Dispositivo
	uint8_t		ScratchPad[9];					// ScratchPad del dispositivo
	int16_t		Temperature;					// Temperaura en formato raw 12bits
	uint8_t		ValidDataFlag;					// vale 1 Si la temperatura es valida
} Ds18b20Sensor_t;

typedef enum {
	DS18B20_Resolution_9bits = 9,
	DS18B20_Resolution_10bits = 10,
	DS18B20_Resolution_11bits = 11,
	DS18B20_Resolution_12bits = 12
} DS18B20_Resolution_t;

/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void	DS18B20_Init(uint8_t initFull);
void	DS18B20_Del(Ds18b20Sensor_t* sensors);
// Control
uint8_t DS18B20_Start(uint8_t num); 											// Iniciar la conversión de un sensor
void 	DS18B20_StartAll(void);													// Iniciar la conversión de todos los sensores
uint8_t	DS18B20_Read(uint8_t num, Ds18b20Sensor_t* sensor);						// Leer un sensor
void 	DS18B20_ReadAll(void);													// Leer todos los sensores conectados
uint8_t DS18B20_Is18xxx(uint8_t* ROM); 											// Compruebe si la ROM es de la familia DS18xxx
uint8_t DS18B20_AllDone(OneWire_t* onewire);									// Comprueba si se realizó la conversión de todos los sensores

#if DS18B20_USE_GET_RESOLUTION
uint8_t DS18B20_GetResolution(uint8_t num, Ds18b20Sensor_t* sensor);			// Obtener la resolución del sensor
#endif
#if DS18B20_USE_SET_RESOLUTION
uint8_t DS18B20_SetResolution(uint8_t num, DS18B20_Resolution_t resolution);	// Establecer la resolución del sensor
#endif

//	ROMs
void	DS18B20_GetROM(uint8_t num, uint8_t* ROM); 		// Obtener la ROM del sensor de id 'num'
void	DS18B20_WriteROM(uint8_t num, uint8_t* ROM); 	// Escribe una ROM en la posición de 'num' en la tabla de sensores
// Funciones de retorno
uint8_t DS18B20_Quantity(void);							// Devuelve la cantidad de sensores conectados
uint8_t	DS18B20_GetTempC(uint8_t num, float* res); 		// Obtener la temp. de un sensor en °C
uint8_t	DS18B20_GetTempF(uint8_t num, float* res); 		// Obtener la temp. de un sensoren °F

/*
*********************************************************************************************************
*/
#endif /* INC_18B20_H_ */
