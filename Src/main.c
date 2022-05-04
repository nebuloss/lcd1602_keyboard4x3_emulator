#include "hal.h"
#include "os/os_start.h"

int main(){

    getchar();
    init_hal();
    os_start();
    end_hal();
    
}