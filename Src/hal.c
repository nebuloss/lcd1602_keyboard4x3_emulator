#include "hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>



#define PUTPIXEL(r,g,b) printf("\e[48;2;%hhu;%hhu;%hhum  ",r,g,b) //permet d'afficher un pixel (pour l'affichage des bitmaps)
#define RESET_COLOR() printf("\e[0m")
#define SET_FOREGROUND(fg) printf("\e[38;2;%hhu;%hhu;%hhum",fg.r,fg.g,fg.b)
#define SET_BACKGROUND(bg) printf("\e[48;2;%hhu;%hhu;%hhum",bg.r,bg.g,bg.b)
#define SET_COLOR(fg,bg) printf("\e[38;2;%hhu;%hhu;%hhu;48;2;%hhu;%hhu;%hhum",fg.r,fg.g,fg.b,bg.r,bg.g,bg.b) //règle la couleur du cuseur
#define PRINTXY(r,x,y,format,...) printf("\e[%u;%uH"format,r.yo+y,r.xo+x,##__VA_ARGS__)

#define SET_TERMINAL_DIMENSION(width,height) printf("\e[8;%d;%dt",height,width) //dimensionne de force le terminal
#define GOTOXY(x,y) printf("\e[%d;%dH",y,x) //déplace le curseur
#define HIDE_CURSOR() printf("\e[?25l"); //cache le curseur
#define SHOW_CURSOR() printf("\e[?25h"); //affiche le curseur
#define CLEAR_SCREEN() printf("\e[2J"); //efface l'écran (scoll dans le terminal)

#define MIN2(a,b) (a<b?a:b)
#define MAX2(a,b) (a>b?a:b)
#define BETWEEN(a,n,b) MAX2(MIN2(n,b),a)

typedef struct color{ //structure couleur RGB256
    uint8_t r,g,b;
}color;

#define BLUE (color){0x00,0x00,0xFF} //définition de quelques couleurs utiles
#define BLACK (color){0x00,0x00,0x00}
#define WHITE (color){0xFF,0xFF,0xFF}
#define RED (color){0xFF,0x00,0x00}
#define GREEN (color){0x00,0xFF,0x00}
#define BROWN (color){0x58,0x29,0x00}
#define LIGHT_BLUE (color){0x77,0xB5,0xFE}
#define LIGHT_RED (color){0xC0,0x40,0x40}
#define ORANGE (color){0xFC,0x93,0x03}
#define LIGHT_VIOLET (color){0xB0,0x9D,0xB9}
#define GREY (color){0x78,0x78,0x78}

typedef struct rect{ //definition d'une structure permettant de stocker un rectangle (point d'origine, largeur, hauteur)
    uint8_t xo,yo,width,height;
}rect;

#define WINDOW_WIDTH 42 //constantes
#define WINDOW_HEIGHT 26

#define DEVICE_WIDTH 18
#define DEVICE_HEIGHT 21
#define DEVICE_X 13
#define DEVICE_Y 3

#define SCREEN_WIDTH 18
#define SCREEN_HEIGHT 4

#define DEVICE (rect){DEVICE_X,DEVICE_Y,DEVICE_WIDTH,DEVICE_HEIGHT}
#define SCREEN_BORDER (rect){DEVICE_X,DEVICE_Y,SCREEN_WIDTH,4}
#define SCREEN (rect){DEVICE_X+1,DEVICE_Y+1,SCREEN_WIDTH-2,2}


static char* clist="123456789*0#";
static bool statuslist[12];
static int xscreen,yscreen;

pthread_t keyboardThread;
pthread_mutex_t keyboardMutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t displayMutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t quitMutex=PTHREAD_MUTEX_INITIALIZER;

static void fill_rect(rect r,color c){ //permet de remplir une zone d'une certaine couleur
    SET_BACKGROUND(c); //on règle la couleur du curseur
    for (uint8_t x,y=0;y<r.height;y++){
        GOTOXY(r.xo,r.yo+y);
        for (x=0;x<r.width;x++) putchar(' ');
    }
}

