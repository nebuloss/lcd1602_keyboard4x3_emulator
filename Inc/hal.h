#ifndef FF55E453_3B0C_4574_A40C_0F8E9E23D065
#define FF55E453_3B0C_4574_A40C_0F8E9E23D065

#include <stdio.h>


void init_hal();

void end_hal();

void set_cursor_position(unsigned x,unsigned y);

unsigned get_cursor_x();

unsigned get_cursor_y();

//new function
void printchar(char c);

void print(char* buffer);

void printxy(unsigned x,unsigned y,char* buffer);

char get_button();

void start_chrono();

unsigned stop_chrono();

void msleep(unsigned ms);

#endif /* FF55E453_3B0C_4574_A40C_0F8E9E23D065 */
