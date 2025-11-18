#include <stdlib.h>
#include <stdio.h>
#include <termios.h>

#define YMAX 49
#define XMAX 34



void main(void){

    struct termios info;
    tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info.c_lflag &= ~ICANON;      /* disable canonical mode */
    info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
    info.c_cc[VTIME] = 0;         /* no timeout */
    tcsetattr(0, TCSANOW, &info); /* set immediately */

    char *Rcar[4];
    char Rcar1[10] = "  ______\n";
    char Rcar2[14] = " /|_||_\\\\.__\n";
    char Rcar3[15] = "(   _    _ _\\\n";
    char Rcar4[15] = "='-(_)--(_)-'\n";
    Rcar[0] = &(Rcar1[0]);
    Rcar[1] = &(Rcar2[0]);
    Rcar[2] = &(Rcar3[0]);
    Rcar[3] = &(Rcar4[0]);

    char *Lcar[4];
    char Lcar1[] = "     ______  \n";
    char Lcar2[] = " __.//_||_|\\\n";
    char Lcar3[] = "/_ _    _   )\n";
    char Lcar4[] = "'-(_)--(_)-'=\n";
    Lcar[0] = &(Lcar1[0]);
    Lcar[1] = &(Lcar2[0]);
    Lcar[2] = &(Lcar3[0]);
    Lcar[3] = &(Lcar4[0]);

char *Lknight[15];
char Lknight0[] = "|\\             //\n";
char Lknight1[] = " \\\\           _!_\n";
char Lknight2[] = "  \\\\         /___\\\n";
char Lknight3[] = "   \\\\        [+++]\n";
char Lknight4[] = "    \\\\    _ _\\^^^/_\n";
char Lknight5[] = "     \\\\/ (    '-'  ( )\n";
char Lknight6[] = "     /( \\/ | {&}   /\\ \\\n";
char Lknight7[] = "       \\  / \\     / _> )\n";
char Lknight8[] = "        \"\"` >:::;-'`\"\"'-.\n";
char Lknight9[] = "            /:::/         \\\n";
char Lknight10[] = "           /  /||   {&}   |\n";
char Lknight11[] = "          (  / (\\         /\n";
char Lknight12[] = "          / /   \\'-.___.-'\n";
char Lknight13[] = "        _/ /     \\ \\\n";
char Lknight14[] = "       /___|    /___|\n";
Lknight[0] = &(Lknight0[0]);
Lknight[1] = &(Lknight1[0]);
Lknight[2] = &(Lknight2[0]);
Lknight[3] = &(Lknight3[0]);
Lknight[4] = &(Lknight4[0]);
Lknight[5] = &(Lknight5[0]);
Lknight[6] = &(Lknight6[0]);
Lknight[7] = &(Lknight7[0]);
Lknight[8] = &(Lknight8[0]);
Lknight[9] = &(Lknight9[0]);
Lknight[10] = &(Lknight10[0]);
Lknight[11] = &(Lknight11[0]);
Lknight[12] = &(Lknight12[0]);
Lknight[13] = &(Lknight13[0]);
Lknight[14] = &(Lknight14[0]);

char *Rknight[15];
char Rknight0[] = "           \\\\             /|\n";
char Rknight1[] = "           _!_           //\n";
char Rknight2[] = "          /___\\         //\n";
char Rknight3[] = "          [+++]        //\n";
char Rknight4[] = "         _\\^^^/_ _    //\n";
char Rknight5[] = "      ( )  '-'    ) \\//\n";
char Rknight6[] = "     / /\\   {&} | \\/ )\\\n";
char Rknight7[] = "    ( <_ \\     / \\  /\n";
char Rknight8[] = "   .-'\"\"`'-;:::< `\"\"\n";
char Rknight9[] = " /         \\:::\\\n";
char Rknight10[] = " |   {&}   ||\\  \\\n";
char Rknight11[] = " \\         /) \\  )\n";
char Rknight12[] = "  '-.___.-'/   \\ \\\n";
char Rknight13[] = "        / /     \\ \\_\n";
char Rknight14[] = "       |___\\    |___\\\n";
Rknight[0] = &(Rknight0[0]);
Rknight[1] = &(Rknight1[0]);
Rknight[2] = &(Rknight2[0]);
Rknight[3] = &(Rknight3[0]);
Rknight[4] = &(Rknight4[0]);
Rknight[5] = &(Rknight5[0]);
Rknight[6] = &(Rknight6[0]);
Rknight[7] = &(Rknight7[0]);
Rknight[8] = &(Rknight8[0]);
Rknight[9] = &(Rknight9[0]);
Rknight[10] = &(Rknight10[0]);
Rknight[11] = &(Rknight11[0]);
Rknight[12] = &(Rknight12[0]);
Rknight[13] = &(Rknight13[0]);
Rknight[14] = &(Rknight14[0]);

    
    int Direction;
    int X = XMAX/2;
    int Y = YMAX/2;
    int ind = 0;
    int ch;
    system("clear");
    for(ind=0;ind<(YMAX/2);ind++){
        printf("\n");
    }
    for(ind=0;ind<15;ind++){
        for(int i=0;i<(XMAX/2);i++){
            printf("\t");
        }
        printf("%s",Rknight[ind]);
    }
    while((ch = getchar()) != 27 /* ascii ESC */) {
        if (ch < 0) {
            if (ferror(stdin)) { /* there was an error... */ }
            clearerr(stdin);
            /* do other stuff */
        } else {
            /* some key OTHER than ESC was hit, do something about it? */
            if(ch=='d'){
                if(X!=XMAX){
                    X++;
                    Direction = 0;
                } 
            }else if(ch=='a'){
                if(X!=0){
                    X--;
                    Direction = 1;
                } 
            }else if(ch=='w'){
                if(Y>=0){
                    Y-=3;
                }
            }else if(ch=='s'){
                if(Y<=YMAX){
                   Y+=3;
                }
            }
            system("clear");
            for(int i=0; i<Y;i++){
                printf("\n");
            }            
            if(Direction==0){
                
                for(ind=0;ind<15;ind++){
                    for(int i=0;i<X;i++){
                        printf("\t");
                    }
                    printf("%s",Rknight[ind]);
                }
            }else{
            
                for(ind=0;ind<15;ind++){
                    for(int i=0;i<X;i++){
                        printf("\t");
                    }
                    printf("%s",Lknight[ind]);
                }                
            }


        }
    }
}