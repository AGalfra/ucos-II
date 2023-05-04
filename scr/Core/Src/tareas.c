/*
 * ********************************************************************************************************
 *
 *  Este archivo contiene las definiciones de las tareas necesarias para la aplicacion de usuario.
 *
 * Filename:	tareas.c
 * Author:		Alejandro Galfrascoli
 *
 * En las siguientes tareas se hace uso de Eventos tipo Flag , Mbox y Semaforo para la sincronizacion de
 * tareas y asegurar el accceso exclusivo a recursos del microcontrolador.
 * Se hace uso del modulo Discreto de Entrada Salida DIO.
 *
 * ucos-II:		v2.92.16
 * Asignacion de pines:
 *
 *
 *********************************************************************************************************
 */

/*
*********************************************************************************************************
*											INCLUDE FILES
*********************************************************************************************************
*/
#include "includes.h"
/*
*********************************************************************************************************
*                                          	VARIABLES
*********************************************************************************************************
*/
extern BOOLEAN	Led;									// Para monitorear el estado del LED
extern INT8U	CtxPerSec;								// Cambios de contexto por segundo
extern char		*ErrStr;								// Str para debug de errores en eventos o funciones

extern GUI_MENU guiMenu;								// Estructuraa del menu
extern Ds18b20Sensor_t ds18b20[DS18B20_MAX_SENSORS];	// tabla de sensores
//------------------------------------------------------------------------------
uint8_t mjeCom0[] = 	"\r\n\n### Test por COM ###\r\n";
uint8_t mjeCom1[] = 	"[W]-> Button Up \r\n";
uint8_t mjeCom2[] = 	"[S]-> Button Down\r\n";
uint8_t mjeCom3[] = 	"[D]-> Button OK\r\n";
uint8_t mjeCom4[] = 	"[P]-> Test OLED\r\n";
uint8_t mjeCom5[] = 	"[T]-> Sensor Table\r\n";
uint8_t mjeCom6[] = 	"[H]-> Menu Options\r\n";
uint8_t mjeCom7[] = 	"[X]-> Exit\r\n";

uint8_t mjeComSalida[]= "\r\nFinish Test COM...";
uint8_t mjeComST[] = 	"\r\nSensorTable()";
uint8_t mjeComTO[] = 	"\r\nTestOLED()";
uint8_t mjeComOK[] = 	"\r\nOK";
uint8_t mjeComUP[] = 	"\r\nUP";
uint8_t mjeComDW[] = 	"\r\nDOWN";
//------------------------------------------------------------------------------
// textos para los menus								// 18 caracteres max
static char itmBack[] = "< Back";						// 14 caracteres max

static char itmRoot[] = "Root menu";					// 18 caracteres max
static char itmSubmenu1[] = "Escala";					// 14 caracteres max
static char itmSubmenu2[] = "Visualizacion";			// 14 caracteres max
static char itmSubmenu3[] = "Actualizacion";			// 14 caracteres max
static char itmSubmenu4[] = "Inf. Sistema";				// 14 caracteres max
static char itmSubmenu5[] = "Test por COM";				// 14 caracteres max

static char itmGC[] = "Celsius";						// 14 caracteres max
static char itmGF[] = "Fahrenheit";						// 14 caracteres max

static char itmT12[] = "T1 y T2";						// 14 caracteres max
static char itmT34[] = "T3 y T4";						// 14 caracteres max

static char itm1s[] = " 1 seg.";						// 14 caracteres max
static char itm3s[] = " 3 seg.";						// 14 caracteres max
static char itm5s[] = " 5 seg.";						// 14 caracteres max
static char itm10s[] = "10 seg.";						// 14 caracteres max
//--------------------------------------------------------------------
//menús: el primer elemento es el título del menú y no cuenta para cnt
//--------------------------------------------------------------------
static char* mnuRoot[] = 	  {	itmRoot,
						itmSubmenu1,itmSubmenu2,itmSubmenu3,itmSubmenu4,itmSubmenu5,itmBack};
uint8_t cntRoot = 6;

static char* mnuSubmenu1[] = {	itmSubmenu1,			// Escala
								itmGC,itmGF,itmBack};
uint8_t cntSubmenu1 = 3;

static char* mnuSubmenu2[] = {	itmSubmenu2,			// Visualizacion
								itmT12,itmT34,itmBack};
uint8_t cntSubmenu2 = 3;

static char* mnuSubmenu3[] = {	itmSubmenu3,			// Actualizacion
								itm1s,itm3s,itm5s,itm10s,itmBack};
