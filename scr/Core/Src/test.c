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

#define min(a, b) (((a) < (b)) ? (a) : (b))		// utilizada como funcion auxiliar para evitar negativos

/*
*********************************************************************************************************
*                                          		FUNCTIONS
*********************************************************************************************************
* @brief  Ejecuta una prueba de la pantalla utilizando lineas diagonales en toda la pantalla.
* @param  color: color utilizado.
* @retval NONE
*/
void TestLines(SSD1306_COLOR_t color) {
    uint16_t x1, y1, x2, y2;

	CPU_SR   cpu_sr;
	OS_ENTER_CRITICAL();

    SSD1306_Clear();

    x1 = y1 = 0;
    y2 = SSD1306_HEIGHT - 1;
    for(x2 = 0; x2 < SSD1306_WIDTH; x2 += 6) {
    	SSD1306_DrawLine(x1, y1, x2, y2, color);
    	OSTimeDly(10);
    	SSD1306_UpdateScreen();
    }
    x2 = SSD1306_WIDTH - 1;
    for(y2 = 0; y2 < SSD1306_HEIGHT; y2 += 6) {
    	SSD1306_DrawLine(x1, y1, x2, y2, color);
    	OSTimeDly(10);
    	SSD1306_UpdateScreen();
    }

    OSTimeDly(40);
    SSD1306_Clear();

    x1 = SSD1306_WIDTH - 1;
    y1 = 0;
    y2 = SSD1306_HEIGHT - 1;
    for (x2 = 0; x2 < SSD1306_WIDTH; x2 += 6) {
    	SSD1306_DrawLine(x1, y1, x2, y2, color);
    	SSD1306_UpdateScreen();
    	OSTimeDly(10);
    }
    x2 = 0;
    for (y2 = 0; y2 < SSD1306_HEIGHT; y2 += 6) {
    	SSD1306_DrawLine(x1, y1, x2, y2, color);
    	SSD1306_UpdateScreen();
    	OSTimeDly(10);
    }

    OSTimeDly(40);
    SSD1306_Clear();

    x1 = 0;
    y1 = SSD1306_HEIGHT - 1;
    y2 = 0;
    for (x2 = 0; x2 < SSD1306_WIDTH; x2 += 6) {
    	SSD1306_DrawLine(x1, y1, x2, y2, color);
    	OSTimeDly(5);
    	SSD1306_UpdateScreen();
    }
    x2 = SSD1306_WIDTH - 1;
    for (y2 = 0; y2 < SSD1306_HEIGHT; y2 += 6) {
    	SSD1306_DrawLine(x1, y1, x2, y2, color);
    	OSTimeDly(5);
    	SSD1306_UpdateScreen();
    }

    OSTimeDly(20);
    SSD1306_Clear();

    x1 = SSD1306_WIDTH - 1;
    y1 = SSD1306_HEIGHT - 1;
    y2 = 0;
    for (x2 = 0; x2 < SSD1306_WIDTH; x2 += 6) {
    	SSD1306_DrawLine(x1, y1, x2, y2, color);
    	OSTimeDly(5);
    	SSD1306_UpdateScreen();
    }
    x2 = 0;
    for (y2 = 0; y2 < SSD1306_HEIGHT; y2 += 6) {
    	SSD1306_DrawLine(x1, y1, x2, y2, color);
    	OSTimeDly(5);
    	SSD1306_UpdateScreen();
    }
    OS_EXIT_CRITICAL();
}

/*
*********************************************************************************************************
* @brief  Ejecuta una prueba de la pantalla utilizando rectangulos.
* @param  color: color utilizado.
* @retval NONE
*/
void TestRectangles(SSD1306_COLOR_t color) {
    uint16_t n, i, i2;

	CPU_SR   cpu_sr;
	OS_ENTER_CRITICAL();

    SSD1306_Clear();
    n = min(SSD1306_WIDTH, SSD1306_HEIGHT);
    for (i = 2; i < n; i += 6) {
		i2 = i / 2;
        SSD1306_DrawRectangle((SSD1306_WIDTH/2) - i2, (SSD1306_HEIGHT/2) - i2, i, i, color);
        SSD1306_UpdateScreen();
        OSTimeDly(10);
    }
    OS_EXIT_CRITICAL();
}

