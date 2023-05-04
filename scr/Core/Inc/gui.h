/*
*									GUI - Graphical User Interface
*
*	Libreria para el control de menus y submenus.
*
*	Notas: para funcionar necesita del modulo driver ssd1306.c y las definiciones de fuentes fonts.c
* -------------------------------------------------------------------------------------------------------

*	Filename   : gui.h
*	Programmer : Alejandro Galfrascoli
* -------------------------------------------------------------------------------------------------------
*			LOS ELEMENTOS DE MENU DEBEN DEFINIRSE COMO SE MUESTRA A CONTINUACION (formato de muestra):
*
* // textos para los menus								// 18 caracteres max
* char itmBack[] = "< Back";							// 14 caracteres max
* char itmOn[] = "On";									// 14 caracteres max
* char itmOff[] = "Off";								// 14 caracteres max
*
* char itmRoot[] = "Menu Raiz";							// 18 caracteres max
* char itmSubmenu1[] = "Submenu 1";						// 14 caracteres max
* char itmSubmenu2[] = "Submenu 2";						// 14 caracteres max
* char itmSubmenu3[] = "Submenu 3";						// 14 caracteres max
* char itmMessageBox[] = "Cuadro de mensaje";			// 18 caracteres max
* char itmItem1[] = "Item 1";							// 14 caracteres max
* char itmItem2[] = "Item 2";							// 14 caracteres max
* char itmItem3[] = "Item 3";							// 14 caracteres max
* char itmItem4[] = "Item 4";							// 14 caracteres max
* char itmItem5[] = "Item 5";			 				// 14 caracteres max
*
* //menús: el primer elemento es el título del menú y no cuenta para cnt
* char* mnuRoot[] = 	{	itmRoot,
* 							itmSubmenu1,itmSubmenu2,itmSubmenu3,itmBack};
* uint8_t cntRoot = 4;
*
* char* mnuSubmenu1[] = {	itmSubmenu1,
* 							itmItem1,itmItem2,itmItem3,itmItem4,itmItem5,itmBack};
* uint8_t cntSubmenu1 = 6;
*
* char* mnuSubmenu2[] = {	itmSubmenu2,
* 							itmOn,itmOff,itmBack};
* uint8_t cntSubmenu2 = 3;
*
*********************************************************************************************************
*/
#ifndef INC_GUI_H_
#define INC_GUI_H_
/*
*********************************************************************************************************
*                                        		LIBRARIES
*********************************************************************************************************
*/
#include <stdio.h>
#include "fonts.h"
#include "ssd1306.h"
/*
*********************************************************************************************************
*                                        CONFIGURATION CONSTANTS
*********************************************************************************************************
*/
#define GUI_DefFontP	Font_7x10									/* tipos de fuente utilizadas */
#define GUI_DefFont		Font_9x16
/*
*********************************************************************************************************
*                                        		VARIABLES
*********************************************************************************************************
*/
typedef struct {						/* Estructura para el control del menu de opciones */
    uint8_t	selectedIndex;				/* Item seleccionado */
    uint8_t itemCount;					/* Cantidad de items del nivel actual */
    uint8_t firstVisible;				/* Indice del primer elemento visible en la pantalla */
    uint8_t active;						/* Menu activo */
    uint8_t init;						/* Para señalizar acciones por unica vez al arrancar app */
    uint8_t ending;						/* Para señalizar acciones por unica vez al finalizar el menu */
    char	tempBuffer[20];				/* Variable temporal */
	char** 	CurrentMenu;				/* Puntero al array que contiene las descrip. del nivel actual */
}GUI_MENU;

/*
*********************************************************************************************************
*                                          FUNCTIONS PROTOTYPES
*********************************************************************************************************
*/
void guiInit(void);
void guiInitMenu(char** page, uint8_t itemCount, uint8_t selectedIndex);
void guiTitle(char text[]);
void guiItem(uint8_t index, char text[], SSD1306_COLOR_t col);
void guiItemSelected(uint8_t index, char text[], SSD1306_COLOR_t bg);
uint8_t guiPress(uint8_t action);
void guiFinishMenu(void);
void guiShowMenu(void);
void guiMessageBox(char text[]);

void guiStat(void);
void guiBitmap(void);
void guiTemp(uint8_t escala, uint8_t modo);

/*
*********************************************************************************************************
*/

#endif /* INC_GUI_H_ */
