#ifndef D2D40D82_A2DF_412E_BBF8_F26F752DA902
#define D2D40D82_A2DF_412E_BBF8_F26F752DA902

#include <stddef.h>

void os_set_cursor_position(unsigned x,unsigned y);

void os_move_cursor_right();

void os_move_cursor_left();

unsigned os_get_cursor_x();

unsigned os_get_cursor_y();

void os_putchar(char c);

void os_static_putchar(char c);

char os_getkey();

char os_getkey_from_num(int num);

int os_getkeynum_from_key(char c);

void os_putkeynum(int num);

int os_getkeynum();

int os_getkeynum_timeout(long timeout);

char os_getkey_timeout(long timeout);

int os_log(char* format,...);

int os_reset_log();

void os_clear();

extern void (*os_sleep)(unsigned ms);

extern void (*os_init)(void);

extern void (*os_end)(void);

extern int (*os_phone_sms)(char* phone_number,char* message);

extern int (*os_phone_call)(char* phone_number);

extern void (*os_phone_hang)(void);

#endif /* D2D40D82_A2DF_412E_BBF8_F26F752DA902 */
