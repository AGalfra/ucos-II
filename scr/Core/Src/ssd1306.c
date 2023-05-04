/*
*********************************************************************************************************
*								Driver para pantalla OLED SSD1306 - via I2C
*
*									Adapatada para funcionar con ucos-II
*
*	SSD1306    |STM32F10x    |DESCRIPTION
*	-------------------------------------------
*	VCC        |3.3V         |
*	GND        |GND          |
*	SCL        |PB6          |Serial clock line
*	SDA        |PB7          |Serial data line
*
*
* Filename   : ssd1306.c
* Programmer : Alejandro Galfrascoli
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
extern	I2C_HandleTypeDef	hi2c1;										/* Instancia del I2C1 */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];		/* SSD1306 data buffer */
static SSD1306_t SSD1306;												/* Private variable */
static OS_EVENT *OLEDSem;												/* Semaphore */
/*
*********************************************************************************************************
*                                          		FUNCTIONS
*********************************************************************************************************
* @brief  Initializes SSD1306 LCD
* @param  None
* @retval Initialization status:
*           - 0: LCD was not detected on I2C port
*           - 1: LCD initialized OK and ready to use
*/
uint8_t SSD1306_Init(void) {

	OLEDSem = OSSemCreate(1);								/* Create exclusive access semaphore */

	ssd1306_I2C_Init();										/* Init I2C */
															/* Check if LCD connected to I2C */

	if (HAL_I2C_IsDeviceReady(&hi2c1, SSD1306_I2C_ADDR, 1, 20000) != HAL_OK) {
		return 0;							/* Return false */
	}
									/* Init LCD */
	SSD1306_WRITECOMMAND(0xAE); 	//display off
	SSD1306_WRITECOMMAND(0x20);		//Set Memory Addressing Mode
	SSD1306_WRITECOMMAND(0x10); 	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	SSD1306_WRITECOMMAND(0xB0); 	//Set Page Start Address for Page Addressing Mode,0-7
	SSD1306_WRITECOMMAND(0xC8); 	//Set COM Output Scan Direction
	SSD1306_WRITECOMMAND(0x00); 	//---set low column address
	SSD1306_WRITECOMMAND(0x10); 	//---set high column address
	SSD1306_WRITECOMMAND(0x40); 	//--set start line address
	SSD1306_WRITECOMMAND(0x81); 	//--set contrast control register
	SSD1306_WRITECOMMAND(0xFF);
	SSD1306_WRITECOMMAND(0xA1); 	//--set segment re-map 0 to 127
	SSD1306_WRITECOMMAND(0xA6); 	//--set normal display
	SSD1306_WRITECOMMAND(0xA8); 	//--set multiplex ratio(1 to 64)
	SSD1306_WRITECOMMAND(0x3F); 	//
	SSD1306_WRITECOMMAND(0xA4);		//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	SSD1306_WRITECOMMAND(0xD3); 	//-set display offset
	SSD1306_WRITECOMMAND(0x00); 	//-not offset
	SSD1306_WRITECOMMAND(0xD5); 	//--set display clock divide ratio/oscillator frequency
	SSD1306_WRITECOMMAND(0xF0); 	//--set divide ratio
	SSD1306_WRITECOMMAND(0xD9); 	//--set pre-charge period
	SSD1306_WRITECOMMAND(0x22); 	//
	SSD1306_WRITECOMMAND(0xDA); 	//--set com pins hardware configuration
	SSD1306_WRITECOMMAND(0x12);
	SSD1306_WRITECOMMAND(0xDB); 	//--set vcomh
	SSD1306_WRITECOMMAND(0x20); 	//0x20,0.77xVcc
	SSD1306_WRITECOMMAND(0x8D); 	//--set DC-DC enable
	SSD1306_WRITECOMMAND(0x14); 	//
	SSD1306_WRITECOMMAND(0xAF); 	//--turn on SSD1306 panel

	SSD1306_WRITECOMMAND(SSD1306_DEACTIVATE_SCROLL);
	SSD1306_Fill(SSD1306_COLOR_BLACK);				/* Clear screen */
	SSD1306_UpdateScreen();							/* Update screen */

	SSD1306.CurrentX = 0;							/* Set default values */
	SSD1306.CurrentY = 0;
	SSD1306.Initialized = 1;						/* Initialized OK */
	
	return 1;										/* Return OK */
}
/*
*********************************************************************************************************
* @brief  Updates buffer from internal RAM to LCD
* @note   This function must be called each time you do some changes to LCD, to update buffer from RAM to LCD
* @param  None
* @retval None
*/
void SSD1306_UpdateScreen(void) {
	uint8_t m;
	uint8_t err;
	
	OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */

	for (m = 0; m < 8; m++) {
		SSD1306_WRITECOMMAND(0xB0 + m);
		SSD1306_WRITECOMMAND(0x00);
		SSD1306_WRITECOMMAND(0x10);
		ssd1306_I2C_WriteMulti(SSD1306_I2C_ADDR, 0x40, &SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
	}
	OSSemPost(OLEDSem);								/* Release access to LCD */
}
/*
*********************************************************************************************************
* @brief  Toggles pixels invertion inside internal RAM
* @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
* @param  None
* @retval None
*/
void SSD1306_ToggleInvert(void) {
	uint16_t i;
	uint8_t err;
	
	OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
	SSD1306.Inverted = !SSD1306.Inverted;			/* Toggle invert */
	for (i = 0; i < sizeof(SSD1306_Buffer); i++) {	/* Do memory toggle */
		SSD1306_Buffer[i] = ~SSD1306_Buffer[i];
	}
	OSSemPost(OLEDSem);								/* Release access to LCD */
}
/*
*********************************************************************************************************
* @brief  Fills entire LCD with desired color
* @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
* @param  Color: Color to be used for screen fill. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
* @retval None
*/
void SSD1306_Fill(SSD1306_COLOR_t color) {
	uint8_t err;

	OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
	memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer)); /* Set memory */
	OSSemPost(OLEDSem);								/* Release access to LCD */
}
/*
*********************************************************************************************************
 * @brief  Draws pixel at desired location
 * @note   @ref SSD1306_UpdateScreen() must called after that in order to see updated LCD screen
 * @param  x: X location. This parameter can be a value between 0 and SSD1306_WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SSD1306_HEIGHT - 1
 * @param  color: Color to be used for screen fill. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color) {

	if ( x >= SSD1306_WIDTH	|| y >= SSD1306_HEIGHT)
		return;															/* Error */

	if (SSD1306.Inverted) 												/* Check if pixels are inverted */
		color = (SSD1306_COLOR_t)!color;

	if (color == SSD1306_COLOR_WHITE)									/* Set color */
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	else
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
}
/*
*********************************************************************************************************
 * @brief  Sets cursor pointer to desired location for strings
 * @param  x: X location. This parameter can be a value between 0 and SSD1306_WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SSD1306_HEIGHT - 1
 * @retval None
 */
