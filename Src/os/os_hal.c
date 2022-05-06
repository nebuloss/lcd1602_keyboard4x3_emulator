#include "os/os_hal.h"
#include "hal.h"
#include <time.h>
#include <stdbool.h>

static int xscreen,yscreen;
static clock_t os_hal_timer;

static void _os_set_cursor_position(unsigned x,unsigned y){
    xscreen=x;
    yscreen=y;
    hal_set_cursor_position(x,y);
}

void os_set_cursor_position(unsigned x,unsigned y){
    if (x>=15) x=15;
    if (y) y=1;
    _os_set_cursor_position(x,y);
}

static bool os_move_cursor_right_check(){
    xscreen++;
    if (xscreen>15){
        if (yscreen) xscreen=15;
        else{
            yscreen++;
            xscreen=0;
        }
        return false;
    }
    return true;
}

void os_move_cursor_right(){
    os_move_cursor_right_check();
    hal_set_cursor_position(xscreen,yscreen);
}

void os_move_cursor_left(){
    xscreen--;
    if (xscreen<0){
        if (yscreen){
            yscreen--;
            xscreen=15;
        }
        else xscreen=0;
    }
    hal_set_cursor_position(xscreen,yscreen);
}

unsigned os_get_cursor_x(){
    return xscreen;
}

unsigned os_get_cursor_y(){
    return yscreen;
}

void os_putchar(char c){
    if (c!='\n' && c!='\r'){
        hal_putchar(c);
        if (!os_move_cursor_right_check()) hal_set_cursor_position(xscreen,yscreen);
    }
    else _os_set_cursor_position(0,1);
}

void os_start_chrono(){
    os_hal_timer=clock();
}

long os_stop_chrono(){
    return clock()-os_hal_timer;
}

void (*os_sleep)(unsigned)=hal_sleep;

void (*os_init)()=hal_init;

void (*os_end)()=hal_end;

char (*os_getkey)()=hal_getkey;