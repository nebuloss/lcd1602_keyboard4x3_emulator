#include "os/os_io.h"
#include "os/os_hal.h"

void os_start(){
    char buffer[17];
    os_init();
    os_puts_at(0,0,"type a message:");
    os_set_cursor_position(0,1);
    os_set_input_mode(os_alpha_input_mode);
    os_read(buffer,17);
    os_clear();
    os_puts_at(0,0,buffer);
    os_puts_at(0,1,"press a key...");
    os_wait_event();
    os_end();
}