void SSD1306_GotoXY(uint16_t x, uint16_t y) {
	uint8_t err;

	OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
	SSD1306.CurrentX = x;							/* Set write pointers */
	SSD1306.CurrentY = y;
	OSSemPost(OLEDSem);								/* Release access to LCD */
}
/*
*********************************************************************************************************
 * @brief  Puts character to internal RAM
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  ch: Character to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval Character written
 */
char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color) {
	uint32_t i, b, j;

	if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight))
				return 0;			/* Error */

	for (i = 0; i < Font->FontHeight; i++) {					/* Go through font */
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000)
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t) color);
			else
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
		}
	}
	SSD1306.CurrentX += Font->FontWidth;		/* Increase pointer */
	return ch;									/* Return character written */
}
/*
*********************************************************************************************************
 * @brief  Puts string to internal RAM
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  *str: String to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval Zero on success or character value when function failed
 */
char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color) {

	uint8_t err;

	OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
	while (*str) {
		if (SSD1306_Putc(*str, Font, color) != *str) {			/* Write character by character */
			OSSemPost(OLEDSem);								/* Release access to LCD  if error*/
			return *str;										/* Return error */
		}
		str++;													/* Increase string pointer */
	}
	OSSemPost(OLEDSem);								/* Release access to LCD */
	return *str;						/* Everything OK, zero should be returned */
}
/*
*********************************************************************************************************
 * @brief  Draws line on LCD
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x0: Line X start point. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y0: Line Y start point. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  x1: Line X end point. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y1: Line Y end point. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c) {
	int16_t dx, dy, sx, sy, err1, e2, i, tmp;
	uint8_t err;


	if(x0 >= SSD1306_WIDTH)						/* Check for overflow */
		x0 = SSD1306_WIDTH - 1;
	if(x1 >= SSD1306_WIDTH)
		x1 = SSD1306_WIDTH - 1;
	if(y0 >= SSD1306_HEIGHT)
		y0 = SSD1306_HEIGHT - 1;
	if(y1 >= SSD1306_HEIGHT)
		y1 = SSD1306_HEIGHT - 1;
	
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err1 = ((dx > dy) ? dx : -dy) / 2;

	OSSemPend(OLEDSem, 0, &err);				/* Obtain exclusive access to the LCD */
	if(dx == 0) { 								/* Vertical line */
		if(y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		if(x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		for(i = y0; i <= y1; i++)
			SSD1306_DrawPixel(x0, i, c);
		OSSemPost(OLEDSem);						/* Release access to LCD  for dx==0*/
		return;									/* Return from function */
	}
	
	if(dy == 0) {								/* Horizontal line */
		if(y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		if(x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		for(i = x0; i <= x1; i++)
			SSD1306_DrawPixel(i, y0, c);
		OSSemPost(OLEDSem);							/* Release access to LCD for dy==0 */
		return;									/* Return from function */
	}
	
	while(1) {
		SSD1306_DrawPixel(x0, y0, c);
		if(x0 == x1 && y0 == y1)
			break;
		e2 = err1;
		if(e2 > -dx) {
			err1 -= dy;
			x0 += sx;
		} 
		if(e2 < dy) {
			err1 += dx;
			y0 += sy;
		} 
	}
	OSSemPost(OLEDSem);							/* Release access to LCD */
}
/*
*********************************************************************************************************
 * @brief  Draws rectangle on LCD
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: Top left X start point. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y: Top left Y start point. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  w: Rectangle width in units of pixels
 * @param  h: Rectangle height in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c) {
	
	if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) 			/* Check input parameters */
		return;												/* Return error */


	if((x + w) >= SSD1306_WIDTH)							/* Check width and height */
		w = SSD1306_WIDTH - x;
	if((y + h) >= SSD1306_HEIGHT)
		h = SSD1306_HEIGHT - y;
															/* Draw 4 lines */
	SSD1306_DrawLine(x, y, x + w, y, c);         			/* Top line */
	SSD1306_DrawLine(x, y + h, x + w, y + h, c); 			/* Bottom line */
	SSD1306_DrawLine(x, y, x, y + h, c);         			/* Left line */
	SSD1306_DrawLine(x + w, y, x + w, y + h, c); 			/* Right line */
}
/*
*********************************************************************************************************
 * @brief  Draws filled rectangle on LCD
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: Top left X start point. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y: Top left Y start point. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  w: Rectangle width in units of pixels
 * @param  h: Rectangle height in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c) {
	uint8_t i;
	
	if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) 			/* Check input parameters */
		return;												/* Return error */

	if((x + w) >= SSD1306_WIDTH) 							/* Check width and height */
		w = SSD1306_WIDTH - x;
	if((y + h) >= SSD1306_HEIGHT)
		h = SSD1306_HEIGHT - y;
	
	for (i = 0; i <= h; i++) 								/* Draw lines */
		SSD1306_DrawLine(x, y + i, x + w, y + i, c);
}
/*
*********************************************************************************************************
 * @brief  Draws triangle on LCD
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x1: First coordinate X location. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y1: First coordinate Y location. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  x2: Second coordinate X location. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y2: Second coordinate Y location. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  x3: Third coordinate X location. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y3: Third coordinate Y location. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
#if DRAW_TRIANGLE_EN
void SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color) {

	SSD1306_DrawLine(x1, y1, x2, y2, color);				/* Draw lines */
	SSD1306_DrawLine(x2, y2, x3, y3, color);
	SSD1306_DrawLine(x3, y3, x1, y1, color);
}
#endif
/*
*********************************************************************************************************
 * @brief  Draws filled triangle on LCD
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x1: First coordinate X location. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y1: First coordinate Y location. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  x2: Second coordinate X location. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y2: Second coordinate Y location. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  x3: Third coordinate X location. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y3: Third coordinate Y location. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
#if DRAW_TRIANGLE_EN
void SSD1306_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color) {
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;
	
	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if(x2 >= x1){ xinc1 = 1; xinc2 = 1; }
	else 		{ xinc1 = -1; xinc2 = -1; }

	if(y2 >= y1){ yinc1 = 1; yinc2 = 1; }
	else 		{ yinc1 = -1; yinc2 = -1; }

	if (deltax >= deltay) {
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	}
	else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		SSD1306_DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}
#endif
/*
*********************************************************************************************************
 * @brief  Draws circle to STM buffer
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
#if DRAW_CIRCLE_EN
void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;
	uint8_t err;

	OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */

    SSD1306_DrawPixel(x0, y0 + r, c);
    SSD1306_DrawPixel(x0, y0 - r, c);
    SSD1306_DrawPixel(x0 + r, y0, c);
    SSD1306_DrawPixel(x0 - r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        SSD1306_DrawPixel(x0 + x, y0 + y, c);
        SSD1306_DrawPixel(x0 - x, y0 + y, c);
        SSD1306_DrawPixel(x0 + x, y0 - y, c);
        SSD1306_DrawPixel(x0 - x, y0 - y, c);

        SSD1306_DrawPixel(x0 + y, y0 + x, c);
        SSD1306_DrawPixel(x0 - y, y0 + x, c);
        SSD1306_DrawPixel(x0 + y, y0 - x, c);
        SSD1306_DrawPixel(x0 - y, y0 - x, c);
    }
    OSSemPost(OLEDSem);								/* Release access to LCD */
}
#endif
/*
*********************************************************************************************************
 * @brief  Draws filled circle to STM buffer
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
#if DRAW_CIRCLE_EN
void SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;
	uint8_t err;

	OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
    SSD1306_DrawPixel(x0, y0 + r, c);
    SSD1306_DrawPixel(x0, y0 - r, c);
    SSD1306_DrawPixel(x0 + r, y0, c);
    SSD1306_DrawPixel(x0 - r, y0, c);
    SSD1306_DrawLine(x0 - r, y0, x0 + r, y0, c);
    OSSemPost(OLEDSem);								/* Release access to LCD */
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1306_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, c);
        SSD1306_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, c);

        SSD1306_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, c);
        SSD1306_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}
