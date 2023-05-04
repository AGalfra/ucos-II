/*
 * ********************************************************************************************************
 *
 *  Este archivo contiene las definiciones de las tareas necesarias para la aplicacion de usuario
 *
 * Filename:	tareas.h
 * Author:		Alejandro Galfrascoli
 *********************************************************************************************************
 */
#ifndef TAREAS_H
#define TAREAS_H

/*
*********************************************************************************************************
*                                    CONFIGURATION CONSTANTS
*********************************************************************************************************
*/
#define	MENU_OUT_EN			1			// Habilitacion de salida del menu al seleccionar una opcion
#define	MENU_OUT_AUTO		1000		// Tiempo de salida automatica del menu en mili segundos
#define	LOGO_EN				1			// Habilitacion del logotipo de inicio
/*
*********************************************************************************************************
*											PROTOTYPES
*********************************************************************************************************
*/
void StartupTask (void *p_arg);			/* Crea el resto de las tareas de App y eventos */
void Task1 (void *p_arg);				/* Realiza el test de hardware por puerto serial */
void Task2 (void *p_arg);				/* Gestiona el encoder rotativo */
void Task3 (void *p_arg);				/* Gestiona el menu de opciones con gui */
void Task4 (void *p_arg);				/* presentacion de datos de los sensores */

void TaskBlink (void *p_arg);			/* Blink Built-in Led */
void TaskStat (void *p_arg);			/* Estadisticas del sistema */

void App_TaskCreate (void);				/* Creacion de las tareas de la App */
void App_EventCreate(void);				/* Creacion de los eventos de la App */
void App_InitModules(void);				/* Inicializacion de Modulos HAL    */

void EdgeAFnct(void *arg);				/* Callback para la deteccion de flancos en EncA */
void EdgeButFnct(void *arg);			/* Callback para la deteccion de flancos en button*/
void menuCom(void);						/* Despliega las opciones de menu por COM */
void TestOLED(void);					/* Grafica animaciones del tipo geometricas */
void SensorTable(void);					/* Despliega la tabla de sensores actual */

/*
*********************************************************************************************************
*										DEFINICION DE STACKs
*********************************************************************************************************
*/

OS_STK StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE];
OS_STK Task1Stk[APP_CFG_TASK1_STK_SIZE];
OS_STK Task2Stk[APP_CFG_TASK2_STK_SIZE];
OS_STK Task3Stk[APP_CFG_TASK3_STK_SIZE];
OS_STK Task4Stk[APP_CFG_TASK4_STK_SIZE];
OS_STK TaskBlinkStk[APP_CFG_TASK_BLINK_STK_SIZE];
OS_STK TaskStatStk[APP_CFG_TASK_STAT_STK_SIZE];

/*
*********************************************************************************************************
*										DEFINICIONES PARA EVENTOS
*********************************************************************************************************
*/
// FLAG para control de estados (del encoder y menu)
OS_FLAG_GRP		*flag_grp; 		// Declaro puntero a estructura  tipo Event Flag

/* MASCARAS PARA EL CONTROL DE EVENTO FLAG */
#define ENCA	0x01			// flanco descendete - Encoder señal A
#define EBUT	0x02			// flanco descendete - Encoder Button
#define DOWN	0x04			// accion DOWN
#define UP		0x08			// accion UP
#define OK		0x10			// accion OK (Enter/ok/select)
#define COM		0x20			// indica el acceso al test por COM

// Mail Box para enviar la escala de los sensores
OS_EVENT 		*EscaleMbox;

// Mail Box para enviar el modo de visualizacion de los sensores
OS_EVENT 		*ModeMbox;

// Mail Box para enviar la frecuencia de actualizacion de los sensores
OS_EVENT 		*FrecMbox;

/*
********************************************************************************
*/

#endif  /* TAREAS_H */
