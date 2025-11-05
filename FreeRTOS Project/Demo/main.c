#include "FreeRTOS.h"
#include "uart.h"
#include "task.h"
#define mainTASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )


void vTaskFunction(void *pvParameters);

int main(int argc, char **argv){

	(void) argc;
	(void) argv;
	
	xTaskCreate(
		// Function which implements the task
		vTaskFunction,
		// Name of the task (debug purposes, not used by the kernel)
		"Task1",
		// Stack to allocate to the task
		configMINIMAL_STACK_SIZE,
		// Parameter passed to the task. Not needed for Hello World example
		NULL,
		// Priority assigned to the task
		mainTASK_PRIORITY,
		// Task handle. Not required
		NULL
	);

	// Give control to the scheduler
	vTaskStartScheduler();

	// If everything ok should never reach here
    for( ; ; );
}

/* Task Function */
void vTaskFunction(void *pvParameters) {

	// Avoid warning about unused pvParameters
	(void) pvParameters;
        my_uart_printf("Ciao Sono Task1 \n");
        __asm volatile ("mov r4, #10");

}
