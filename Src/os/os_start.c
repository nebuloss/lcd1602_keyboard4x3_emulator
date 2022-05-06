#include "os/os_io.h"
#include "os/os_hal.h"
#include <stdio.h>

void os_start(){
    char buffer[16];
    char* test="123";
    os_init();
    os_reset_log();
    os_puts(test);
    os_wait_event();
    os_set_cursor_position(0,1);
    os_set_input_mode(os_alpha_input_mode);
    os_read(buffer,10);
    os_set_cursor_position(0,0);
    os_puts(buffer);
    os_wait_event();
    os_end();
}