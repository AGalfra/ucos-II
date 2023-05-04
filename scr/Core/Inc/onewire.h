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

#ifndef INC_ONEWIRE_H_
#define INC_ONEWIRE_H_

/*
*********************************************************************************************************
*                                        		LIBRARIES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        CONFIGURATION CONSTANTS
*********************************************************************************************************
*/
#define OW_PORT				GPIOA				// Puerto para el bus 1-Wire
#define OW_PIN				GPIO_PIN_15			// Pin para el bus 1-Wire
//------------------------------------------------------------------------------------------

#define CMD_RSCRATCHPAD		0xBE				// Comandos ROM para los dispositivos
#define CMD_WSCRATCHPAD		0x4E
#define CMD_CPYSCRATCHPAD	0x48
#define CMD_RECEEPROM		0xB8
#define CMD_RPWRSUPPLY		0xB4
#define CMD_SEARCHROM		0xF0
#define CMD_READROM			0x33
#define CMD_MATCHROM		0x55
#define CMD_SKIPROM			0xCC
/*
*********************************************************************************************************
*                                            DATA TYPES
*********************************************************************************************************
*/
typedef struct {
	GPIO_TypeDef* GPIOx;           				// GPIO Port para el bus One Wire
	uint16_t GPIO_Pin; 							// GPIO Pin del bus One Wire
    uint8_t LastDiscrepancy;					// utilizada para la busqueda de dispositivos
    uint8_t LastFamilyDiscrepancy;				// idem
    uint8_t LastDeviceFlag;						// idem
	uint8_t ROM_NO[8];							// 8-byte ROM address del ultimo dispositivo encontrado
} OneWire_t;

/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void	OW_Init(OneWire_t* onewire, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint8_t OW_Reset(OneWire_t* onewire);

void 	OW_Write_bit(OneWire_t* onewire, uint8_t bit);
uint8_t OW_Read_bit(OneWire_t* onewire);
void 	OW_Write(OneWire_t* onewire, uint8_t data);
uint8_t OW_Read(OneWire_t* onewire);

void 	OW_Reset_Search(OneWire_t* onewire);
uint8_t OW_Search(OneWire_t* onewire, uint8_t command);
uint8_t OW_First(OneWire_t* onewire);
uint8_t OW_Next(OneWire_t* onewire);

void 	OW_Select(OneWire_t* onewire, uint8_t* addr);
void 	OW_Skip(OneWire_t* onewire);
void 	OW_GetFullROM(OneWire_t* onewire, uint8_t* newAddr);

uint8_t OW_CRC8(uint8_t* addr, uint8_t len);
/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*                                           HARDWARE SPECIFIC
*********************************************************************************************************
*/
void 	OW_Config_Timer(void);
void 	OW_Delay(uint16_t us);
void 	OW_Set_Pin_Output(OneWire_t *onewire);
void 	OW_Set_Pin_Input(OneWire_t *onewire);
void 	OW_PinLow(OneWire_t *onewire);
void 	OW_PinHigh(OneWire_t *onewire);

/*
*********************************************************************************************************
*/
#endif /* INC_ONEWIRE_H_ */
