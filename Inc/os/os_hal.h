#ifndef D2D40D82_A2DF_412E_BBF8_F26F752DA902
#define D2D40D82_A2DF_412E_BBF8_F26F752DA902

void os_set_cursor_position(unsigned x,unsigned y);

void os_move_cursor_right();

void os_move_cursor_left();

unsigned os_get_cursor_x();

unsigned os_get_cursor_y();

void os_putchar(char c);

void os_start_chrono();

long os_stop_chrono();

extern void (*os_sleep)(unsigned ms);

extern void (*os_init)(void);

extern void (*os_end)(void);

extern char (*os_getkey)(void);

extern void (*os_clear)(void);

#endif /* D2D40D82_A2DF_412E_BBF8_F26F752DA902 */
