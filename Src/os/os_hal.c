#include "os/os_hal.h"
#include "hal.h"
#include <time.h>
#include <stdbool.h>

static int os_hal_xscreen,os_hal_yscreen;
static clock_t os_hal_timer;

#ifdef LCD1602_KEYBOARD4x3_EMULATOR
static const unsigned os_hal_tick=CLOCKS_PER_SEC/1000;
#endif

static void _os_set_cursor_position(unsigned x,unsigned y){
    os_hal_xscreen=x;
    os_hal_yscreen=y;
    hal_set_cursor_position(x,y);
}

void os_set_cursor_position(unsigned x,unsigned y){
    if (x>=15) x=15;
    if (y) y=1;
    _os_set_cursor_position(x,y);
}

static bool os_move_cursor_right_check(){
    os_hal_xscreen++;
    if (os_hal_xscreen>15){
        if (os_hal_yscreen) os_hal_xscreen=15;
        else{
            os_hal_yscreen++;
            os_hal_xscreen=0;
        }
        return false;
    }
    return true;
}

void os_move_cursor_right(){
    os_move_cursor_right_check();
    hal_set_cursor_position(os_hal_xscreen,os_hal_yscreen);
}

void os_move_cursor_left(){
    os_hal_xscreen--;
    if (os_hal_xscreen<0){
        if (os_hal_yscreen){
            os_hal_yscreen--;
            os_hal_xscreen=15;
        }
        else os_hal_xscreen=0;
    }
    hal_set_cursor_position(os_hal_xscreen,os_hal_yscreen);
}

unsigned os_get_cursor_x(){
    return os_hal_xscreen;
}

unsigned os_get_cursor_y(){
    return os_hal_yscreen;
}

void os_putchar(char c){
    if (c!='\n' && c!='\r'){
        hal_putchar(c);
        if (!os_move_cursor_right_check()) hal_set_cursor_position(os_hal_xscreen,os_hal_yscreen);
    }
    else _os_set_cursor_position(0,1);
}

void os_start_chrono(){
    os_hal_timer=clock();
}

long os_stop_chrono(){
    long diff=clock()-os_hal_timer;
    #ifdef LCD1602_KEYBOARD4x3_EMULATOR
    return diff/os_hal_tick;
    #else
    return diff;
    #endif
}

void (*os_sleep)(unsigned)=hal_sleep;

void (*os_init)(void)=hal_init;

void (*os_end)(void)=hal_end;

char (*os_getkey)(void)=hal_getkey;

void (*os_clear)(void)=hal_clear;