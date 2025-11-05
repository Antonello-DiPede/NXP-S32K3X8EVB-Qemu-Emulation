#include <stdint.h>

/* Dichiarazioni di funzioni */
extern void vPortSVCHandler(void);
extern void xPortPendSVHandler(void);
extern void xPortSysTickHandler(void);

void Reset_Handler( void ) __attribute__( ( naked ) );
static void Default_Handler( void ) __attribute__( ( naked ) );
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* Gestori delle eccezioni */
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
//void SVC_Handler(void)     __attribute__((weak, alias("vPortSVCHandler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
//void PendSV_Handler(void) __attribute__((weak, alias("xPortPendSVHandler")));
//void SysTick_Handler(void) __attribute__((weak, alias("xPortSysTickHandler")));

/* Dichiarazione esterna di `main` */
extern int main(void);

/* Dichiarazione dei simboli definiti nel linker.ld */
extern uint32_t _estack;       /* Estremità superiore dello stack */
extern uint32_t _sidata;       /* Inizio della sezione .data in Flash */
extern uint32_t _sdata;        /* Inizio della sezione .data in RAM */
extern uint32_t _edata;        /* Fine della sezione .data in RAM */
extern uint32_t __bss_start__;         /* Inizio della sezione .bss */
extern uint32_t __bss_end__;         /* Fine della sezione .bss */

/* Tabella delle ISR */
__attribute__((section(".isr_vector"), used))
const uint32_t *isr_vector[] = {
    (uint32_t *)&_estack,       /* Stack Pointer iniziale */
    (uint32_t *)&Reset_Handler,    /* Reset Handler */
    (uint32_t *)&Default_Handler,  /* NMI Handler */
    (uint32_t *)&HardFault_Handler,/* HardFault Handler */
    (uint32_t *)&Default_Handler,  /* MemManage Handler */
    (uint32_t *)&Default_Handler,  /* BusFault Handler */
    (uint32_t *)&Default_Handler,  /* UsageFault Handler */
    0,                             /* Riservato */
    0,                             /* Riservato */
    0,                             /* Riservato */
    0,                             /* Riservato */
    (uint32_t *)&vPortSVCHandler,      /* SVC Handler */
    (uint32_t *)&Default_Handler,  /* DebugMon Handler */
    0,                             /* Riservato */
    (uint32_t *)&xPortPendSVHandler,   /* PendSV Handler */
    (uint32_t *)&xPortSysTickHandler   /* SysTick Handler */
};

/* Reset Handler */
void Reset_Handler(void) {
    /* Copia della sezione .data dalla Flash alla RAM */
    uint32_t *src = &_sidata;
    uint32_t *dest = &_sdata;
    while (dest < &_edata) {
        *dest++ = *src++;
    }

    /* Azzeramento della sezione .bss */
    dest = &__bss_start__;
    while (dest < &__bss_end__) {
        *dest++ = 0;
    }

    /* Chiamata alla funzione main */
    main();

    /* Loop infinito se main termina */
    while (1) {
    }
}

/* Gestore di default per eccezioni non gestite */
void Default_Handler(void) {
     while (1) {
        /* Loop infinito */
    }
}