#endif
/*
*********************************************************************************************************
 * @brief  Draws the Bitmap
 * @param  X:  X location to start the Drawing
 * @param  Y:  Y location to start the Drawing
 * @param  *bitmap : Pointer to the bitmap
 * @param  W : width of the image
 * @param  H : Height of the image
 * @param  color : 1-> white/blue, 0-> black
 */
#if DRAW_BITMAP_EN
void SSD1306_DrawBitmap(int16_t x, int16_t y, unsigned char* bitmap, int16_t w, int16_t h, uint16_t color) {
    int16_t byteWidth = (w + 7) / 8; 						// Bitmap scanline pad = whole byte
    uint8_t byte = 0;
    uint8_t err;

    OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
    for(int16_t j=0; j<h; j++, y++) {
        for(int16_t i=0; i<w; i++) {
            if(i & 7)
            	byte <<= 1;
            else
            	byte = (*(unsigned char *)(&bitmap[j * byteWidth + i / 8]));

            if(byte & 0x80)
            	SSD1306_DrawPixel(x+i, y, color);
        }
    }
    OSSemPost(OLEDSem);								/* Release access to LCD */
}
#endif
/*
*********************************************************************************************************
 * @brief  scroll RIGHT the screen for fixed rows
 * @param  start_row: start page address [0x00 - 0x0F]
 * @param  end_row:   end page address   [0x00 - 0x0F]
 */