uint8_t cntSubmenu3 = 5;

char* mnuSubmenu4[] = 		{	itmSubmenu4				// Info. Sistema
								};
uint8_t cntSubmenu4 = 0;

char* mnuSubmenu5[] = 		{	itmSubmenu5				// Test UART
								};
uint8_t cntSubmenu5 = 0;

/*
*********************************************************************************************************
*                                          StartupTask()
*
* Description : The startup task.  The uC/OS-II ticker should only be initialize once multitasking starts.
*
* 				Inicializa estadisticas (Si estan habilidas), Eventos, y ademas crea las restantes tareas
* 				con el llamado a App_TaskCreate().
* 				App_InitModules() inicializacion de Modulos.
*				App_EventCreate() Crea los eventos para las tareas.
*
* Argument(s) : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
* Return(s)   : none.
* Caller(s)   : This is a task.
* Note(s)     : Esta tarea se elimina asi misma una vez que se ejecuta
*********************************************************************************************************
*/
void StartupTask (void *p_arg)
{
 CPU_INT32U cpu_clk;
 (void)p_arg;

 cpu_clk = HAL_RCC_GetHCLKFreq();
 /* Initialize and enable System Tick timer */
 OS_CPU_SysTickInitFreq(cpu_clk);

 #if (OS_TASK_STAT_EN > 0)
  OSStatInit();                                  /* Determine CPU capacity. */
 #endif
//******************
 App_EventCreate();                            	/* Create application events. */
 App_InitModules();								/* Inicializacion de Modulos HAL. */
 App_TaskCreate();                              /* Create application tasks. */
//******************

 while (DEF_TRUE) {
	 OSTaskSuspend(APP_CFG_TASK1_PRIO);			/* Task 1 inicia suspendida, se habilita por menu */
	 OSTaskDel(APP_CFG_STARTUP_TASK_PRIO);		/* elimino al tarea de inicio una vez que se ejecutó */
 	 }
}

/*
*********************************************************************************************************
*                                          Task1()
*
* Description : Verificación de teclas presionadas. El caracter es recibido por la UART. las opciones
* 				disponibles son las siguientes:
* 				D	-> Select/OK
* 				W	-> UP
* 				S	-> DOWN
* 				P	-> Test OLED
* 				T	-> Sensor Table
* 				H	-> Help
* 				X	-> Exit
*
* Argument(s) : p_arg       Argument passed to 'Task1()' by 'OSTaskCreate()'.
* Return(s)   : none.
* Caller(s)   : This is a task.
* Note(s)     : Esta tarea se ejecuta cada 500ms para verificar si hay teclas presionadas.
* 				Esta tarea se encuentra suspendida hasta ser habilitada desde el menu de opciones.
* 				Tambien se suspende asi misma si es seleccionada la opcion 'X'.
*********************************************************************************************************
*/
void Task1 (void *p_arg) {
 (void)p_arg;
 INT8U err;							// para devolucion de errores
 INT8U D[1];						// longitud de caracteres recibidos
 OS_FLAGS flags_rdy;

 OS_STK_DATA stk_data_chk;			// para verificar el Stack
 INT32U stk_size_chk;
 INT8U err_chk;

 while(DEF_TRUE) {

	flags_rdy = OSFlagAccept (flag_grp,(OS_FLAGS ) COM, OS_FLAG_WAIT_SET_ALL | OS_FLAG_CONSUME, &err);
	if((flags_rdy & COM) == (COM)) {
		menuCom();
		COMGets(D, 1);				// limpio buffer
 	 }

	if(COMGets(D, 1) == HAL_OK) {
			D[1]=0;               						/* Pongo un "0" al final del vector		*/

			if(D[0]=='d' || D[0]=='D') {				/* Opcion Select/OK del Encoder */
				COMPuts(mjeComOK);
				OSFlagPost (flag_grp, (OS_FLAGS) OK, OS_FLAG_SET, &err);	/* seteo el bit OK de flag group */
			}

			if(D[0]=='w' || D[0]=='W') {				/* Opcion UP del Encoder */
				COMPuts(mjeComUP);
				OSFlagPost (flag_grp, (OS_FLAGS) UP, OS_FLAG_SET, &err);	/* seteo el bit UP de flag group */
			}

			if(D[0]=='s' || D[0]=='S') {				/* Opcion DOWN del Encoder */
				COMPuts(mjeComDW);
				OSFlagPost (flag_grp, (OS_FLAGS) DOWN, OS_FLAG_SET, &err);	/* seteo el bit DOWN de flag group */
			}

			if(D[0]=='p' || D[0]=='P') {				/* Opcion Test OLED */
				COMPuts(mjeComTO);
				TestOLED();								/* ejecuta prueba de la pantalla LCD */
				guiShowMenu();
			}

			if(D[0]=='t' || D[0]=='T') {				/* Opcion Sensor Table*/
				COMPuts(mjeComST);
				SensorTable();							/* visualiza la tabla de sensores actual */
			}

			if(D[0]=='h' || D[0]=='H') {				/* Opcion Help */
				menuCom();								/* despliega el menu de opciones */
			}

			if(D[0]=='x' || D[0]=='X') {				/* Opcion Exit */
				COMPuts(mjeComSalida);
				guiFinishMenu();
				OSTaskSuspend(APP_CFG_TASK1_PRIO);		/* Task1 se suspende asi misma, se habilita desde el menu */
			}

	}	// fin if(HAL_UART_Receive)


  	//-------------------------------------------------------------------------------
	err_chk = OSTaskStkChk(APP_CFG_TASK1_PRIO, &stk_data_chk);	// verificacion del stack
	if (err_chk == OS_ERR_NONE) {
		stk_size_chk = stk_data_chk.OSFree + stk_data_chk.OSUsed;
		stk_size_chk += 0;
	}
	//-------------------------------------------------------------------------------

	OSTimeDlyHMSM(0u, 0u, 0u, 500u);
 }
}

