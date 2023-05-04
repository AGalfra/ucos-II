/*
*						GUI - Graphical User Interface
*
*	Libreria para el control de menus y submenus.
*
*	Notas: para funcionar necesita del modulo driver ssd1306.c y las definiciones de fuentes fonts.c
*
*	Filename   : gui.c
*	Programmer : Alejandro Galfrascoli
*********************************************************************************************************
*                                        		LIBRARIES
*********************************************************************************************************
*/
#include "includes.h"
#include "bitmap.h"
/*
*********************************************************************************************************
*                                       		 VARIABLES
*********************************************************************************************************
*/
GUI_MENU guiMenu;
/*
*********************************************************************************************************
 *                                         		FUNCTIONS
*********************************************************************************************************
 * @brief  Inicializa la comunicación I2C con el lcd a traves del driver ssd1306.c
 * 		   Setea la estructura de menu y visualiza el logotipo de inicio.
 * @param  None
 * @retval None
 */
void guiInit(void) {
	SSD1306_Init();
	guiMenu.firstVisible = 1;
	guiMenu.itemCount = 0;
	guiMenu.selectedIndex = 0;
	guiMenu.active = 0;
	guiMenu.ending = 0;
	guiMenu.CurrentMenu = 0;
	guiMenu.init = 1;
}

/*
*********************************************************************************************************
 * @brief  Inicializa la estructura del menu con los parametros pasadaos como parametros
 * @param  page:		  puntero al puntero que contiene los string de los items del submenu actual
 * @param  itemCount:	  cantidad de elementos (items) del nivel de submenu
 * @param  selectedIndex: indice del item seleccionado
 * @retval None
 */
void guiInitMenu(char** page, uint8_t itemCount, uint8_t selectedIndex) {
//	CPU_SR   cpu_sr;

//	OS_ENTER_CRITICAL();
	guiMenu.CurrentMenu = page;
	guiMenu.itemCount = itemCount;
	guiMenu.selectedIndex = selectedIndex;
	guiMenu.init = 0;
	guiMenu.ending = 0;
	guiMenu.active = 1;
//	OS_EXIT_CRITICAL();

	guiShowMenu();
}

/*
*********************************************************************************************************
 * @brief  Muestra en el LCD el nivel actual del submenu con un formato especifico (fuente: Font_7x10)
 * @param  text: string a visualizar
 * @retval None
 */
void guiTitle(char text[]) {
	  SSD1306_GotoXY((64-strlen(text)*Font_7x10.FontWidth/2), 0);		// centrado del texto
	  SSD1306_Puts(text, &Font_7x10, SSD1306_COLOR_WHITE);				// imprimir la cadena
	  SSD1306_DrawLine(0, 11, 128, 11, SSD1306_COLOR_WHITE);			// linea separadora
	  SSD1306_UpdateScreen();											// actualizar pantalla
}

/*
*********************************************************************************************************
 * @brief  Muestra en el LCD un item con el formato de NO seleccionado, en el nivel de submenu actual
 * @param  index: indice del array de items del submenu 					(fuente: Font_9x16)
 * @param  text:  string con el texto a visualizar
 * @param  col:   color con el que se visuliza el texto
 * @retval None
 */
void guiItem(uint8_t index, char text[], SSD1306_COLOR_t col) {
	SSD1306_GotoXY(0, (13 + index*17));
	SSD1306_Puts(text, & Font_9x16, col);
	SSD1306_UpdateScreen();
}

/*
*********************************************************************************************************
 * @brief  Visualiza un item con el formato de SELECCIONADO, en el nivel de submenu actual
 * @param  index: indice del array de items del submenu 					(fuente: Font_9x16)
 * @param  text:  string con el texto a visualizar.
 * @param  col:   color con el que se visuliza el fondo, el texto uasará color invertido.
 * @retval None
 */
void guiItemSelected(uint8_t index, char text[], SSD1306_COLOR_t bg) {
	SSD1306_DrawFilledRectangle(0, (13 + index*17), 128, 16, bg);
	SSD1306_GotoXY(0, (13 + index*17));
	SSD1306_Puts(text, &Font_9x16, !bg);
	SSD1306_UpdateScreen();
}

/*
*********************************************************************************************************
 * @brief  Si la accion realizada en el encoder es UP, DOWN: actualiza el item seleccionado y refrezca el
 * 		   lcd o devuelve el indice seleccionado si se presiono OK.
 * @param  action:	acción realizada (DOWN, UP , OK) en el encoder rotativo.
 * @retval selectedIndex: el indice del item del submenu actual si la accion es OK o 0 si es UP, DOWN.
 */
