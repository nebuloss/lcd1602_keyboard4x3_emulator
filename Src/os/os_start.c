#include "os/os_io.h"
#include "os/os_hal.h"
#include <stdio.h>

void os_start(){
    os_init();
    for (int i=0;i<40;i++) os_getchar();
        
    os_end();
}