/*
*********************************************************************************************************
*                                          Task2()
*
* Description : Esta tarea es la encargada de manejar el encoder rotativo
*
* Argument(s) : p_arg       Argument passed to 'Task2()' by 'OSTaskCreate()'.
* Return(s)   : none.
* Caller(s)   : This is a task.
* Note(s)     : Esta tarea se ejecuta cada vez que se activan cualquiera de los sig. flags:
* 				ENCA: deteccion de flanco descendente en pin Encoder A.
* 				EBUT: deteccion de flanco descendente en pin Button.
*
* 				Los flags ENCA y EBUT son seteados en EdgeAFnct() y EdgeButFnct() que son las funciones
* 				callback de deteccion de flancos del modulo DIO
**********************************************************************************************************
*/
void Task2 (void *p_arg)
{
 (void)p_arg;
 INT8U err;
 OS_FLAGS flags_rdy;

 DICfgMode(0, DI_MODE_EDGE_LOW_GOING);					// deteccion de flancos descendentes en EncA
 DICfgEdgeDetectFnct(0, EdgeAFnct, (void *)flag_grp);	// EdgeAFnct: callback para señalizar los flag

 DICfgMode(1, DI_MODE_DIRECT);							// pin de EncB

 DICfgMode(3, DI_MODE_EDGE_LOW_GOING);					// pin de Button
 DICfgEdgeDetectFnct(3, EdgeButFnct, (void *)flag_grp);	// EdgeButFnct: callback para señalizar los flag
 DIClr(0);												// borro el num de transiciones

 while (DEF_TRUE)
 {
	 flags_rdy = OSFlagPend (flag_grp,(OS_FLAGS ) ENCA | EBUT, OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME, 0, &err);
	 if ((flags_rdy & ENCA) == (ENCA))  {
		 if(DIGet(1)) {
			 OSFlagPost (flag_grp, (OS_FLAGS) UP, OS_FLAG_SET, &err);	/* seteo el bit UP de flag group */
		 	 }
		 else {
			 OSFlagPost (flag_grp, (OS_FLAGS) DOWN, OS_FLAG_SET, &err);	/* seteo el bit DOWN de flag group */
		 	 }

		 DIClr(0);					// borro el num de transiciones
	 }

	 if ((flags_rdy & EBUT) == (EBUT))  {
		 OSFlagPost (flag_grp, (OS_FLAGS) OK, OS_FLAG_SET, &err);	/* seteo el bit OK de flag group */
	 	 }
/*
	  	//-------------------------------------------------------------------------------
		err_chk = OSTaskStkChk(APP_CFG_TASK2_PRIO, &stk_data_chk);	// verificacion del stack
		if (err_chk == OS_ERR_NONE) {
			stk_size_chk = stk_data_chk.OSFree + stk_data_chk.OSUsed;
		}
		//-------------------------------------------------------------------------------
*/
//	 OSTimeDlyHMSM(0u, 0u, 0u, 50u);
 }
}

