#include "os/os_io.h"
#include "os/os_hal.h"
#include <stdio.h>

void os_start(){
    os_init();
    os_start_chrono();
    while (!os_getkey()){
        os_printf_at(0,0,"%d",os_stop_chrono());
        os_sleep(1);
    }
    os_end();
}