/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*                    You can find our product's documentation at: doc.micrium.com
*
*                          For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                            EXAMPLE CODE
*
* Filename : app_cfg.h
*********************************************************************************************************
*/
#ifndef  _APP_CFG_H_
#define  _APP_CFG_H_
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  "cpu_cfg.h"                     /* Needed for the Kernel&Non-Kernel aware Cortex-M port */

/*
*********************************************************************************************************
*                                           TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_CFG_STARTUP_TASK_PRIO         20u				/* Tarea para inicializar al resto	*/
#define  APP_CFG_TASK1_PRIO			       14u				/* Lectura del Teclado cada 500ms   */
#define  APP_CFG_TASK2_PRIO			        5u				/* Manejo del Encoder Rotativo */
#define  APP_CFG_TASK3_PRIO			        7u				/* Gestion del menu de opciones */
#define  APP_CFG_TASK4_PRIO			        9u				/* GUI y lectura de sensores */
#define  APP_CFG_TASK_STAT_PRIO		       11u				/* estadisticas  del sistema operativo */
#define  APP_CFG_TASK_BLINK_PRIO	       19u				/* Blink Led Built-In cada 1s */

#define  OS_TASK_TMR_PRIO                  (OS_LOWEST_PRIO - 2u)

/*
*********************************************************************************************************
*                                          TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/
														/* Stk medidos con OSTaskStkChk() */
#define  APP_CFG_STARTUP_TASK_STK_SIZE    	280u		/* stk_data_chk.OSUsed:  51 */
#define  APP_CFG_TASK1_STK_SIZE				160u		/* stk_data_chk.OSUsed: 160 */
#define  APP_CFG_TASK2_STK_SIZE				 80u		/* stk_data_chk.OSUsed:  47 */
#define  APP_CFG_TASK3_STK_SIZE				256u		/* stk_data_chk.OSUsed: 156 */
#define  APP_CFG_TASK4_STK_SIZE				196u		/* stk_data_chk.OSUsed: 160 */
#define  APP_CFG_TASK_STAT_STK_SIZE			 60u		/* stk_data_chk.OSUsed: 40 */
#define  APP_CFG_TASK_BLINK_STK_SIZE		 48u		/* stk_data_chk.OSUsed: 38 */

/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                    0u
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO                   1u
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                    2u
#endif

#define  APP_TRACE_LEVEL                   TRACE_LEVEL_OFF
#define  APP_TRACE                         printf

#define  APP_TRACE_INFO(x)    ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)     ((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_TRACE x) : (void)0)

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/
#endif                                                          /* End of module include.              */
