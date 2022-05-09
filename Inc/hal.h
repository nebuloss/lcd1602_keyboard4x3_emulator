#ifndef FF55E453_3B0C_4574_A40C_0F8E9E23D065
#define FF55E453_3B0C_4574_A40C_0F8E9E23D065
#define LCD1602_KEYBOARD4x3_EMULATOR//for specific functions targetting emulator such as logging.

void hal_putchar(char c);

int hal_getkeynum();

void hal_set_cursor_position(unsigned x,unsigned y);

void hal_init();

void hal_end();

void hal_sleep(unsigned ms);

void hal_clear();

int hal_phone_sms(char* phone_number,char* message);

int hal_phone_call(char* phone_number);

void hal_phone_hang();

#endif /* FF55E453_3B0C_4574_A40C_0F8E9E23D065 */
