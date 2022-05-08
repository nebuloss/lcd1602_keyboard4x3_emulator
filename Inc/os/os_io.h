#ifndef BE6F0CD6_1743_4A93_B3A3_D9482521D5FF
#define BE6F0CD6_1743_4A93_B3A3_D9482521D5FF
#include <stdarg.h>

void os_puts(char* buffer);

void os_puts_at(unsigned x,unsigned y,char* buffer);

char os_wait_event();

int os_printf(char* restrict format,...);

int os_vprintf(char* restrict format,va_list va);

int os_printf_at(unsigned x,unsigned y,char* restrict format,...);

char os_getchar();

void os_set_input_mode(int (*input_mode)());

int os_simple_input_mode();

char* os_read(char* buffer,unsigned lenght);

int os_alpha_input_mode();

void os_vpopup(char* title,char* message,va_list va);

void os_popup(char* title,char* message,...);

void os_error(char* message,...);

void os_info(char* message,...);

char* os_input(char* title,int (*input_mode)(void),char* buffer,int lenght);

#endif /* BE6F0CD6_1743_4A93_B3A3_D9482521D5FF */
