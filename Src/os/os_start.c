#include "os/os_hal.h"
#include "os/os_toolkit.h"
#include "os/os_io.h"
#include <stdarg.h>

char os_phone_contacts_name[16][16];
char os_phone_contacts_number[10][16];
unsigned os_phone_contact_count;

char* os_type_phone_number(char* buffer,int lenght){
    return os_input("Phone number:",os_simple_input_mode,buffer,lenght);
}

char* os_type_message(char* buffer,int lenght){
    return os_input("Message:",os_alpha_input_mode,buffer,lenght);
}



int os_phone_sms_graphic(char* phone_number,char* message){
    int status;
    os_clear();
    os_puts("sending sms:\n->");
    os_puts_at(3,1,phone_number);
    if ((status=os_phone_sms(phone_number,message))<0) os_error("sms not sent");
    return status;
}

int os_phone_call_graphic(char* phone_number){
    int status;
    os_clear();
    os_puts("calling:\n->");
    os_puts_at(3,1,phone_number);
    if ((status=os_phone_call(phone_number))<0) os_error("fail to call");
    return status;
}

void os_dialer(){
    char phone_number[11];
    if (!os_type_phone_number(phone_number,11)) return;
    os_phone_call_graphic(phone_number);
}

void os_messaging(){
    char phone_number[11];
    char message[256];
    if (!os_type_phone_number(phone_number,11)) return;
    if (!os_type_message(message,256)) return;
    os_phone_sms_graphic(phone_number,message);
}

void os_contact(){
    int action,ncontact;
    char* contact_menu[3]={"Browse","Add"};
    char phone_number[11];
    char message[256];
    char name[16];

    while ((action=os_menu("Contact menu:",2,contact_menu))!=-1){
        
        if (action){
            if (os_input("Contact name:",os_alpha_input_mode,name,16)!=-1){
                if (os_type_phone_number(phone_number,11)){
                    if (os_phone_contact_count==16) os_error("contact full");
                    else{
                        os_strncpy(os_phone_contacts_name[os_phone_contact_count],name,16);
                        os_strncpy(os_phone_contacts_number[os_phone_contact_count],phone_number,10);
                        os_phone_contact_count++;
                        os_info("contact added");
                    }
                }
            }
        }else{
            if ((ncontact=os_menu("Select contact:",os_phone_contact_count,os_phone_contacts_name))!=-1){
                
            }
        }
    }
    

    
}

void os_start(){
    int c;
    char* os_main_menu_entry[]={"Messaging","Dialer","Contact"};
    void (*os_main_menu_actions[3])(void)={os_messaging,os_dialer,os_contact};

    os_init();
    os_clear();
    os_puts("hey\n->test");
    os_wait_event();
    
    while (1) if ((c=os_menu("Application menu",3,os_main_menu_entry))!=-1) os_main_menu_actions[c]();
}