#include "os/os_hal.h"
#include "os/os_io.h"
#include <stdio.h>
#include <unistd.h>

static int (*os_current_input_mode)()=os_simple_input_mode;
static const char* os_alpha_mode_characters[12]={
    "&'(1","ABC2","DEF3","GHI4","JKL5","MNO6","PQRS7","TUV8","WXYZ9","*"," 0","#"
};

void os_puts(char* buffer){
    for (char c;(c=*buffer);buffer++) os_putchar(c);
}

void os_puts_at(unsigned x,unsigned y,char* buffer){
    os_set_cursor_position(x,y);
    os_puts(buffer);
}

char os_wait_event(){
    char c;
    while (!(c=os_getkey())) os_sleep(1);
    os_sleep(100);
    return c;
}

int os_vprintf(char* restrict format,va_list va){
    char buffer[33];
    int value;

    value=vsnprintf(buffer,33,format,va);
    os_puts(buffer);
    return value;
}

int os_printf(char* restrict format,...){
    va_list va;
    int value;
    
    va_start(va,format);
    value=os_vprintf(format,va);
    va_end(va);
    return value;
}

int os_printf_at(unsigned x,unsigned y,char* restrict format,...){
    va_list va;
    int value;
    
    va_start(va,format);
    os_set_cursor_position(x,y);
    value=os_vprintf(format,va);
    va_end(va);
    return value;
}

char os_getchar(){
    char c;
    os_putchar((c=os_wait_event()));
    return c;
}

void os_set_input_mode(int (*input_mode)()){
    os_current_input_mode=input_mode;
}


char* os_read(char* buffer,unsigned lenght){
    unsigned x,y;
    unsigned n,l;
    int i;

    if (!buffer || !(l=31-((y=os_get_cursor_y())<<4)-(x=os_get_cursor_x()))) return NULL;

    n=0;
    lenght--;

    while ((i=os_current_input_mode())!=0){
        if (i==-1){
            if (!n) continue;
            n--;
            if (n<l){
                os_move_cursor_left();
                os_putchar(' ');
                os_move_cursor_left();
            }else{
                os_set_cursor_position(x,y);
                for (int j=n-l;j<n;j++) os_putchar(buffer[j]);
                os_set_cursor_position(15,1);
            } 
        }else{
            if (n==lenght) continue;
            buffer[n]=i;
            n++;
            if (n>l){
                os_set_cursor_position(x,y);
                for (int j=n-l;j<n;j++) os_putchar(buffer[j]);
                os_set_cursor_position(15,1);
                os_putchar(' ');
            }
            else os_putchar(i);
        }
    }
    if (!n) return NULL;
    if (n>lenght) n=lenght;
    buffer[n]='\0';
    return buffer;

}

int os_simple_input_mode(){
    char c=os_wait_event();
    
    os_sleep(100); //minimal time between keypress: does not support long keypress
    //os_log("pressed %c -> %d",c,c);
    if (c=='*') return -1;
    if (c=='#') return 0;

    return c;
}

int os_alpha_input_mode(){
    int c1,c2,n=0;
    char c;

    while ((c1=os_getkeynum())==-1) os_sleep(1);
    c=os_alpha_mode_characters[c1][0];
    os_sleep(100);
    if (c=='*') return -1;
    if (c=='#') return 0;

    os_static_putchar(c);

    while(1){
        
        if ((c2=os_getkeynum_timeout(400))==-1 || c2!=c1){
        	os_sleep(100);
            //os_log("c1=%d c2=%d",c1,c2);
            os_putkeynum(c2);
            break;
        }
        n++;
        if (!(c=os_alpha_mode_characters[c1][n])) n=c;
        c=os_alpha_mode_characters[c1][n];
        os_static_putchar(c);
        os_sleep(100);
    };
    
    return c;
    
}

void os_vpopup(char* title,char* message,va_list va){
    os_clear();
    os_puts(title);
    os_set_cursor_position(0,1);
    os_vprintf(message,va);
    os_wait_event();
}

void os_popup(char* title,char* message,...){
    va_list va;
    va_start(va,message);
    os_vpopup(title,message,va);
    va_end(va);
    
}

void os_error(char* message,...){
    va_list va;
    va_start(va,message);
    os_vpopup("Error:",message,va);
    va_end(va);
}

void os_info(char* message,...){
    va_list va;
    va_start(va,message);
    os_vpopup("Info:",message,va);
    va_end(va);
}

char* os_input(char* title,int (*input_mode)(void),char* buffer,int lenght){
    os_clear();
    os_puts(title);
    os_set_cursor_position(0,1);
    os_set_input_mode(input_mode);
    return os_read(buffer,lenght);
}
