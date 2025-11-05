#include "hw/arm/s32k3x8evb_uart.h"
#include "hw/sysbus.h"
#include "qemu/units.h"
#include "qemu/error-report.h"
#include "qemu/log.h"
#include "hw/irq.h"


/* UART MMIO Operations */
static const MemoryRegionOps s32k3x8evb_uart_ops = {
    .read = s32k3x8evb_uart_read,
    .write = s32k3x8evb_uart_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

// Trigger a UART TX interrupt if conditions are met
static void uart_tx_interrupt(S32K3X8EVBUartState *uartDev) {
    if (uartDev->isUartIntEnabled && (uartDev->uartStat & UART_STATUS_TX_COMPLETE)) {
        qemu_set_irq(uartDev->irq, 1); // Assert IRQ signal
        // This is an unimplemented detail log for debugging
        qemu_log_mask(LOG_UNIMP, "UART interrupt triggered (status: 0x%x)\n", uartDev->uartStat);
    }
}

/* Read Function: called by QEMU when guest reads from the device */
uint64_t s32k3x8evb_uart_read(void *opaque, hwaddr addr, unsigned size) {
    S32K3X8EVBUartState *uartDev = opaque;

    switch (addr) {
        case 0x10: // BAUD RATE register
            return uartDev->uartBaud;
        case 0x18: // CONTROL register
            return uartDev->uartCtrl;
        case 0x14: // STATUS register
            qemu_log_mask(LOG_UNIMP, "UART STATUS READ at 0x%" PRIx64 ": 0x%x\n", addr, uartDev->uartStat);
            return uartDev->uartStat;  // ensure it returns TX_COMPLETE if set
        case 0x1C: // DATA register (dummy read)
            return uartDev->uartData;
        default:
            // It's an invalid read address
            qemu_log_mask(LOG_GUEST_ERROR, "Invalid UART read at address 0x%" PRIx64 "\n", addr);
            return 0;
    }
}

/* Write Function: called by QEMU when guest writes to the device */
void s32k3x8evb_uart_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
    S32K3X8EVBUartState *uartDev = opaque;
    
    qemu_log_mask(LOG_UNIMP, "UART WRITE: addr=0x%" PRIx64 ", val=0x%" PRIx64 "\n", addr, val);

    switch (addr) {
        case 0x10: // BAUD RATE
            uartDev->uartBaud = (uint32_t)val;
            break;

        case 0x18: // CONTROL
            uartDev->uartCtrl = (uint32_t)val;
            uartDev->isUartIntEnabled = ((uint32_t)val & UART_CTRL_INTERRUPT_ENABLE) != 0;
            break;

        case 0x1C: // DATA (Transmit)
            uartDev->uartData = (uint32_t)val;
            
            qemu_log_mask(LOG_UNIMP, "UART OUTPUT: %c\n", (char)val);

            // If there's an output file/stream set, write the character 
            if (uartDev->uartOut) {
                fputc((char)val, uartDev->uartOut);
                fflush(uartDev->uartOut);
            }

            //  Imposta il flag di TX completo 
            uartDev->uartStat |= UART_STATUS_TX_COMPLETE;
            //  Trigger the interrupt if enabled 
            uart_tx_interrupt(uartDev);
            break;

        case 0x14: // STATUS (Clear TX_COMPLETE)
            qemu_log_mask(LOG_UNIMP, "Clearing TX_COMPLETE flag\n");
            // Clear the TX_COMPLETE bit
            uartDev->uartStat &= ~UART_STATUS_TX_COMPLETE;
            break;

        default:
            qemu_log_mask(LOG_UNIMP, "Invalid UART write address: 0x%" PRIx64 "\n", addr);
            break;
    }
}

/* UART Initialization: sets default values and maps the memory region */
void s32k3x8evb_uart_realize(DeviceState *dev, Error **errp) {
    S32K3X8EVBUartState *uartDev = S32K3X8EVB_UART(dev);

    // Set default configurations
    uartDev->uartBaud = 115200;
    uartDev->uartCtrl = 0;
    uartDev->uartStat = UART_STATUS_TX_COMPLETE;  // ensure TX is ready
    uartDev->uartData = 0;
    uartDev->uartOut  = stdout;

    // Initialize MMIO region (Ensure size is correct!)
    memory_region_init_io(&uartDev->mem, OBJECT(uartDev), &s32k3x8evb_uart_ops, uartDev,
                          "s32k3x8evb_uart", 0x1000);

    // Bind the MMIO region to the system bus
    sysbus_init_mmio(SYS_BUS_DEVICE(uartDev), &uartDev->mem);

    // Initialize the IRQ line
    sysbus_init_irq(SYS_BUS_DEVICE(uartDev), &uartDev->irq); 

    /* Mappatura della memoria per la UART */
    sysbus_mmio_map(SYS_BUS_DEVICE(uartDev), 0, S32K3X8EVB_UART0_BASE);
}

/* Device Class Initialization */
 void s32k3x8evb_uart_class_init(ObjectClass *klass, void *data) {
    DeviceClass *dc = DEVICE_CLASS(klass);
    dc->realize = s32k3x8evb_uart_realize;
}

/* Register Device Type */
static const TypeInfo s32k3x8evb_uart_class_info = {
    .name          = TYPE_S32K3X8EVB_UART,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(S32K3X8EVBUartState),
    .class_init    = s32k3x8evb_uart_class_init,
};

/* Register the UART with QEMU */
static void s32k3x8evb_uart_register_types(void) {
    type_register_static(&s32k3x8evb_uart_class_info);
}

type_init(s32k3x8evb_uart_register_types);