/*
*********************************************************************************************************
*                                          Task3()
*
* Description :	Gestiona la presentacion del menu en la pantalla OLED
*
* Argument(s) : p_arg       Argument passed to 'Task3()' by 'OSTaskCreate()'.
* Return(s)   : none.
* Caller(s)   : This is a task.
* Note(s)     : Esta tarea se ejecuta cada vez que se activan cualquiera de los sig. flags, los cuales
* 				son consumidos en la llamada:
*					DOWN: giro en sentido horario del encoder.
*					UP:	  giro en sentido antihorario del encoder.
*					OK:	  pulsación del boton del encoder.
*
*				Esta tarea tambien envia MailBox a la Task4() con la configuracion de:
*				ModeSensor:		Es el modo de visualizacion de los sensores en lcd: 1, 2.
*				FrecSensor:		Es la frecuencia de actualizacion de los sensores: 3, 5 o 10 segundos.
*				EscaleSensor:	Escala de lectura Celsius (1) o Fahrenheit (2).
**********************************************************************************************************
*/
void Task3 (void *p_arg) {
 (void)p_arg;
 INT8U err;
 OS_FLAGS flags_rdy;
 uint8_t clickedItem;						// indice del item cliqueado
 uint8_t EscaleSensor = 1;					// Escala en grados Celsius
 uint8_t ModeSensor = 1;					// Modo 1 de visualizacion
 uint8_t FrecSensor = 3;					// Frecuencia de inicio: 3 segundos

 OS_STK_DATA stk_data_chk;					// para verificar el Stack en CubeMonitor
 INT32U stk_size_chk;
 INT8U err_chk;

 if(LOGO_EN) {
	 guiBitmap();							// visualizar logotipo de inicio
 }

 while (DEF_TRUE) {

	flags_rdy = OSFlagPend (flag_grp,(OS_FLAGS ) DOWN | UP | OK, OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME, 0, &err);

	if((flags_rdy & DOWN) == (DOWN))					/* por DOWN...					*/
		if(guiMenu.active)								/* si estoy dentro del menu 	*/
			guiPress(DOWN);								/* informo a guiPress() un down */

	if((flags_rdy & UP) == (UP))						// Por UP...
		if(guiMenu.active)								/* si estoy dentro del menu 	*/
			guiPress(UP);								/* informo a guiPress() un UP	*/
	if((flags_rdy & OK) == (OK)) {
	 	clickedItem = guiPress(OK);						/* si presiono ok lo informo a guiPress(), devuelve item seleccionado */
	 	if(!guiMenu.active) {							/* si estoy fuera del menu	 	*/
			guiInitMenu(mnuRoot, cntRoot, 1);			/* lo inicio en nivel Root 		*/
			clickedItem = 0;
			}
	}

	if(clickedItem > 0) {
		if(guiMenu.CurrentMenu == mnuRoot)							/* ### Root Menu ### */
			switch(clickedItem) {
		    	case 1: guiInitMenu(mnuSubmenu1, cntSubmenu1, 1);	/* cargo el submenu correspondiente */
		        		break;
		        case 2:	guiInitMenu(mnuSubmenu2, cntSubmenu2, 1);
		        		break;
		        case 3:	guiInitMenu(mnuSubmenu3, cntSubmenu3, 1);
		        		break;
		        case 4: guiStat();		        					/* mostrar estadisticas */
		        		break;
		        case 5: guiMessageBox("Iniciando UART");		    /* arrancar tarea para uart */
		        		OSFlagPost (flag_grp, (OS_FLAGS) COM, OS_FLAG_SET, &err);	/* seteo el bit COM de flag group */
		        		OSTaskResume(APP_CFG_TASK1_PRIO);			/* Reanudo la task1, para test Uart */
		        		break;
		        case 6:	guiFinishMenu();						// salir del menu
		        		break;
				}

		else if(guiMenu.CurrentMenu == mnuSubmenu1) {			/* ### Submenu 1: Escala ### */
			switch (clickedItem) {
	   			case 1:	guiMessageBox("Grados: *C");
	   					EscaleSensor = 1;
	   					break;
	   			case 2:	guiMessageBox("Grados: *F");
	   					EscaleSensor = 2;
	   				   	break;
	   			case 3:	guiInitMenu(mnuRoot, cntRoot, 1);		//back
	   					break;
				}
			err = OSMboxPost(EscaleMbox, (void *) &EscaleSensor);	// Config de Escala a task4
			if(err != OS_ERR_NONE)
				ErrStr = "Err OSMboxPost: EscaleMbox " + err;

			if(MENU_OUT_EN & (clickedItem != 3)) {
				OSTimeDly(MENU_OUT_AUTO);
				guiFinishMenu();					// salida del menu luego de seleccionar Escala
			}
		}

		else if(guiMenu.CurrentMenu == mnuSubmenu2)	{			/* ### Submenu 2: Visualizacion ### */
			switch (clickedItem) {
				case 1:	guiMessageBox("T1 y T2");
						ModeSensor = 1;
						break;
				case 2:	guiMessageBox("T3 y T4");
						ModeSensor = 2;
						break;
				case 3:	guiInitMenu(mnuRoot, cntRoot, 2);		// back
						break;
				}
			err = OSMboxPost(ModeMbox, (void *) &ModeSensor);	/* Config de Visualizacion a task4 */
			if(err != OS_ERR_NONE)
				ErrStr = "Err OSMboxPost: ModeMbox " + err;

			if(MENU_OUT_EN & (clickedItem != 3)) {
				OSTimeDly(MENU_OUT_AUTO);
				guiFinishMenu();						// salida del menu luego de seleccionar Modo
			}
		}

		else if(guiMenu.CurrentMenu == mnuSubmenu3)	{			/* ### Submenu 3: Actualizacion ### */
		   	switch (clickedItem) {
		   		case 1:	guiMessageBox("1 segundo");
		   				FrecSensor = 1;
		   				break;
		   		case 2:	guiMessageBox("3 segundos");
		   				FrecSensor = 3;
		   				break;
		   		case 3:	guiMessageBox("5 segundos");
		   				FrecSensor = 5;
		   				break;
		   		case 4:	guiMessageBox("10 segundos");
		   				FrecSensor = 10;
		   				break;
		   		case 5:	guiInitMenu(mnuRoot, cntRoot, 3);		// back
		   				break;
		   		}
		   	err = OSMboxPost(FrecMbox, (void *) &FrecSensor);	/* enviar configuracion a task4 */
		   	if(err != OS_ERR_NONE)
		   		ErrStr = "Err en OSMboxPost: " + err;

			if(MENU_OUT_EN & (clickedItem != 5)) {
				OSTimeDly(MENU_OUT_AUTO);
				guiFinishMenu();						// salida del menu luego de seleccionar Frecuencia
			}
		}
		//-------------
		clickedItem = 0;
		}	// Fin de if(clickedItem > 0)


  	//-------------------------------------------------------------------------------
	err_chk = OSTaskStkChk(APP_CFG_TASK3_PRIO, &stk_data_chk);	// verificacion del stack
	if (err_chk == OS_ERR_NONE) {
		stk_size_chk = stk_data_chk.OSFree + stk_data_chk.OSUsed;
		stk_size_chk += 0;
	}
	//-------------------------------------------------------------------------------

	// OSTimeDlyHMSM(0u, 0u, 0u, 50u);
 }
}

