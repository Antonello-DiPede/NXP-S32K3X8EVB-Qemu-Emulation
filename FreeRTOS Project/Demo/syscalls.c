#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

/* Gestione della memoria */
caddr_t _sbrk(int incr) {
    extern char _heap_bottom; /* Definito nel linker.ld */
    extern char _heap_top;    /* Definito nel linker.ld */
    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_heap_bottom;
    }
    prev_heap_end = heap_end;

    if (heap_end + incr > &_heap_top) {
        /* Esaurita la memoria heap */
        write(1, "Heap overflow\n", 14);
        abort();
    }

    heap_end += incr;
    return (caddr_t)prev_heap_end;
}

/* Gestione input/output */
int _write(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return -1; /* Non implementato */
}

int _read(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return -1; /* Non implementato */
}

int _close(int file) {
    (void)file;
    return -1; /* Non implementato */
}

int _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return -1; /* Non implementato */
}

int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file) {
    return 1;
}

void _exit(int status) {
    (void)status;
    while (1) {
        /* Loop infinito */
    }
}

/* Funzioni aggiunte */
int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    return -1; /* Non implementato */
}

int _getpid(void) {
    return 1; /* Restituisce un PID fittizio */
}
