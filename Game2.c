#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#define ROW 30
#define COL ROW*2
#define ENCOUNTERS ROW*COL/300

char *world[ROW][COL];
    char path = ' ';
    char mountain = '#';
    char tree = '^';
    char pc = '$';
    char border = 'X';
    char encounter = '!';
char *deflt[ROW][COL];
int X = (COL/2);
int Y = (ROW/2);
int zoom = 10;

void InitializeWorld(void);
void SetWorld(void);
void movepc(int move);
void printworld(void);
void printclose(void);
void GenerateEncounters(void);
void mainmenu(void);



void main(void){
    struct termios info;
    tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info.c_lflag &= ~ICANON;      /* disable canonical mode */
    info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
    info.c_cc[VTIME] = 0;         /* no timeout */
    tcsetattr(0, TCSANOW, &info); /* set immediately */

    int num_options = 3;
    char *options[num_options];
    char option1[] = "1) World Map";
    char option2[] = "2) Game World";
    char option3[] = "3) Generate New Map";
    options[0] = (char *)&option1;
    options[1] = (char *)&option2;
    options[2] = (char *)&option3;
    char ch;
    int i,j;
    int move = 0;
    mainmenu();
    InitializeWorld();
    SetWorld();
    world[Y][X] = (char *)(&pc);
    GenerateEncounters();

    int mode = 0;
    while(1){


        system("clear");
        printf("  _____ _____  ______ _____    ____  _    _ ______  _____ _______ \n");
        printf(" / ____|  __ \\|  ____/ ____|  / __ \\| |  | |  ____|/ ____|__   __|\n");
        printf("| |  __| |__) | |__ | |  __  | |  | | |  | | |__  | (___    | |   \n");
        printf("| | |_ |  _  /|  __|| | |_ | | |  | | |  | |  __|  \\___ \\   | |   \n");
        printf("| |__| | | \\ \\| |___| |__| | | |__| | |__| | |____ ____) |  | |   \n");
        printf(" \\_____|_|  \\_\\______\\_____|  \\___\\_\\_____/|______|_____/   |_|   \n");
        printf("\n\t [OPTIONS]\n");
        printf("\t===========\n");
        for(i=0;i<num_options;i++){
            printf("\t%s\n",options[i]);
        }

        while((ch = getchar()) != 27) {
            if (ch < 0) {
                if (ferror(stdin)) {}
                clearerr(stdin);
            } else {
                if(ch=='1'){
                    mode = 1;
                    goto game;
                }else if(ch=='2'){
                    mode = 2;
                    goto game;
                }else if(ch=='3'){
                    InitializeWorld();
                    SetWorld();
                    world[Y][X] = (char *)(&pc);
                    GenerateEncounters(); 
                    printf("*generated new encounters\n");                   
                }
            }
        }


        while((ch = getchar()) != 27 /* ascii ESC */) {
            if (ch < 0) {
                if (ferror(stdin)) {}
                clearerr(stdin);
            } else {
                game:
                move = 0;
                if(ch=='d'){
                    if(X!=COL-1&&*(world[Y][X+1])==' '){
                        move = 1;
                    }else{
                        move = 0;
                    } 
                }else if(ch=='a'){
                    if(X!=0&&*(world[Y][X-1])==' '){
                        move = 2;
                    }else{
                        move = 0;
                    }
                }else if(ch=='w'){
                    if(Y!=0&&*(world[Y-1][X])==' '){
                        move = 3;
                    }else{
                        move = 0;
                    }
                }else if(ch=='s'){
                    if(Y!=ROW-1&&*(world[Y+1][X])==' '){
                    move = 4;
                    }else{
                        move = 0;
                    }
                }else if(ch=='-'){
                    if(!(zoom*2+1>=COL-1)){
                        zoom++;
                    }
                }else if(ch=='+'){
                    if((zoom>5)){
                        zoom--;
                    }
                }
                movepc(move);
                system("clear"); 
                if(mode==1){
                    printworld(); 
                }else if(mode==2){
                    printclose();
                }
        
            }
        }
    }


}
void mainmenu(void){
    system("clear");
    printf("__        __   _                          \n");
    printf("\\ \\      / /__| | ___ ___  _ __ ___   ___ \n");
    printf(" \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\\n");
    printf("  \\ V  V /  __/ | (_| (_) | | | | | |  __/\n");
    printf("   \\_/\\_/_\\___|_|\\___\\___/|_| |_| |_|\\___|\n");
    sleep(1);
    system("clear");
    printf("__        __   _                          \n");
    printf("\\ \\      / /__| | ___ ___  _ __ ___   ___ \n");
    printf(" \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\\n");
    printf("  \\ V  V /  __/ | (_| (_) | | | | | |  __/\n");
    printf("   \\_/\\_/_\\___|_|\\___\\___/|_| |_| |_|\\___|\n");
    printf("        |_   _|__                         \n");
    printf("          | |/ _ \\                        \n");
    printf("          | | (_) |                       \n");
    printf("          |_|\\___/                        \n");
    sleep(1);
    system("clear");
    printf("  _____ _____  ______ _____    ____  _    _ ______  _____ _______ \n");
    printf(" / ____|  __ \\|  ____/ ____|  / __ \\| |  | |  ____|/ ____|__   __|\n");
    printf("| |  __| |__) | |__ | |  __  | |  | | |  | | |__  | (___    | |   \n");
    printf("| | |_ |  _  /|  __|| | |_ | | |  | | |  | |  __|  \\___ \\   | |   \n");
    printf("| |__| | | \\ \\| |___| |__| | | |__| | |__| | |____ ____) |  | |   \n");
    printf(" \\_____|_|  \\_\\______\\_____|  \\___\\_\\_____/|______|_____/   |_|   \n");
    sleep(1);


}

