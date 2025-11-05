/*--------------------------------------------------
   Alias per i simboli BSS
--------------------------------------------------*/
_bss_start__ = _sbss;   /* Alias per __bss_start__ */
_bss_end__   = _ebss;   /* Alias per __bss_end__   */

/*--------------------------------------------------
   Definizione delle regioni di memoria
--------------------------------------------------*/
MEMORY
{
    /* ITCM: qui va la IVT all'indirizzo 0x00000000 */
    ITCM (xrw) : ORIGIN = 0x00000000, LENGTH = 0x8000     /* 32 KB (0x0000_0000 - 0x0000_7FFF) */

    /* DTCM */
    DTCM (rw)  : ORIGIN = 0x20000000, LENGTH = 0xF000     /* 60 KB (0x2000_0000 - 0x2000_EFFF) */

    /* Flash: 0x00400000 - 0x00BFFFFF */
    FLASH (xr) : ORIGIN = 0x00400000, LENGTH = 0x00800000 /* 8 MB */

    /* SRAM */
    SRAM0 (rw) : ORIGIN = 0x20400000, LENGTH = 256K       /* 0x2040_0000 - 0x2043_FFFF */
    SRAM1 (rw) : ORIGIN = 0x20440000, LENGTH = 256K       /* 0x2044_0000 - 0x2047_FFFF */
    SRAM2 (rw) : ORIGIN = 0x20480000, LENGTH = 256K       /* 0x2048_0000 - 0x204B_FFFF */
}

ENTRY(Reset_Handler)

/*--------------------------------------------------
   Configurazione per stack e heap
--------------------------------------------------*/
_Min_Heap_Size  = 0x100;    /* Dimensione minima dello heap  */
_Min_Stack_Size = 0x400;    /* Dimensione minima dello stack */

/* Indirizzo dello stack iniziale (in cima a SRAM2) */
_estack = ORIGIN(SRAM2) + LENGTH(SRAM2);

/*--------------------------------------------------
   Sezioni
--------------------------------------------------*/
SECTIONS
{
    /*------------------------------------------------
       1) IVT (Interrupt Vector Table) in ITCM
       all'indirizzo 0x00000000
    ------------------------------------------------*/
    .isr_vector :
    {
        __vector_table = .;     /* Etichetta per la tabella dei vettori */
        KEEP(*(.isr_vector))    /* Mantiene la sezione contenente i vettori */
        . = ALIGN(4);
    } > ITCM

    /*------------------------------------------------
       2) Codice e dati in sola lettura in Flash
    ------------------------------------------------*/
    .text :
    {
        *(.text)                /* Codice */
        *(.rodata*)             /* Dati in sola lettura */
        *(.constdata*)          /* Costanti */
        _etext = .;             /* Fine sezione di testo/RO */
        _sidata = .;            /* Origine dei dati iniz. per la copia in RAM */
    } > FLASH

    /*------------------------------------------------
       3) Dati in DTCM (inizializzati e non)

       .dtcm_data  -> variabili inizializzate
                      (copiate da FLASH a DTCM)
       .dtcm_bss   -> variabili non inizializzate (NOLOAD)
    ------------------------------------------------*/
    .dtcm_data :
    {
        . = ALIGN(8);
        _sdtcm_data = .;        /* Inizio dei dati inizializzati in DTCM */
        *(.dtcm_data)           /* Variabili con attributo `section(".dtcm_data")` */
        _edtcm_data = .;        /* Fine dei dati in DTCM */
    } > DTCM AT > FLASH

    .dtcm_bss (NOLOAD) :
    {
        . = ALIGN(8);
        _sdtcm_bss = .;         /* Inizio dei dati non inizializzati in DTCM */
        *(.dtcm_bss)
        *(COMMON)
        _edtcm_bss = .;         /* Fine */
    } > DTCM

    /*------------------------------------------------
       4) Sezione dati (.data) in SRAM0,
          caricati da FLASH (AT > FLASH)
    ------------------------------------------------*/
    .data :
    {
        . = ALIGN(8);
        _data  = .;
        _sdata = .;
        *(vtable)
        *(.data)
        _edata = .;
    } > SRAM0 AT > FLASH

    /*------------------------------------------------
       5) Sezione BSS (.bss) in SRAM1,
          dati non inizializzati
    ------------------------------------------------*/
    .bss :
    {
        . = ALIGN(8);
        __bss_start__ = .;
        _sbss         = .;
        *(.bss)
        *(COMMON)
        _ebss      = .;
        __bss_end__ = .;
    } > SRAM1

    /*------------------------------------------------
       6) Sezione Heap in SRAM2
    ------------------------------------------------*/
    .heap :
    {
        . = ALIGN(8);
        PROVIDE(end  = .);
        PROVIDE(_end = .);
        _heap_bottom = .;
        . = . + _Min_Heap_Size;
        _heap_top = .;
        . = ALIGN(8);
    } > SRAM2

    /*------------------------------------------------
       7) Configurazione dello Stack
          (cima di SRAM2)
    ------------------------------------------------*/
    __StackTop   = ORIGIN(SRAM2) + LENGTH(SRAM2);
    __StackLimit = __StackTop - _Min_Stack_Size;
    PROVIDE(__stack = __StackTop);

    /*------------------------------------------------
       8) Controllo finale su Stack/Heap
    ------------------------------------------------*/
    ASSERT(__StackLimit >= _heap_top, "region RAM overflowed with stack")
}
