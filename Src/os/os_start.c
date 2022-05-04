#include "hal.h"
#include <stdio.h>

void os_start(){
    char c,buffer[16];
    while ((c=get_button())!='*'){
        msleep(1);
        if (!c) continue;
        snprintf(buffer,16,"%d  ",c);
        printxy(0,1,buffer);
        
    }
}