#if DRAW_SCROLL_EN
void SSD1306_ScrollRight(uint8_t start_row, uint8_t end_row) {
  uint8_t err;

  OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
  SSD1306_WRITECOMMAND(SSD1306_RIGHT_HORIZONTAL_SCROLL);  		// send 0x26
  SSD1306_WRITECOMMAND(0x00);  									// send dummy
  SSD1306_WRITECOMMAND(start_row);  							// start page address
  SSD1306_WRITECOMMAND(0x00);  									// time interval 5 frames
  SSD1306_WRITECOMMAND(end_row);  								// end page address
  SSD1306_WRITECOMMAND(0x00);
  SSD1306_WRITECOMMAND(0xFF);
  SSD1306_WRITECOMMAND(SSD1306_ACTIVATE_SCROLL); 				// start scroll
  OSSemPost(OLEDSem);								/* Release access to LCD */
}
/*
*********************************************************************************************************
 * @brief  scroll LEFT the screen for fixed rows
 * @param  start_row: start page address [0x00 - 0x0F]
 * @param  end_row:   end page address   [0x00 - 0x0F]
 */
void SSD1306_ScrollLeft(uint8_t start_row, uint8_t end_row) {
  uint8_tt err;

  OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
  SSD1306_WRITECOMMAND(SSD1306_LEFT_HORIZONTAL_SCROLL);			// send 0x26
  SSD1306_WRITECOMMAND(0x00);  									// send dummy
  SSD1306_WRITECOMMAND(start_row);  							// start page address
  SSD1306_WRITECOMMAND(0x00);  									// time interval 5 frames
  SSD1306_WRITECOMMAND(end_row);  								// end page address
  SSD1306_WRITECOMMAND(0x00);
  SSD1306_WRITECOMMAND(0xFF);
  SSD1306_WRITECOMMAND(SSD1306_ACTIVATE_SCROLL); 				// start scroll
  OSSemPost(OLEDSem);								/* Release access to LCD */
}
/*
*********************************************************************************************************
 * @brief  scroll DIAGONAL RIGHT the screen for fixed rows
 * @param  start_row: start page address [0x00 - 0x0F]
 * @param  end_row:   end page address   [0x00 - 0x0F]
 */