uint8_t guiPress(uint8_t action) {

	if (action == DOWN)
		guiMenu.selectedIndex++;
	if (action == UP)
		guiMenu.selectedIndex--;

	if(guiMenu.selectedIndex > guiMenu.itemCount)			// controlo los limites para hacerlo ciclico
		guiMenu.selectedIndex = 1;
	if(guiMenu.selectedIndex < 1)
		guiMenu.selectedIndex = guiMenu.itemCount;

	if (action == OK)
		return guiMenu.selectedIndex;

	guiShowMenu();
	return 0;
}

/*
*********************************************************************************************************
 * @brief  Al salir del menu, se limpia la pantalla y señaliza la variable en la estructura.
 * @param  None
 * @retval None
 */
void guiFinishMenu(void) {
	SSD1306_Clear();
	guiMessageBox("Actualizando...");		// Muestro este mje hasta tener lectura de temp
	guiMenu.ending = 1;
	guiMenu.active = 0;
}

/*
*********************************************************************************************************
 * @brief  actualiza en el LCD el nivel de submenu actual (Titulo) y todos los items, resaltando el item
 * 		   seleccionado con inversion de color.
 * @param  None
 * @retval None
 */
void guiShowMenu(void){
	uint8_t p = 3;
	uint8_t i = 0;

	if(guiMenu.selectedIndex > guiMenu.firstVisible + 2)
			guiMenu.firstVisible = guiMenu.selectedIndex - 2;
	else if(guiMenu.selectedIndex < guiMenu.firstVisible)
			guiMenu.firstVisible = guiMenu.selectedIndex;

	SSD1306_Clear();													// limpiar pantalla;
	strcpy(guiMenu.tempBuffer, (char*) ((guiMenu.CurrentMenu[0])));		// mostrar titulo
	guiTitle(guiMenu.tempBuffer);

	if(p > (guiMenu.itemCount - guiMenu.firstVisible + 1))				// porque se muestran de a 3 item
	    p = guiMenu.itemCount - guiMenu.firstVisible + 1;

	for(i=0; i<p; i++) {
		strcpy(guiMenu.tempBuffer, (char*) ((guiMenu.CurrentMenu[i+ guiMenu.firstVisible])));
	    if((guiMenu.selectedIndex - guiMenu.firstVisible)== i)
	    	guiItemSelected(i, guiMenu.tempBuffer, 1);					// mostrar item seleccionado
	    else
	    	guiItem(i, guiMenu.tempBuffer, 1);							// mostrar los NO seleccionados
	}
}

/*
*********************************************************************************************************
 * @brief  Superpone en el centro de la pantalla un recuadro con el mensaje a visualizar.
 * @param  text: string del mensaje a mostrar
 * @retval None
 */
void guiMessageBox(char text[]) {

	uint8_t textWidth = strlen(text)*7;
	uint8_t x = 64-(textWidth/2);
	uint8_t y = 32;

	SSD1306_DrawFilledRectangle(x-11, y-11, textWidth+20, 10+17, 1);
	SSD1306_DrawFilledRectangle(x-5, y-5, textWidth+10, 10+7, 0);
	SSD1306_DrawRectangle(x-3, y-3, textWidth+6, 10+3, 1);
	SSD1306_GotoXY(x, y);
	SSD1306_Puts(text, &Font_7x10, 1);
	SSD1306_UpdateScreen();
}

/*
*********************************************************************************************************
 * @brief  Presenta en la pantalla LCD la informacion del sistema operativo, Uso del CPU en porcentaje,
 * 		   numero de tareas, cambios de contexto por segundo y la version de RTOS.
 * NOTA:   La informacion no se actualiza dinamicamente, solo es una captura al momento de la llamada..
 * @param  None
 * @retval None
 */