/*
*********************************************************************************************************
* @brief  Ejecuta una prueba de la pantalla utilizando rectangulos reelenos.
* @param  color: color utilizado.
* @retval NONE
*/
void TestFilledRectangles(SSD1306_COLOR_t color) {
    uint16_t n, i, i2, cx = SSD1306_WIDTH/2 - 1, cy = SSD1306_HEIGHT/2 - 1;

	CPU_SR   cpu_sr;
	OS_ENTER_CRITICAL();

    SSD1306_Clear();
    n = min(SSD1306_WIDTH, SSD1306_HEIGHT);
    for (i = n; i > 0; i -= 6) {
        i2 = i/2;
        SSD1306_DrawFilledRectangle(cx-i2, cy-i2, i, i, color);
        SSD1306_UpdateScreen();
        OSTimeDly(10);
    }
    OS_EXIT_CRITICAL();
}
/*
*********************************************************************************************************
* @brief  Ejecuta una prueba de la pantalla utilizando circulos.
* @param  radius: valor del radio en pixeles
* @param  color: color utilizado.
* @retval NONE
*/
#if DRAW_CIRCLE_EN
void TestCircles(uint16_t radius, SSD1306_COLOR_t color) {
	uint16_t x;
	uint16_t y;
	uint16_t r2 = radius * 2;
	uint16_t w = SSD1306_WIDTH + radius;
	uint16_t h = SSD1306_HEIGHT + radius;

	CPU_SR   cpu_sr;
	OS_ENTER_CRITICAL();

	SSD1306_Clear();
    for (x = radius; x < w; x += r2)
        for (y = radius; y < h; y += r2) {
            SSD1306_DrawCircle(x, y, radius, color);
            SSD1306_UpdateScreen();
            OSTimeDly(40);
        }
    OS_EXIT_CRITICAL();
}

/*
*********************************************************************************************************
* @brief  Ejecuta una prueba de la pantalla utilizando circulos rellenos.
* @param  radius: valor del radio en pixeles
* @param  color: color utilizado.
* @retval NONE
*/
void TestFilledCircles(uint16_t radius, SSD1306_COLOR_t color) {
    uint16_t x;
    uint16_t y;
    uint16_t w = SSD1306_WIDTH;
    uint16_t h = SSD1306_HEIGHT;
    uint16_t r2 = radius * 2;

	CPU_SR   cpu_sr;
	OS_ENTER_CRITICAL();

    SSD1306_Clear();
    for (x = radius; x < w; x += r2)
        for (y = radius; y < h; y += r2) {
            SSD1306_DrawFilledCircle(x, y, radius, color);
            SSD1306_UpdateScreen();
            OSTimeDly(40);
        }
    OS_EXIT_CRITICAL();
}
#endif

/*
*********************************************************************************************************
* @brief  Ejecuta una prueba de la pantalla utilizando triangulos.
* @param  radius: valor del radio en pixeles
* @param  color: color utilizado.
* @retval NONE
*/
#if DRAW_TRIANGLE_EN
void TestTriangles(SSD1306_COLOR_t color) {
    uint16_t n;
    uint16_t i;
    uint16_t cx = SSD1306_WIDTH/2 - 1;
    uint16_t cy = SSD1306_HEIGHT/2 - 1;

	CPU_SR   cpu_sr;
	OS_ENTER_CRITICAL();

    SSD1306_Clear();
    n = min(cx, cy);
    for (i = 0; i < n; i += 5) {
        SSD1306_DrawTriangle(cx , cy-i, cx-i, cy+i, cx+i, cy+i, color);
        SSD1306_UpdateScreen();
        OSTimeDly(80);
    }
    OS_EXIT_CRITICAL();
}
#endif

/*
*********************************************************************************************************
*/