void SSD1306_Scrolldiagright(uint8_t start_row, uint8_t end_row) {
  uint8_tt err;

  OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
  SSD1306_WRITECOMMAND(SSD1306_SET_VERTICAL_SCROLL_AREA);  		// sect the area
  SSD1306_WRITECOMMAND(0x00);   								// write dummy
  SSD1306_WRITECOMMAND(SSD1306_HEIGHT);

  SSD1306_WRITECOMMAND(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  SSD1306_WRITECOMMAND(0x00);
  SSD1306_WRITECOMMAND(start_row);
  SSD1306_WRITECOMMAND(0x00);
  SSD1306_WRITECOMMAND(end_row);
  SSD1306_WRITECOMMAND(0x01);
  SSD1306_WRITECOMMAND(SSD1306_ACTIVATE_SCROLL);
  OSSemPost(OLEDSem);								/* Release access to LCD */
}
/*
*********************************************************************************************************
 * @brief  scroll DIAGONAL LEFT the screen for fixed rows
 * @param  start_row: start page address [0x00 - 0x0F]
 * @param  end_row:   end page address   [0x00 - 0x0F]
 */
void SSD1306_Scrolldiagleft(uint8_t start_row, uint8_t end_row) {
  uint8_tt err;

  OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */

  SSD1306_WRITECOMMAND(SSD1306_SET_VERTICAL_SCROLL_AREA);  		// sect the area
  SSD1306_WRITECOMMAND(0x00);   								// write dummy
  SSD1306_WRITECOMMAND(SSD1306_HEIGHT);

  SSD1306_WRITECOMMAND(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  SSD1306_WRITECOMMAND(0x00);
  SSD1306_WRITECOMMAND(start_row);
  SSD1306_WRITECOMMAND(0x00);
  SSD1306_WRITECOMMAND(end_row);
  SSD1306_WRITECOMMAND(0x01);
  SSD1306_WRITECOMMAND(SSD1306_ACTIVATE_SCROLL);
  OSSemPost(OLEDSem);								/* Release access to LCD */
}
#endif
/*
*********************************************************************************************************
 * @brief  Stop scroll
 * @param  NONE
 */
void SSD1306_Stopscroll(void) {
	uint8_t err;

	OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
	SSD1306_WRITECOMMAND(SSD1306_DEACTIVATE_SCROLL);
	OSSemPost(OLEDSem);								/* Release access to LCD */
}
/*
*********************************************************************************************************
 * @brief  inverts the display
 * @param  i: 1->inverted, i = 0->normal
 */
void SSD1306_InvertDisplay (uint8_t i) {
	uint8_t err;

	OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
	if(i)	SSD1306_WRITECOMMAND(SSD1306_INVERTDISPLAY);
	else	SSD1306_WRITECOMMAND(SSD1306_NORMALDISPLAY);
	OSSemPost(OLEDSem);								/* Release access to LCD */
}
/*
*********************************************************************************************************
 * @brief  clear the display
 * @param  NONE
 */
void SSD1306_Clear (void) {
	SSD1306_Fill (0);
    SSD1306_UpdateScreen();
}
/*
*********************************************************************************************************
 * @brief  On the display
 * @param  NONE
 */
void SSD1306_ON(void) {
	uint8_t err;

	OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
	SSD1306_WRITECOMMAND(0x8D);  
	SSD1306_WRITECOMMAND(0x14);  
	SSD1306_WRITECOMMAND(0xAF);  
	OSSemPost(OLEDSem);								/* Release access to LCD */
}
/*
*********************************************************************************************************
 * @brief  Off the display
 * @param  NONE
 */
void SSD1306_OFF(void) {
	uint8_t err;

	OSSemPend(OLEDSem, 0, &err);					/* Obtain exclusive access to the LCD */
	SSD1306_WRITECOMMAND(0x8D);  
	SSD1306_WRITECOMMAND(0x10);
	SSD1306_WRITECOMMAND(0xAE);  
	OSSemPost(OLEDSem);								/* Release access to LCD */
}

//*********************************************************************************************************
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//				  _____ ___   _____
//				 |_   _|__ \ / ____|
//				   | |    ) | |
//				   | |   / /| |
//				  _| |_ / /_| |____
//				 |_____|____|\_____|
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Initializes SSD1306 LCD
 * @param  None
 * @retval Initialization status:
 *           - 0: LCD was not detected on I2C port
 *           - > 0: LCD initialized OK and ready to use
 */
void ssd1306_I2C_Init() {

	hi2c1.Instance = I2C1;												/* I2C1 Initialization Function */
	hi2c1.Init.ClockSpeed = 400000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
	    Error_Handler();
	}

}
/*
*********************************************************************************************************
 * @brief  Writes single byte to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  data: data to be written
 * @retval None
 */
void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data) {
	uint8_t dt[2];
	dt[0] = reg;
	dt[1] = data;
	HAL_I2C_Master_Transmit(&hi2c1, address, dt, 2, 10);
}
/*
*********************************************************************************************************
 * @brief  Writes multi bytes to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  *data: pointer to data array to write it to slave
 * @param  count: how many bytes will be written
 * @retval None
 */
void ssd1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
uint8_t dt[256];
dt[0] = reg;
uint8_t i;
for(i = 0; i < count; i++)
dt[i+1] = data[i];
HAL_I2C_Master_Transmit(&hi2c1, address, dt, count+1, 10);
}
/*
*********************************************************************************************************
*/
