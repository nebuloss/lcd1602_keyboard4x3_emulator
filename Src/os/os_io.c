#include "hal.h"
#include "os/os_io.h"
#include <stdio.h>
#include <stdarg.h>

void os_wait_event(){
    while (!get_button()) msleep(1);
}

int os_printf(char* restrict format,...){
    va_list va;
    int value;
    char buffer[33];

    va_start(va,format);
    value=vsnprintf(buffer,33,format,va);
    print(buffer);
    va_end(va);
    return value;
}