static void display_button(rect r,char c,color fg,color bg){ //affichage d'un nombre centré dans le rectangle
    
    fill_rect(r,bg);
    SET_FOREGROUND(fg);
    PRINTXY(r,(r.width>>1),(r.height>>1),"%c",c);
}

static void display_screen(){
    fill_rect(SCREEN_BORDER,BROWN);
    fill_rect(SCREEN,BLUE);
}

static void display_keyboard(bool* status){
    rect r;
    uint8_t xstart=DEVICE_X+3,xend=DEVICE_X+11,yend=DEVICE_Y+17;
    r.yo=DEVICE_Y+5;
    r.width=r.height=3;

    for (char *c=clist;r.yo<=yend;r.yo+=4){
        for (r.xo=xstart;r.xo<=xend;r.xo+=4,c++){
            if (*status++) display_button(r,*c,WHITE,RED);
            else display_button(r,*c,WHITE,GREY);
        }
    }
}

static void* display_loop(void* arg){
    bool statuscopy[12];
    while (!pthread_mutex_trylock(&quitMutex)){
        pthread_mutex_unlock(&quitMutex);
        
        pthread_mutex_lock(&keyboardMutex);
        for (int j=0;j<12;j++){
            statuscopy[j]=statuslist[j];
            statuslist[j]=false;
        }
        pthread_mutex_unlock(&keyboardMutex);
        
        
        
        pthread_mutex_lock(&displayMutex);
        HIDE_CURSOR();
        display_keyboard(statuscopy);
        GOTOXY(SCREEN.xo+xscreen,SCREEN_BORDER.yo+yscreen);
        putchar('\n'); //refresh
        SET_COLOR(WHITE,BLUE);
        SHOW_CURSOR();

        pthread_mutex_unlock(&displayMutex);

        hal_sleep(100);
    }
    return NULL;
}

void hal_putchar(char c){
    pthread_mutex_lock(&displayMutex);
    putchar(c);
    xscreen++;
    pthread_mutex_unlock(&displayMutex);
}

char hal_getkey(){
    int attrib;  
    char c;
    bool key;
    
    attrib = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl (STDIN_FILENO, F_SETFL, attrib | O_NONBLOCK);
    c = getchar();
    fcntl (STDIN_FILENO, F_SETFL, attrib);

    #ifdef __gnu_linux__
    stdin->_IO_read_ptr=stdin->_IO_read_end; //flush stdin
    #endif
    
    key=false;

    pthread_mutex_lock(&keyboardMutex);
    for (int i=0;i<12;i++){
        if (c==clist[i]){
            statuslist[i]=true;
            key=true;
        }
    }
    pthread_mutex_unlock(&keyboardMutex);
    
    if (!key) return 0;

    return c;
}

void hal_set_cursor_position(unsigned x,unsigned y){
    pthread_mutex_lock(&displayMutex);
    xscreen=x;
    yscreen=y;
    GOTOXY(SCREEN.xo+xscreen,SCREEN.yo+yscreen);
    pthread_mutex_unlock(&displayMutex);
}

void hal_init(){
    SET_TERMINAL_DIMENSION(WINDOW_WIDTH,WINDOW_HEIGHT);
    CLEAR_SCREEN();
    system("stty raw -echo");
    fill_rect(DEVICE,LIGHT_VIOLET);
    display_screen();
    hal_set_cursor_position(0,0);
    pthread_create(&keyboardThread,NULL,display_loop,NULL);
}

void hal_end(){
    pthread_mutex_lock(&quitMutex);
    pthread_join(keyboardThread,NULL);
    pthread_mutex_unlock(&quitMutex);
    GOTOXY(1,1);
    system("stty cooked echo");
    RESET_COLOR();
    CLEAR_SCREEN();
}

void hal_sleep(unsigned ms){
    usleep(ms*1000);
}