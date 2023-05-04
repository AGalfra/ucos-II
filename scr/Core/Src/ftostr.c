/*
*********************************************************************************************************
*	Este modulo de funciones convierte numeros enteros decimales, enteros hexadecimales y numeros de
*	punto flotante en cadenas de caracteres.
*
*	NOTA: 	La conversion se lleva a cabo sin recurrir a funciones estandard como 'sprintf',
*			con lo cual se ahorra entre 30-35% de espacio de memoria y tiempo de ejecucion.
*
*	Filename   : ftostr.h
*	Programmer : Alejandro Galfrascoli*
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

char relleno = ' ';								/* caracter de relleno para la parte entera del numero */

/*
*********************************************************************************************************
*                     	                     	FUNCTIONS
*********************************************************************************************************
* @brief  Invierte una cadena de caracteres dada.
* @param  str: puntero al string que se desea invertir.
* @param  len: longitud de la cadena de caracteres.
* @retval None
*/
void reverse(char* str, uint8_t len) {
	uint8_t i = 0;
	uint8_t j = len - 1;
	uint8_t temp;

	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

/*
*********************************************************************************************************
* @brief  Convierte un entero x dado en una cadena str[]. Si d es mayor que el número de dígitos de x,
* 		  se agregan caracteres de 'relleno' al principio.
*
* NOTA:   Usar esta funcion solo para la parte entera del numero y decToStr() para la parte decimal.
*
* @param  x: es el numero entero a convertir en string.
* @param  str: string donde se almacena el resultado.
* @param  d: es el número de dígitos requeridos en la salida.
* @retval i: la longitud del string
*/
uint8_t intToStr(uint16_t x, char str[], uint8_t d) {
	uint8_t i = 0;

	while(x) {
		str[i++] = (x % 10) + '0';		// si x=40, obtengo en str "04"
		x = x / 10;
	}

	while (i < d)			// Si la cantidad de dígitos requerida es mayor, agrego relleno al principio.
		str[i++] = relleno;

	reverse(str, i);		// invertir el string obtenido
	str[i] = '\0';
	return i;				// devuelvo la longitud del string
}

/*
*********************************************************************************************************
* @brief  Convierte un entero x dado en una cadena str[]. Si d es mayor que el número de dígitos de x,
* 		  se agregan ceros '0' al principio.
*
* NOTA:   Esta funcion es igual a intToStr() salvo que el caracter de relleno es '0'.
* 		  Usar esta funcion solo para la parte decimal del numero.
*
* @param  x: es el numero entero a convertir en string.
* @param  str: string donde se almacena el resultado.
* @param  d: es el número de dígitos requeridos en la salida.
* @retval i: la longitud del string.
*/
uint8_t decToStr(uint16_t x, char str[], uint8_t d) {
	uint8_t i = 0;

	while(x) {
		str[i++] = (x % 10) + '0';		// si x=40, obtengo en str "04"
		x = x / 10;
	}

	while (i < d)			// Si la cantidad de dígitos requerida es mayor, agrego ceros al principio.
		str[i++] = '0';

	reverse(str, i);		// invertir el string obtenido
	str[i] = '\0';
	return i;				// devuelvo la longitud del string
}

/*
*********************************************************************************************************
* @brief  Convierte un número de punto flotante (admite numeros negativos) en una cadena.
* @param  n: es el float convertir en string.
* @param  res: string donde se alamacena el resultado.
* @param  afterpoint: es la cantidad de dígitos decimales (precision).
* @retval None
*/
void ftostr(float n, char* res, uint8_t beforePoint, uint8_t afterPoint) {
	uint8_t ipart;
	uint8_t i, s = 0;
	float fpart;

	if(n < 0) {									// si es negativo...
		n *= -1;								// convierto a positivo
		s = 1;									// señalizo el signo negativo
	}
	ipart = (uint8_t)n;							// Extraer parte entera del float
	fpart = n - (float)ipart;					// Extraer parte de punto flotante

	i = intToStr(ipart, res, beforePoint);		// convertir parte entera en str, completar con beforePoint
	if(ipart == 0)
			res[beforePoint - 1] = '0';			// solo para cuando la parte entera es cero
	if(s)										// si es negativo...
		res[0] = '-';							// pongo el menos

	if (afterPoint != 0) {						// comprobar la opción de visualización después del punto.
		res[i] = '.'; 							// agrego punto
		fpart = fpart * pow(10, afterPoint);	// Obtener el num de digitos de la parte decimal
		decToStr((int)fpart, res+i+1, afterPoint);
	}
}

/*
*********************************************************************************************************
* @brief  Convierte un entero Hexadecimal x dado en una cadena str[]. Si d es mayor que el número de
* 		  dígitos de x, se agregan ceros '0' al principio.
*
* @param  x: es el numero HEX a convertir en string.
* @param  str: string donde se almacena el resultado.
* @param  d: es el número de dígitos requeridos en la salida.
* @retval i: la longitud del string.
*/
uint8_t hexToStr(uint8_t x, char str[], uint8_t d){
	uint8_t i = 0;
	uint8_t tmp;

	while(x) {
		tmp = (x % 16);
		if(tmp <= 9)
			str[i++]= tmp + '0';
		else
			str[i++]= tmp -10 + 'A';
		x = x / 16;
	}

	while (i < d)			// Si la cantidad de dígitos requerida es mayor, agrego ceros al principio.
		str[i++] = '0';

	reverse(str, i);		// invertir el string obtenido
	str[i] = '\0';
	return i;				// devuelvo la longitud del string
}


/*
*********************************************************************************************************
*/
