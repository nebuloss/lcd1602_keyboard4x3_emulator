#include "os/os_hal.h"
#include "hal.h"
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

static struct timeval os_hal_tval_before, os_hal_tval_after, os_hal_tval_result;

#ifdef LCD1602_KEYBOARD4x3_EMULATOR
#include <stdio.h>
#include <stdarg.h>
static const char* logfile="os.log";
#endif

static unsigned os_hal_xscreen,os_hal_yscreen;
static const char* os_hal_character_list="123456789*0#";
static int os_hal_buffer_keynum=-1;

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

void os_static_putchar(char c){
    hal_putchar(c);
    hal_set_cursor_position(os_hal_xscreen,os_hal_yscreen);
}

void os_start_chrono(){
    gettimeofday(&os_hal_tval_before, NULL);
}

long os_stop_chrono(){
    gettimeofday(&os_hal_tval_after, NULL);
    os_hal_tval_result.tv_sec = os_hal_tval_after.tv_sec - os_hal_tval_before.tv_sec;
    os_hal_tval_result.tv_usec = os_hal_tval_after.tv_usec - os_hal_tval_before.tv_usec;
    if (os_hal_tval_result.tv_usec < 0) { 
        os_hal_tval_result.tv_sec--; 
        os_hal_tval_result.tv_usec += 1000000;
    }

    return (os_hal_tval_result.tv_usec/1000)+os_hal_tval_result.tv_sec*1000;
}

int os_reset_log(){
    #ifdef LCD1602_KEYBOARD4x3_EMULATOR
    return fclose(fopen(logfile,"w"));
    #else
    return -1;
    #endif
}

int os_log(char* format,...){
    #ifdef LCD1602_KEYBOARD4x3_EMULATOR
    time_t t;
    struct tm *now;
    va_list va;
    FILE* f;

    if ((f=fopen(logfile,"a"))){
        t=time(&t); //on récupère l'heure
        now=localtime(&t);
        fprintf(f,"%4d-%02d-%02d %02d:%02d:%02d> ",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
        va_start(va,format); //utilisation des va_args pour gérer les arguments à nombres variables
        vfprintf(f,format,va);
        va_end(va); //libération des ressources des va_args
        putc('\n',f); //retour à la ligne
        fclose(f);
        return 0;
    }
    
    #endif
    return -1;
    
}

char os_getkey_from_num(int num){
    if (num==-1) return 0;
    return os_hal_character_list[num];
}

int os_getkeynum_from_key(char c){
    for (int i=0;i<12;i++) if (os_hal_character_list[i]==c) return i;
    return -1;
}

char os_getkey(){
    return os_getkey_from_num(os_getkeynum());
}

void os_putkeynum(int num){
    os_hal_buffer_keynum=num;
}


int os_getkeynum(){
    int key;
    if ((key=os_hal_buffer_keynum)==-1) return hal_getkeynum();
    os_hal_buffer_keynum=-1;
    return key;
}

int os_getkeynum_timeout(long timeout){
    int key;

    if ((key=os_hal_buffer_keynum)!=-1){
        os_hal_buffer_keynum=-1;
        return key;
    }
    os_start_chrono();
    do{
        key=hal_getkeynum();
        os_sleep(1);
    }while(key==-1 && os_stop_chrono()<timeout);
    return key;
}

char os_getkey_timeout(long timeout){
    return os_getkey_from_num(os_getkeynum_timeout(timeout));
}

void os_clear(){
    hal_clear();
    os_hal_xscreen=os_hal_yscreen=0;
}

void (*os_sleep)(unsigned)=hal_sleep;

void (*os_init)(void)=hal_init;

void (*os_end)(void)=hal_end;

int (*os_phone_sms)(char*,char*)=hal_phone_sms;

int (*os_phone_call)(char*)=hal_phone_call;