/*
*********************************************************************************************************
*                                          Task4()
*
* Description :	Verifica el 3 MailBox que contienen la configuracion seleccionada en el menu de opciones
* 				y ademeas realiza la lectura de los sensores 1-Wire.
*
* 			    EscaleMbox: contiene la frecuencia de actualizacion de los sensores.
* 			    ModeMbox: contiene el modo de visualizacion de los sensores.
* 			    FrecMbox: contiene la frecuencia de actualizacion.
*
* Argument(s) : p_arg       Argument passed to 'Task4()' by 'OSTaskCreate()'.
* Return(s)   : None.
* Caller(s)   : This is a task.
* Note(s)     : Esta tarea se ejecuta cada 'frecSensor' segundos (3, 5, o 10), Se inicializa en 3s.
* 				Solo se visualizan los resultados de temperatura si el menu no se encuentra activo.
*********************************************************************************************************
*/
void Task4 (void *p_arg) {
 (void)p_arg;
 uint8_t escaleSensor = 1;
 uint8_t modeSensor = 1;
 uint8_t frecSensor = 3;
 void *msg;

 OS_STK_DATA stk_data_chk;					// para verificar el Stack en CubeMonitor
 INT32U stk_size_chk;
 INT8U err_chk;

 while(DEF_TRUE)
 {
	OSTimeDlyHMSM(0u, 0u, frecSensor, 0u);
	//-------------------------
	msg = OSMboxAccept(EscaleMbox);			// si hay un msg, es el nuevo valor de escala
	if(msg != (void *)0)
		escaleSensor = *(uint8_t *) msg;
	//-------------------------
	msg = OSMboxAccept(ModeMbox);			/* si hay un msg, es el nuevo valor de visualizacion */
	if(msg != (void *)0)
		modeSensor = *(uint8_t *) msg;
	//-------------------------
	msg = OSMboxAccept(FrecMbox);			/* si hay un msg, es el nuevo valor de actualizacion */
	if(msg != (void *)0)
		frecSensor = *(uint8_t *) msg;
	//-------------------------

	DS18B20_Del(ds18b20);					/* limpiar la tabla de sensores */
	DS18B20_Init(0);						/* inicializar la tabla de sensores */
	DS18B20_StartAll();
	DS18B20_ReadAll();						/* lectura del ScrtchPad de todos los sensores */

	if(!guiMenu.active)						/* si el menu no esta activo visualizar... */
		guiTemp(escaleSensor, modeSensor);

	//-------------------------------------------------------------------------------
	err_chk = OSTaskStkChk(APP_CFG_TASK4_PRIO, &stk_data_chk);	// verificacion del stack
	if (err_chk == OS_ERR_NONE) {
		stk_size_chk = stk_data_chk.OSFree + stk_data_chk.OSUsed;
		stk_size_chk += 0;
	}
	//-------------------------------------------------------------------------------

 }
}

