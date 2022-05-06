#ifndef D2D40D82_A2DF_412E_BBF8_F26F752DA902
#define D2D40D82_A2DF_412E_BBF8_F26F752DA902

void os_set_cursor_position(unsigned x,unsigned y);

void os_move_cursor_right();

void os_move_cursor_left();

unsigned os_get_cursor_x();

unsigned os_get_cursor_y();

void os_putchar(char c);

void os_static_putchar(char c);

void os_start_chrono();

long os_stop_chrono();

char os_getkey();

char os_getkey_from_num(int num);

int os_getkeynum_from_key(char c);

void os_putkeynum(int num);

int os_getkeynum();

int os_getkeynum_timeout(long timeout);

char os_getkey_timeout(long timeout);

int os_log(char* format,...);

int os_reset_log();

extern void (*os_sleep)(unsigned ms);

extern void (*os_init)(void);

extern void (*os_end)(void);

extern void (*os_clear)(void);

#endif /* D2D40D82_A2DF_412E_BBF8_F26F752DA902 */
