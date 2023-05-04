/*
 ********************************************************************************
 *
 * includes.h
 *
 *  Created on: May 06, 2021
 *      Author: Alejandro Galfrascoli
 ********************************************************************************
 */
#ifndef INCLUDES_H
#define INCLUDES_H
/*
 ********************************************************************************
 *									CONSTANTS
 ********************************************************************************
 */
#define MODULE_DIO		1u		/* Habilitacion del Modulo DIO */

#define BASE_TIEMPO		50		/* 50ms base de tiempo para las secuencias */
/*
 ********************************************************************************
 *								STANDARD LIBRARIES
 ********************************************************************************
 */
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#include "lib_str.h"
/*
 ********************************************************************************
 *								uC/OS Header Files
 ********************************************************************************
 */
#include "cpu_core.h"
#include "os.h"
#include "ucos_ii.h"
#include "os_cfg.h"
#include "app_cfg.h"
#include "cpu_cfg.h"   /* Needed for the Kernel&Non-Kernel aware Cortex-M port */
#include "os_cpu.h"
/*
 ********************************************************************************
 *							Building Blocks Header Files
 ********************************************************************************
 */
#if	MODULE_DIO
	#include "DIO.h"
#endif
/*
 ********************************************************************************
 *									Header Files
 ********************************************************************************
 */
#include "main.h"
#include "tareas.h"
//#include "usbd_cdc_if.h"

#include "ssd1306.h"
#include "fonts.h"
#include "gui.h"
#include "test.h"

#include "onewire.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_uart.h"
#include "sensor.h"
#include "ftostr.h"
#include "COM.h"

/*
********************************************************************************
 */
#endif		/* INCLUDES_H */
