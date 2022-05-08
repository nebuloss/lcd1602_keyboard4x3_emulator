#include "os/os_hal.h"
#include "os/os_toolkit.h"
#include "os/os_io.h"
#include <stdarg.h>

#define OS_PHONE_MAX_CONTACT 16
#define OS_PHONE_MESSAGE_LENGHT 256
#define OS_PHONE_NUMBER_LENGHT 11
#define OS_PHONE_CONTACT_LENGHT 16


static char os_phone_contacts_name[OS_PHONE_CONTACT_LENGHT][OS_PHONE_MAX_CONTACT];
static char os_phone_contacts_number[OS_PHONE_NUMBER_LENGHT][OS_PHONE_MAX_CONTACT];
static char* os_phone_contacts_name_arr[OS_PHONE_MAX_CONTACT];
static char* os_phone_contacts_number_arr[OS_PHONE_MAX_CONTACT];
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
    if (!os_type_phone_number(phone_number,OS_PHONE_NUMBER_LENGHT)) return;
    os_phone_call_graphic(phone_number);
}

void os_messaging(){
    char phone_number[OS_PHONE_NUMBER_LENGHT];
    char message[OS_PHONE_MESSAGE_LENGHT];
    if (!os_type_phone_number(phone_number,OS_PHONE_NUMBER_LENGHT)) return;
    if (!os_type_message(message,OS_PHONE_MESSAGE_LENGHT)) return;
    os_phone_sms_graphic(phone_number,message);
}

void os_contact(){
    int action,ncontact;
    char* contact_menu[2]={"Browse","Add"};
    char* contact_menu2[2]={"send sms","call"};
    char phone_number[OS_PHONE_NUMBER_LENGHT];
    char message[OS_PHONE_MESSAGE_LENGHT];
    char name[OS_PHONE_CONTACT_LENGHT];

    while ((action=os_menu("Contact menu:",2,contact_menu))!=-1){
        
        if (action){
            if (os_phone_contact_count==OS_PHONE_MAX_CONTACT) os_error("contact full");
            else if (os_input("Contact name:",os_alpha_input_mode,name,OS_PHONE_CONTACT_LENGHT)){
                if (os_type_phone_number(phone_number,OS_PHONE_NUMBER_LENGHT)){
                    os_strncpy(os_phone_contacts_name[os_phone_contact_count],name,OS_PHONE_CONTACT_LENGHT);
                    os_phone_contacts_name_arr[os_phone_contact_count]=(char*)&os_phone_contacts_name[os_phone_contact_count];
                    os_strncpy(os_phone_contacts_number[os_phone_contact_count],phone_number,OS_PHONE_NUMBER_LENGHT);
                    os_phone_contacts_number_arr[os_phone_contact_count]=(char*)&os_phone_contacts_number[os_phone_contact_count];
                    os_phone_contact_count++;
                    os_info("contact added");
                    
                }
            }
        }else{
            if ((ncontact=os_menu("Select contact:",os_phone_contact_count,os_phone_contacts_name_arr))!=-1){
                if ((action=os_menu("Choose an action:",2,contact_menu2))!=-1){
                    if (action) os_phone_call_graphic(os_phone_contacts_number_arr[ncontact]);
                    else{
                        os_type_message(message,OS_PHONE_MESSAGE_LENGHT);
                        os_phone_sms_graphic(os_phone_contacts_number_arr[ncontact],message);
                    }
                }
            }
        }
    }
    

    
}

void os_start(){
    int c;
    char* os_main_menu_entry[]={"Messaging","Dialer","Contact"};
    void (*os_main_menu_actions[3])(void)={os_messaging,os_dialer,os_contact};

    os_init();
    os_reset_log();
    
    while (1) if ((c=os_menu("Application menu",3,os_main_menu_entry))!=-1) os_main_menu_actions[c]();
}