/*
*********************************************************************************************************
*                                          TaskStat()
*
* Description : Estadisticas del Sistema Operativo.
* Argument(s) : p_arg       Argument passed to 'TaskStat()' by 'OSTaskCreate()'.
* Return(s)   : none.
* Caller(s)   : This is a task.
* Note(s)     : Esta tarea se ejecuta cada 1seg para resetear la variable OSCtxSwCtr, por lo que
* 				su ultimo valor indica la cantidad de cambios de contexto por segundo y se guarda en
* 				CtxPerSec para su uso en impresion de estadisticas.
*********************************************************************************************************
*/
void TaskStat (void *p_arg) {
 (void)p_arg;

 while (DEF_TRUE) {
	CtxPerSec = OSCtxSwCtr;
	OSCtxSwCtr = 0;
	OSTimeDlyHMSM(0u, 0u, 1u, 0u);
 }
}

/*
*********************************************************************************************************
*                                          TaskBlink()
*
* Description : Led indicador de sistema "Vivo".
* Argument(s) : p_arg       Argument passed to 'TaskBlink()' by 'OSTaskCreate()'.
* Return(s)   : none.
* Caller(s)   : This is a task.
* Note(s)     : Esta tarea se ejecuta cada 1s para invertir el estado del Built in LED.
*********************************************************************************************************
*/
void TaskBlink (void *p_arg) {
 (void)p_arg;

 while (DEF_TRUE) {
	 HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	 Led = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);						/* variable Led para monitoreo */

	 OSTimeDlyHMSM(0u, 0u, 1u, 0u);
 }
}

