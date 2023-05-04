/*
*********************************************************************************************************
*	Este modulo de funciones convierte numeros enteros decimales, enteros hexadecimales y numeros de
*	punto flotante en cadenas de caracteres.
*
*	NOTA: 	La conversion se lleva a cabo sin recurrir a funciones estandard como 'sprintf',
*			con lo cual se ahorra entre 30-35% de espacio de memoria y tiempo de ejecucion.
*
*	Filename   : ftostr.h
*	Programmer : Alejandro Galfrascoli
*********************************************************************************************************
*/
#ifndef INC_FTOSTR_H_
#define INC_FTOSTR_H_
/*
*********************************************************************************************************
*											FUNCTIONS PROTOTYPES
*********************************************************************************************************
*/

void reverse(char* str, uint8_t len);
uint8_t intToStr(uint16_t x, char str[], uint8_t d);
uint8_t decToStr(uint16_t x, char str[], uint8_t d);
void ftostr(float n, char* res, uint8_t beforePoint, uint8_t afterPoint);
uint8_t hexToStr(uint8_t x, char str[], uint8_t d);

/*
*********************************************************************************************************
*/
#endif /* INC_FTOSTR_H_ */
