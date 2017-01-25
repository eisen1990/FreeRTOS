/*
    FreeRTOS V7.0.1 - Copyright (C) 2011 Real Time Engineers Ltd.


	FreeRTOS supports many tools and architectures. V7.0.0 is sponsored by:
	Atollic AB - Atollic provides professional embedded systems development
	tools for C/C++ development, code analysis and test automation.
	See http://www.atollic.com


    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/* Standard includes. */
//#include <stdio.h>
#include <string.h>

///////////////////////////////////////
#include <gtimer.h>
///////////////////////////////////////

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Demo Includes. */
/*----------------------------------------------------------------------------*/

/* Delay between cycles of the 'check' task. */
#define mainCHECK_DELAY						( ( portTickType ) 5000 / portTICK_RATE_MS )

/* Demo task priorities. */
#define mainQUEUE_POLL_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define mainCHECK_TASK_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define mainSEM_TEST_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainBLOCK_Q_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define mainGEN_Q_PRIORITY			( tskIDLE_PRIORITY )

/* Misc. */
#define mainQUEUE_SIZE				( 3 )
#define mainNO_DELAY				( ( portTickType ) 0 )
#define mainPRINT_PORT				( 0 )
/*----------------------------------------------------------------------------*/


/* Periodically checks to see whether the demo tasks are still running. */
static void em_Check( void *pvParameters );
static void em_Check2( void *pvParameters );
static void em_Check3( void *pvParameters );
static void vCheckTask( void *pvParameters );
/*----------------------------------------------------------------------------*/

/*
 * Configure the clocks, GPIO and other peripherals as required by the demo.
 */
static void prvSetupHardware( void );
/*----------------------------------------------------------------------------*/

int main( void )
{
	int i = 0;
	char str[64];
	em_printf("1. start entry point\n");
	//printf("org print\n",str);
	/* Initialise the Hardware. */
	//prvSetupHardware();
	em_printf("2. prvSetupHardware complete\n");
//	* Start the tasks defined within the file. 
	xTaskCreate( em_Check, (const signed char *)"Check", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
	xTaskCreate( em_Check2, (const signed char *)"Check2", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );

	em_printf("13. xTaskCreate\n");
    em_printf("[%s]: %d\r\n", __func__, __LINE__ );
    //vSerialPutString(configUART_PORT, str, strlen(str) );
	
	vTaskStartScheduler();
	
	em_printf("14. vTaskStartScheduler\n");
	while(1);
	/* Should never reach here. */
	//vSerialPutString(mainPRINT_PORT, (const signed char * const)"Should never reach here!\r\n", 26 );

	/* Will only get here if there was not enough heap space to create the idle task. */
	return 0;
}
/*----------------------------------------------------------------------------*/

void vApplicationTickHook( void )
{
}

/*----------------------------------------------------------------------------*/


void vApplicationIdleHook( void )
{
}

/*----------------------------------------------------------------------------*/
static void em_Check( void *pvParameters )
{
	int i = 0;
	for(i = 0;;i++)
	{
		if(i % 1000000 == 0)
		{
			em_printf("hello arndale\n");
			//vTaskDelay(2);
			//taskYIELD();
		}
	}
}
static void em_Check2( void *pvParameters )
{
	int i = 0;
	for(i = 0;;i++)
	{
		if(i % 1000000 == 0)
		{
			em_printf("Goodbye arndale\n");
			//vTaskDelay(10);
			//taskYIELD();
		}
	}
}
static void em_Check3( void *pvParameters )
{
	int i = 0;
	for(i = 0;;i++)
	{
		if(i % 1000000 == 0)
		em_printf("Again arndale\n");
	}
}
static void vCheckTask( void *pvParameters )
{
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
unsigned long ulVector = 0UL;
    char str[64];

	
	portDISABLE_INTERRUPTS();

	// Install the Spurious Interrupt Handler to help catch interrupts. 
extern void vPortUnknownInterruptHandler( void *pvParameter );
extern void vPortInstallInterruptHandler( void (*vHandler)(void *), void *pvParameter, unsigned long ulVector, unsigned char ucEdgeTriggered, unsigned char ucPriority, unsigned char ucProcessorTargets );
	for ( ulVector = 0; ulVector < 160; ulVector++ )
		vPortInstallInterruptHandler( vPortUnknownInterruptHandler, (void *)ulVector, ulVector, pdTRUE, configMAX_SYSCALL_INTERRUPT_PRIORITY, 1 );


	/* Perform any other peripheral configuration. */
}
/*----------------------------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	__asm volatile (" smc #0 ");
}
/*----------------------------------------------------------------------------*/
