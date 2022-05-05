#include "hal.h"
#include "os/os_io.h"
#include <stdio.h>

void os_start(){
    os_printf("bienvenue %d\ntest",12);
    for (int i=0;i<10;i++) os_wait_event();
        
    
}