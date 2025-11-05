# NXP S32K3X8EVB QEMU Emulation

This project implements and emulates the **NXP S32K3X8EVB** board inside **QEMU**, extending its ARM Cortex-M7 support.  
It includes UART and GPIO mapping, a FreeRTOS runtime environment, and an experimental MPU emulation module.

Developed by **Antonello Di Pede – Computer Architectures And Operating System - Politecnico di Torino (2024)**

---

## 🧠 Overview

QEMU is an open-source emulator and virtualizer that allows full software-based hardware simulation.  
This project extends QEMU to emulate the **S32K3X8EVB evaluation board**, enabling testing and development of embedded software without physical hardware.

Main objectives:
- Correct emulation of the **ARM Cortex-M7** core  
- Mapping and emulation of **UART** and **GPIO** peripherals  
- Execution of a **FreeRTOS project** inside QEMU  
- Design and integration of a **Memory Protection Unit (MPU)** prototype  

---

## ⚙️ Implementation Highlights

### 🔧 Board Emulation
- Implemented in `hw/arm/s32k3x8evb.c`  
- Defines memory regions (SRAM, Flash, ITCM, DTCM) and clock setup  
- Integrates CPU, clock, and memory via `sysbus_realize()`  

### 💬 UART & GPIO
- Custom UART implemented (`s32k3x8evb_uart.c/.h`)  
- Supports read/write operations, baud rate setup, and TX interrupts  
- GPIO mapped to its memory region (placeholder for future extension)

### 🧩 FreeRTOS Integration
- Runs a FreeRTOS demo project on the emulated board  
- Includes `main.c`, `startup.c`, `syscalls.c`, and `linker.ld` for proper memory mapping  
- Demonstrates scheduling, tasks, and I/O handling through the UART

### 🔒 MPU Simulation
- Partial MPU emulation for memory access protection  
- Implements core registers (MPU_TYPE, MPU_CTRL, MPU_RNR, MPU_RBAR, MPU_RASR)  
- Integrated into QEMU’s ARM layer with helper functions  
- Currently under development with planned improvements in access validation and fault handling

---

## HIGH LEVEL STRUCTURE OF THE FOLDERS:

```
qemu
 ├──FreeRTOS Project
 |        ├─Demo
 |        └─FreeRTOS
 ├─build
 |   └─
 |
 └───hw
     └─arm
        └─s32k3x8evb.c

```


# GUIDE : 
1. Clone this repository using --recurse-submodules to obtain the FreeRTOS source code required
2. Build the project with this command : ./configure --target-list=arm-softmmu in the ROOT of QEMU 
3. Copy the "FreeRTOS Project" folder into build/.
4. In the /build directory run this command : make -j $(nproc)
5. In the /FreeRTOS Project/Demo use the make command to compile and link the entire FreeRTOS Project
6. Return in the /build directory
7. Run with this command : ./qemu-system-arm -M s32k3x8evb -nographic -kernel test.elf

If you want debug use these commands: 
1. ./qemu-system-arm -M s32k3x8evb -nographic -S -s -kernel test.elf
2. Open another terminal and use this command gdb-multiarch test.elf
3. In the GDB interface use this command : target remote :1234 to connect 
    GDB COMMANDS : 
        
        1. info registers (To see the value of each register)
        2. si (Next Instrucions)
        3. continue 


## HIGH LEVEL EXPLANATION

With the ./configure command you create the /build directory

To verify the emulation of the board you can see that in register r4 there is the value 10 (a in hex)

