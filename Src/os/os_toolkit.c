#include "os/os_hal.h"
#include "os/os_io.h"
#include "os/os_toolkit.h"

int os_menu(char* title,int n,char* entry[n]){
    int selection,c;

    os_clear();
    os_puts(title);

    if (n<=0){
        os_puts_at(0,1,"<   [EMPTY]   >");
        os_wait_event();
        return -1;
    }
    selection=0;

    while (1){
        
        os_printf_at(0,1,"< %12.12s >",entry[selection]);
        c=os_wait_event();
        if (c=='*'){
            if (!selection) selection=n;
            selection--;
        }else if (c=='#'){
            selection++;
            if (selection==n) selection=0;
        }
        else if (c=='0') break;
        else return -1;
    }
    return selection;
}

char* os_strncpy(char* dst,char* src,unsigned lenght){
    if (!dst || !src || !lenght) return NULL;
    for (int i=0;(*dst=*src);dst++,src++){
        if (++i>=lenght){
            *dst='\0';
            break;
        }
    }
    
    return dst;
}
