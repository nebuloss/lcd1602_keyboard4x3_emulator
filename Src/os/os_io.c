#include "os/os_hal.h"
#include "os/os_io.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

static int (*os_current_input_mode)()=os_simple_input_mode;

void os_puts(char* buffer){
    for (char c;(c=*buffer);buffer++) os_putchar(c);
}

char os_wait_event(){
    char c;
    while (!(c=os_getkey())) os_sleep(1);
    return c;
}

int os_printf(char* restrict format,...){
    va_list va;
    int value;
    char buffer[33];

    va_start(va,format);
    value=vsnprintf(buffer,33,format,va);
    os_puts(buffer);
    va_end(va);
    return value;
}

char os_getchar(){
    os_putchar(os_wait_event());
}

void os_set_input_mode(int (*input_mode)()){
    os_current_input_mode=input_mode;
}


char* os_read(char* buffer,unsigned lenght){
    unsigned x=os_get_cursor_x(),y=os_get_cursor_y();
    unsigned l=32-(y<<4)-x;
    unsigned n=0;
    int i;
    char data[128];
    while ((i=os_current_input_mode())){
        if (i==-1){
            if (n>0){
                n--;
                if (n>l){
                    os_set_cursor_position(x,y);
                    for (int j=n-l;j<n;j++) os_putchar(data[j]);
                    os_set_cursor_position(15,1);
                }else{
                    
                }
            }
            else continue;
        }else{
            if (n<128){
                data[n]=i;
                n++;
                if (n>l){
                    os_set_cursor_position(x,y);
                    for (int j=n-l;j<n;j++) os_putchar(data[j]);
                    os_set_cursor_position(15,1);
                }else{
                    os_putchar(i);
                }
            }
            else continue; 
        }
        
    }
    return NULL;

}

int os_simple_input_mode(){
    char c=os_wait_event();
    os_sleep(100); //minimal time between keypress: does not support long keypress
    if (c=='*') return -1;
    if (c=='#') return 0;
    return c;
}