#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdarg.h>

/* Base address for UART0 - essential for hardware mapping */
#define UART_REG_BASE        0x40328000  

// Baud rate register offset from base
#define UART_REG_BAUD        (UART_REG_BASE + 0x10)

// Data register offset
#define UART_REG_DATA        (UART_REG_BASE + 0x1C)

// Control register offset
#define UART_REG_CTRL        (UART_REG_BASE + 0x18)

/* Status register offset */
#define UART_REG_STAT        (UART_REG_BASE + 0x14)


/* TX done flag */
#define UART_FL_TX_DONE      (1 << 0)

/* Interrupt enable flag */
#define UART_FL_INT_EN       (1 << 1)

/* Function prototypes for our custom UART */
void my_uart_init(uint32_t new_baudrate);
void my_uart_transmit_char(char character);
void my_uart_printf(const char *format, ...);

#endif /* UART_H */
