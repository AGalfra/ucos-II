/*
*********************************************************************************************************
*								Driver para pantalla OLED SSD1306 - via I2C
*
*									Adaptada para funcionar con ucos-II
*
*	SSD1306    |STM32F10x    |DESCRIPTION
*	-------------------------------------------
*	VCC        |3.3V         |
*	GND        |GND          |
*	SCL        |PB6          |Serial clock line
*	SDA        |PB7          |Serial data line
*
*
* Filename   : ssd1306.h
* Programmer : Alejandro Galfrascoli
*********************************************************************************************************
 */
#ifndef SSD1306_H
#define SSD1306_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif
/*
*********************************************************************************************************
*                                        		LIBRARIES
*********************************************************************************************************
*/
#include "stm32f1xx_hal.h"
#include "main.h"
#include "fonts.h"
#include "stdlib.h"
#include "string.h"
/*
*********************************************************************************************************
*                                        CONFIGURATION CONSTANTS
*********************************************************************************************************
*/
#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR		0x78			/* I2C address */
//#define SSD1306_I2C_ADDR		0x7A
#endif
												/* SSD1306 settings */
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH			128				/* SSD1306 width in pixels */
#endif

#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT			64				/* SSD1306 LCD height in pixels */
#endif

#ifndef ssd1306_I2C_TIMEOUT
#define ssd1306_I2C_TIMEOUT		20000
#endif


#define SSD1306_WRITECOMMAND(command) ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x00, (command)) /* Write command */
#define SSD1306_WRITEDATA(data)       ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x40, (data))	   /* Write data */
#define ABS(x)   					  ((x) > 0 ? (x) : -(x))							   /* Absolute value */

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define SSD1306_DEACTIVATE_SCROLL                    0x2E 	// Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F 	// Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 	// Set scroll range

#define SSD1306_NORMALDISPLAY       				 0xA6
#define SSD1306_INVERTDISPLAY       				 0xA7
/*
*********************************************************************************************************
*							CONFIGURATION CONSTANTS - CONDITIONAL COMPILATION
*********************************************************************************************************
*/
#define DRAW_TRIANGLE_EN			0					/* Enable code generation to draw triangle */
#define DRAW_CIRCLE_EN				1					/* Enable code generation to draw circle */
#define DRAW_BITMAP_EN				1					/* Enable code generation to draw bitmap */
#define DRAW_SCROLL_EN				0					/* Enable code generation to scroll for lcd */
/*
*********************************************************************************************************
*                                            DATA TYPES
*********************************************************************************************************
*/
typedef enum {
	SSD1306_COLOR_BLACK = 0x00, 							/* Black color, no pixel */
	SSD1306_COLOR_WHITE = 0x01 								/* Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;


typedef struct {											/* Private SSD1306 structure */
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

uint8_t SSD1306_Init(void);							/* Initializes SSD1306 LCD */
void SSD1306_UpdateScreen(void);					/* Updates buffer from internal RAM to LCD */
void SSD1306_ToggleInvert(void);					/* Toggles pixels invertion inside internal RAM */
void SSD1306_Fill(SSD1306_COLOR_t Color);			/* Fills entire LCD with desired color */
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);	/* Draws pixel at desired location */
void SSD1306_GotoXY(uint16_t x, uint16_t y);							/* Sets cursor pointer to desired location for strings */
char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color);		/* Puts character to internal RAM */
char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color);	/* Puts string to internal RAM */
void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c);	/* Draws line on LCD */
void SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);	/* Draws rectangle on LCD */
void SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);	/* Draws filled rectangle on LCD */

#if DRAW_TRIANGLE_EN
void SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);	/* Draws triangle on LCD */
void SSD1306_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);
#endif

#if DRAW_CIRCLE_EN
void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c); /* Draws circle to STM buffer */
void SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c); /* Draws filled circle to STM buffer */
#endif

#if DRAW_BITMAP_EN
void SSD1306_DrawBitmap(int16_t x, int16_t y, unsigned char* bitmap, int16_t w, int16_t h, uint16_t color); /* Draws the Bitmap */
#endif

#if DRAW_SCROLL_EN
void SSD1306_ScrollRight(uint8_t start_row, uint8_t end_row);	/* scroll the screen for fixed rows */
void SSD1306_ScrollLeft(uint8_t start_row, uint8_t end_row);
void SSD1306_Scrolldiagright(uint8_t start_row, uint8_t end_row);
void SSD1306_Scrolldiagleft(uint8_t start_row, uint8_t end_row);
#endif

void SSD1306_Stopscroll(void);

void SSD1306_InvertDisplay(uint8_t i);						/* inverts the display i = 1->inverted, i = 0->normal */
void SSD1306_Clear(void);									/* clear the display */

/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*                                           HARDWARE SPECIFIC
*********************************************************************************************************
*/

void ssd1306_I2C_Init(void);											/* Initializes SSD1306 LCD */
void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data);		/*  */
void ssd1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);

/*
*********************************************************************************************************
*/

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