void guiStat(void) {
	char s[5] = "";
	char s1[8] = "v?.????";
	extern INT8U CtxPerSec;

	SSD1306_Clear();
	SSD1306_GotoXY(21, 0);
	SSD1306_Puts("Estadisticas", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_DrawLine(0, 11, 128, 11, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, 17);
	SSD1306_Puts("Uso CPU:    %", &Font_7x10, SSD1306_COLOR_WHITE);
	intToStr((uint16_t)OSCPUUsage, s, 2);
	SSD1306_GotoXY(70, 17);
	SSD1306_Puts(s, &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, 29);
	SSD1306_Puts("N Tareas:", &Font_7x10, SSD1306_COLOR_WHITE);
	intToStr((uint16_t)OSTaskCtr, s, 2);
	SSD1306_GotoXY(70, 29);
	SSD1306_Puts(s, &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, 41);
	SSD1306_Puts("Cntx/seg:", &Font_7x10, SSD1306_COLOR_WHITE);
	intToStr((uint16_t)CtxPerSec, s, 2);
	SSD1306_GotoXY(70, 41);
	SSD1306_Puts(s, &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_GotoXY(0, 53);
	SSD1306_Puts("ucos-II:", &Font_7x10, SSD1306_COLOR_WHITE);
	intToStr((uint16_t)(OSVersion()/10000), s1+1, 1);
	s1[2]='.';
	intToStr((uint16_t)(OSVersion()%10000), s1+3, 4);
	SSD1306_GotoXY(70, 53);
	SSD1306_Puts(s1, &Font_7x10, SSD1306_COLOR_WHITE);

	SSD1306_UpdateScreen();
}

/*
*********************************************************************************************************
 * @brief  Muestra en la pantalla lcd un logitpo.
 * @param  None
 * @retval None
 */
void guiBitmap(void) {
	SSD1306_DrawBitmap(0, 0, logo_temp, 128, 64, 1);
	SSD1306_UpdateScreen();
	OSTimeDly(3000);
}

/*
*********************************************************************************************************
 * @brief  Muestra en la pantalla los valores de temperatura segun los parametros de configuracion actual
 * @param  escala: Escala de temperatura
 * 				 - 1: Celsius
 * 				 - 2: Fahrenheit
 * @param  modo: Modo de visualizacion de los sensores.
 * 				 - 1: T1 y T2.
 * 				 - 2: T3 y T4.
 * 		   NOTA: si el sensor no esta presente en el modo elegido se visualiza "???".
 * @retval None
 */
void guiTemp(uint8_t escala, uint8_t modo) {
	uint8_t i;
	char title[7] = "Modo  ";
	char tempStr[4][10] = {"", "", "", ""};
	char gC[3] = "*C";
	char gF[3] = "*F";
	char nan[10] = "   ???   ";
	float temp;

	if(guiMenu.init) {
		guiMenu.init = 0;								// Si acaba de iniciar la App... limpio pantalla
		SSD1306_Clear();
	}
	if(guiMenu.ending) {
		guiMenu.ending = 0;								// Si acaba de salir del menu... limpio pantalla
		SSD1306_Clear();
	}
	intToStr(modo, title+5, 1);							// Visualizo el modo elegido
	guiTitle(title);
	//----------------------------------------------------------
	for(i=0; i<DS18B20_MAX_SENSORS; i++) {
		if(escala == 2) {					// Si esta en *F...
			if(DS18B20_GetTempF(i, &temp)) {
				ftostr(temp, tempStr[i], 3, 3);				// str de salida con relleno de 3 espacios,
				memccpy(tempStr[i]+7, gF, '\0',9);			// y precision de 3 decimales
			}
			else
				memcpy(tempStr[i], nan, 9);					// dato no valido "???"
		}
		else {								// de lo contrario *C
			if(DS18B20_GetTempC(i, &temp)) {
				ftostr(temp, tempStr[i], 3, 3);				// str de salida con relleno de 3 espacios,
				memccpy(tempStr[i]+7, gC, '\0',9);			// y precision de 3 decimales
			}
			else
				memcpy(tempStr[i], nan, 9);					// dato no valido "???"
		}
	}
	//----------------------------------------------------------
	if(modo == 1) {
		SSD1306_GotoXY(6, 26);
		SSD1306_Puts("T1:", &Font_7x10, SSD1306_COLOR_WHITE);
		SSD1306_GotoXY(30, 23);
		SSD1306_Puts(tempStr[0], &Font_9x16, SSD1306_COLOR_WHITE);

		SSD1306_GotoXY(6, 48);
		SSD1306_Puts("T2:", &Font_7x10, SSD1306_COLOR_WHITE);
		SSD1306_GotoXY(30, 45);
		SSD1306_Puts(tempStr[1], &Font_9x16, SSD1306_COLOR_WHITE);
	}
	//----------------------------------------------------------
	else if(modo == 2) {
		SSD1306_GotoXY(6, 26);
		SSD1306_Puts("T3:", &Font_7x10, SSD1306_COLOR_WHITE);
		SSD1306_GotoXY(30, 23);
		SSD1306_Puts(tempStr[2], &Font_9x16, SSD1306_COLOR_WHITE);

		SSD1306_GotoXY(6, 48);
		SSD1306_Puts("T4:", &Font_7x10, SSD1306_COLOR_WHITE);
		SSD1306_GotoXY(30, 45);
		SSD1306_Puts(tempStr[3], &Font_9x16, SSD1306_COLOR_WHITE);
	}
	//----------------------------------------------------------
	SSD1306_UpdateScreen();
}

/*
*********************************************************************************************************
*/
