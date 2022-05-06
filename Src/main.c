#include "os/os_start.h"
#include <stdio.h>

int main(){
    puts("the keyboard is in alpha mode:\nYou must hit several time the same key to access other characters!");
    getchar();
    os_start();
    return 0;
    
}