/*
*********************************************************************************************************
*                                          App_TaskCreate()
*
* Description : llamada en StartupTask() para la creación de las restantes tareas de la App.
* Argument(s) : None.
* Return(s)   : None.
* Caller(s)   :
* Note(s)     : none.
*********************************************************************************************************
*/
void App_TaskCreate (void)
{

CPU_INT08U	os_err;

os_err = OSTaskCreateExt( (void (*)(void *))Task1,
	  	  	  	  	  	  (void			*) 0,
						  (OS_STK		*) &Task1Stk[APP_CFG_TASK1_STK_SIZE - 1],
						  (INT8U		 ) APP_CFG_TASK1_PRIO,
						  (INT16U		 ) APP_CFG_TASK1_PRIO,
						  (OS_STK		*) &Task1Stk[0],
						  (INT32U		 ) APP_CFG_TASK1_STK_SIZE,
						  (void			*) 0,
						  (INT16U		 ) (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0u)
	 OSTaskNameSet( APP_CFG_TASK1_PRIO, (INT8U *)"Task1", &os_err);
#endif
//***********************************************************
os_err = OSTaskCreateExt( (void (*)(void *))Task2,
	  	  	  	  	  	  (void			*) 0,
						  (OS_STK		*) &Task2Stk[APP_CFG_TASK2_STK_SIZE - 1],
						  (INT8U		 ) APP_CFG_TASK2_PRIO,
						  (INT16U		 ) APP_CFG_TASK2_PRIO,
						  (OS_STK		*) &Task2Stk[0],
						  (INT32U		 ) APP_CFG_TASK2_STK_SIZE,
						  (void			*) 0,
						  (INT16U		 ) (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0u)
	 OSTaskNameSet( APP_CFG_TASK2_PRIO, (INT8U *)"Task2", &os_err);
#endif
//******************************************************
os_err = OSTaskCreateExt( (void (*)(void *))Task3,
		  	  	  	  	  (void			*) 0,
						  (OS_STK		*) &Task3Stk[APP_CFG_TASK3_STK_SIZE - 1],
						  (INT8U		 ) APP_CFG_TASK3_PRIO,
						  (INT16U		 ) APP_CFG_TASK3_PRIO,
						  (OS_STK		*) &Task3Stk[0],
						  (INT32U		 ) APP_CFG_TASK3_STK_SIZE,
						  (void			*) 0,
						  (INT16U		 ) (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0u)
	OSTaskNameSet( APP_CFG_TASK3_PRIO, (INT8U *)"Task3", &os_err);
#endif
//******************************************************
os_err = OSTaskCreateExt( (void (*)(void *))Task4,
				  	   	  (void			*) 0,
						  (OS_STK		*) &Task4Stk[APP_CFG_TASK4_STK_SIZE - 1],
						  (INT8U		 ) APP_CFG_TASK4_PRIO,
						  (INT16U		 ) APP_CFG_TASK4_PRIO,
						  (OS_STK		*) &Task4Stk[0],
						  (INT32U		 ) APP_CFG_TASK4_STK_SIZE,
						  (void			*) 0,
						  (INT16U		 ) (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0u)
	OSTaskNameSet( APP_CFG_TASK4_PRIO, (INT8U *)"Task4", &os_err);
#endif
//***********************************************************
os_err = OSTaskCreateExt( (void (*)(void *))TaskBlink,
				  	   	  (void			*) 0,
						  (OS_STK		*) &TaskBlinkStk[APP_CFG_TASK_BLINK_STK_SIZE - 1],
						  (INT8U		 ) APP_CFG_TASK_BLINK_PRIO,
						  (INT16U		 ) APP_CFG_TASK_BLINK_PRIO,
						  (OS_STK		*) &TaskBlinkStk[0],
						  (INT32U		 ) APP_CFG_TASK_BLINK_STK_SIZE,
						  (void			*) 0,
						  (INT16U		 ) (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0u)
	OSTaskNameSet( APP_CFG_TASK_BLINK_PRIO, (INT8U *)"Task Blink", &os_err);
#endif
//***********************************************************
os_err = OSTaskCreateExt( (void (*)(void *))TaskStat,
  			  	  	  	  (void			*) 0,
						  (OS_STK		*) &TaskStatStk[APP_CFG_TASK_STAT_STK_SIZE - 1],
						  (INT8U		 ) APP_CFG_TASK_STAT_PRIO,
						  (INT16U		 ) APP_CFG_TASK_STAT_PRIO,
						  (OS_STK		*) &TaskStatStk[0],
						  (INT32U		 ) APP_CFG_TASK_STAT_STK_SIZE,
						  (void			*) 0,
						  (INT16U		 ) (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0u)
	OSTaskNameSet( APP_CFG_TASK_STAT_PRIO,(INT8U *)"TaskStat", &os_err);
#endif
//***********************************************************
}

/*
*********************************************************************************************************
*                                          App_EventCreate()
*
* Description : llamada en StartupTask() para la creación de los Eventos en la App.
* Argument(s) : None.
* Return(s)   : None.
* Caller(s)   :
* Note(s)     : none.
*********************************************************************************************************
*/
void App_EventCreate(void) {
	INT8U err;

	flag_grp = OSFlagCreate	((OS_FLAGS)	0, 			// el valor inicial del grupo de Flags de eventos
	             	 	 	 (INT8U *)	&err); 		// tipo de error

	if(flag_grp == (void *)0)						/* verifico la creacion del Flag */
		ErrStr = "Err en OSFlagCreate";

	#if OS_FLAG_NAME_EN > 0u
	OSFlagNameSet (flag_grp, (INT8U *)"DIO Flag", &err);
	#endif
	//------------------------------------
	EscaleMbox = OSMboxCreate((void *)NULL);		/* Mbox para modo de escala de los sensores */
	if(EscaleMbox == (void *)0)						/* verifico la creacion del Mbox */
		ErrStr = "Err en EscaleMbox Create";
	//------------------------------------
	ModeMbox = OSMboxCreate((void *)NULL);			/* Mbox para modo de Visualizacion de los sensores */
	if(ModeMbox == (void *)0)
		ErrStr = "Err en ModeMbox Create";
	//------------------------------------
	FrecMbox = OSMboxCreate((void *)NULL);			/* Mbox para frec de actualizacion de los sensores */
	if(FrecMbox == (void *)0)
		ErrStr = "Err en FrecMbox Create";
	//------------------------------------

}

/*
*********************************************************************************************************
*                                          App_InitModules()
*
* Description : llamada en StartupTask() para la inicializacion de Modulos HAL.
* Argument(s) : None.
* Return(s)   : None.
* Caller(s)   :
* Note(s)     : none.
*********************************************************************************************************
*/
void App_InitModules(void) {

	#if MODULE_DIO
		DIOInit();			/* Inicializacion del modulo DIO */
	#endif

	COMInit();				/*  Inicializacion del modulo de Comunicaciones UART */

	guiInit();				/* Inicializacion del modulo GUI con la pantalla OLED SSD1306 */

	DS18B20_Init(1);		/* InitFull = 1: Inicializacion del modulo de sensores 18B20 y bus 1-Wire */
}

/*
*********************************************************************************************************
*                                          EdgeAFnct()
*
* Description : Señala con ENCA cada vez que se detecta un flanco descendente en EncA.
* Argument(s) : El puntero al flag_grup.
* Return(s)   : None.
* Caller(s)   :
* Note(s)     : Es usada como callback en la deteccion de flancos de la DIOTask para poder señalar a la
* 				Task2 la ocurrencia de una transicion.
*********************************************************************************************************
*/
void EdgeAFnct(void *arg) {
	INT8U err;
	OSFlagPost (flag_grp, (OS_FLAGS) ENCA, OS_FLAG_SET, &err);	/* seteo el bit ENCA de flag group */
}

/*
 * *********************************************************************************************************
*                                          EdgeButFnct()
*
* Description : Señala con EBUT cada vez que se detecta un flanco descendente en boton del encoder.
* Argument(s) : El puntero al flag_grup.
* Return(s)   : None.
* Caller(s)   :
* Note(s)     : Es usada como callback en la deteccion de flancos de la DIOTask para poder señalar a la
* 				Task2 la ocurrencia de una transicion.
*********************************************************************************************************
*/
void EdgeButFnct(void *arg) {
	INT8U err;
	OSFlagPost (flag_grp, (OS_FLAGS) EBUT, OS_FLAG_SET, &err);	/* seteo el bit EBUT de flag group */
}

/*
*********************************************************************************************************
*/
void menuCom(void) {

	COMPuts(mjeCom0);
	COMPuts(mjeCom1);
	COMPuts(mjeCom2);
	COMPuts(mjeCom3);
	COMPuts(mjeCom4);
	COMPuts(mjeCom5);
	COMPuts(mjeCom6);
	COMPuts(mjeCom7);
}

/*
*********************************************************************************************************
*/
void TestOLED(void) {

#if DRAW_TRIANGLE_EN
	TestTriangles(1);
	OSTimeDly(800);
#endif

#if DRAW_CIRCLE_EN
	TestCircles(8,1);
	OSTimeDly(800);

	TestFilledCircles(8, 1);
	OSTimeDly(800);
#endif

	TestLines(1);
	OSTimeDly(500);
/*
	TestRectangles(1);
	OSTimeDly(500);
	TestFilledRectangles(1);
	OSTimeDly(500);
*/
}

/*
*********************************************************************************************************
*/
void SensorTable(void) {
	uint8_t i,j;
	char tmp[2];
	char ROM_tmp[17] = "";
	char p[3] = ". ";
	char c[7] = "temp: ";
	char t[5] = "";
	char cadena[35] = "\r\nSensores No Conectados";

	if (DS18B20_Quantity()) {
		for(i = 0; i < DS18B20_Quantity(); i++) {

			decToStr((char)i, tmp, 1);
			for(j = 0; j < 8; j++)
				hexToStr(ds18b20[i].Address[j], ROM_tmp + j*2, 2);

			memccpy(memccpy(cadena+2, tmp, '\0',3)-1, p, '\0',6);
			memccpy(memccpy(cadena+5, ROM_tmp, '\0',22)-1, p, '\0',24);
			decToStr(ds18b20[i].Temperature, t, 4);
			memccpy(memccpy(cadena+23, c, '\0',30)-1, t, '\0',35);
			COMPuts((uint8_t*)cadena);
		}
	}
	else
		COMPuts((uint8_t*)cadena);
}

/*
*********************************************************************************************************
*/