void printworld(void){
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            printf("%c",*(world[i][j]));
        }
        printf("\n");
    }
    printf("\n");
    printf("X==%d Y==%d\n",X,Y); 
}
void InitializeWorld(void){
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            deflt[i][j] = (char *)(&path);
        }
    }
    int k = COL*2/3;
    for(i=0;i<(COL*2/3)+1;i++){
        for(j=0;j<k;j++){
            deflt[i][j] = (char *)(&mountain);
        }
        k -=2;
    }
    for(i=ROW/3;i<ROW;i++){
        for(j=COL*2/3;j<COL;j++){
            deflt[i][j] = (char *)(&tree);
        }
    }
    k = COL;
    for(i=ROW/1.375;i<ROW;i++){
        for(j=0;j<COL-k;j++){
            deflt[i][j] = (char *)(&tree);
        }
        k -=6;
    }
    for(i=0;i<COL;i++){
        deflt[0][i] = (char *)(&border);
    }
    for(i=0;i<COL;i++){
        deflt[ROW-1][i] = (char *)(&border);
    }
    for(i=0;i<ROW;i++){
        deflt[i][0] = (char *)(&border);
    }
    for(i=0;i<ROW;i++){
        deflt[i][COL-1] = (char *)(&border);
    }
}
void SetWorld(void){
    int i,j;
    for(i=0;i<ROW;i++){
        for(j=0;j<COL;j++){
            world[i][j] = deflt[i][j];
        }
    }
}
void GenerateEncounters(void){
    int randx;
    int randy;
    int enc = 0;
    while (enc<ENCOUNTERS){
        randx = rand() % (COL-1 + 1);
        randy = rand() % (ROW-1 + 1);
        if(*(world[randy][randx])==' '){
            world[randy][randx] = (char *)(&encounter);
            enc++;
        }
    }
}
void movepc(int move){
    if(move!=0){
        world[Y][X] = deflt[Y][X];
        if(move==1){
            X++;
        }else if(move==2){
            X--;
        }else if(move==3){
            Y--;
        }else{
            Y++;
        }
    }
    world[Y][X] = (char *)(&pc);
}

void printclose(void){
    int i,j;
    int tempx,tempy;
    
    int xscale = zoom;
    int yscale = xscale/2;

    if(X+xscale>COL-1){
        tempx = COL-xscale;
    }else if(X-xscale<0){
        tempx = 0 + xscale;
    }else{
        tempx = X;
    }

    if(Y+yscale>ROW-1){
        tempy = ROW-yscale;
    }else if(Y-yscale<0){
        tempy = 0 + yscale;
    }else{
        tempy = Y;
    }

    for(i=tempy-yscale;i<tempy+yscale;i++){
        for(j=tempx-xscale;j<tempx+xscale;j++){
            printf("%c",*(world[i][j]));
        }
        printf("\n");
    }
    printf("\n");
    printf("X==%d Y==%d\n",X,Y);
    printf("Zoom == %d\n",zoom); 
}
