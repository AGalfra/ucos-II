/*
*********************************************************************************************************
*				Este modulo contiene declaraciones de funciones para graficar animaciones
*				del tipo geometricas utilizando la libreria ssd1306.h
*
*	NOTA	   : Utiliza las constantes de compilacion condicional de la libreria ssd1306.h
*				 DRAW_CIRCLE_EN y DRAW_TRIANGLE_EN para poder utilizar algunas funciones
*
*	Filename   : test.h
*	Programmer : Alejandro Galfrascoli
*********************************************************************************************************
*/

#ifndef INC_TEST_H_
#define INC_TEST_H_

/*
*********************************************************************************************************
*                                        		LIBRARIES
*********************************************************************************************************
*/

#include "stdint.h"

/*
*********************************************************************************************************
*                                          FUNCTIONS PROTOTYPES
*********************************************************************************************************
*/

void TestLines(SSD1306_COLOR_t color);
void TestRectangles(SSD1306_COLOR_t color);
void TestFilledRectangles(SSD1306_COLOR_t color);

#if DRAW_CIRCLE_EN
void TestCircles(uint16_t radius, SSD1306_COLOR_t color);
void TestFilledCircles(uint16_t radius, SSD1306_COLOR_t color);
#endif

#if DRAW_TRIANGLE_EN
void TestTriangles(SSD1306_COLOR_t color);
#endif

/*
*********************************************************************************************************
*/

#endif /* INC_TEST_H_ */
