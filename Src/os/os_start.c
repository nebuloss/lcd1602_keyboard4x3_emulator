#include "hal.h"
#include <stdio.h>

void os_start(){
    char c,buffer[16];
    start_chrono();
    while ((c=get_button())!='*'){
        msleep(1);
        if (!c) continue;
        snprintf(buffer,16,"%d  ",c);
        printxy(0,1,buffer);
        snprintf(buffer,16,"%d",stop_chrono());
        printxy(0,0,buffer);
        
    }
}