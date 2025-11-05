#include <stdint.h>

int main(void) {
    // Scrive il valore 10 nel registro r0 utilizzando inline assembly
    __asm volatile ("mov r0, #10");

    // Loop infinito per fermare l'esecuzione
    while (1) {
        // Mantieni il processore occupato
    }

    return 0; // Non verrà mai raggiunto
}

