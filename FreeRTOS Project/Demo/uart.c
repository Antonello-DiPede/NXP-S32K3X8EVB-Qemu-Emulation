#include "uart.h"
#include <stdio.h>

/* Initialize the UART with a specific baud rate */
void my_uart_init(uint32_t new_baudrate) {
    // Setting up the baud rate register 
    *(volatile uint32_t *)UART_REG_BAUD = new_baudrate;  

    /* Abilita l'interrupt per la UART, se necessario */
    *(volatile uint32_t *)UART_REG_CTRL = UART_FL_INT_EN;  
}

// Transmit a single character via UART
void my_uart_transmit_char(char character) {
    volatile uint32_t *my_stat_reg = (volatile uint32_t *)UART_REG_STAT;
    volatile uint32_t *my_dat_reg  = (volatile uint32_t *)UART_REG_DATA;

    // Wait until TX is ready 
    while (!(*my_stat_reg & UART_FL_TX_DONE));

    // Clear the TX done flag by writing to status
    *my_stat_reg = 0;

    // Write the character to the data register
    *my_dat_reg = (uint32_t)character;

    // Check again to ensure the character is fully sent
    while (!(*my_stat_reg & UART_FL_TX_DONE));
}

/* Print formatted data through UART */
void my_uart_printf(const char *format, ...) {
    char local_buffer[256]; 
    va_list args;

    // Start reading variadic arguments
    va_start(args, format);
    vsnprintf(local_buffer, sizeof(local_buffer), format, args);
    va_end(args);

    // Now, send the formatted string, one char at a time
    for (char *ptr = local_buffer; *ptr != '\0'; ptr++) {
        my_uart_transmit_char(*ptr);
        // QUi si potrebbe aggiungere un delay se serve
    }
}
