#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define world(rowi,coli) (*(world + ((rowi*COLS)+coli)))
#define byte char
#define RIGHT 0
#define LEFT 1
#define FOCUSRARITY 0.06711

int DEVMODE = 0;

struct terrains{
    struct biomes *Biomes;

    char path;
    char tilepath1[6][13];
    char tilepath2[6][13];
    char tilepath3[6][13];

    char mountain;
    char tilemountain1[6][13];
    char tilemountain2[6][13];
    char tilemountain3[6][13];

    char tree;
    char tiletree1[6][13];
    char tiletree2[6][13];
    char tiletree3[6][13];

    char playercharacter;
    char tileRPC[6][13];
    char tileLPC[6][13];

    char border;
    char tileborder[6][13];

    char encounter;
    char tileencounter[6][13];

    char HUD;
    
};
struct biomes{
    int num_biomes;
    struct biome *start; 
};
struct biome{
    char name[13];
    char element;
    struct biome *next;
};
struct tile{
    byte deflt;
    byte current;
    int randomizer;
};
struct PCstats{
    int level;
    char name[7];
    int X;
    int Y;
    int direction;;
};
struct gamedata{
    struct tile *world;
    struct PCstats *PCstats;
    struct terrains *terrains;
    int ROWS;
    int COLS;
    int EncounterChance;
    int Xfocus;
    int Yfocus;
    int zoom;
    int mode;
};
struct options{
    int num_options;
    struct option *list;
};
struct option{
    char name[24];
    void (*action)(struct gamedata *GameData);
    struct option *next;
};
struct fheader{
    int focuscount;
    struct focus *start;
};
struct focus{
    int x;
    int y;
    int length;
    char type;
    struct focus *next;
    struct vector *vector;
};
struct vector{
    int x;
    int y;
    struct vector *next;
};

char tilepath1[6][13] = {
                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                        {' ','.',' ',' ',' ',' ',' ',' ',' ',' ',' ','.',' '},
                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ',' ',' ','.',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','.',' ',' ',' ',' ',' ',' ',' '}}; 
char tilepath2[6][13] = {
                        {' ',' ',' ',' ',' ',' ','.',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','.',' '},
                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                        {' ',' ','.',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ',' ',' ',' ',' ','.',' ',' ',' '}};
char tilepath3[6][13] = {
                        {'.',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','.',' '},
                        {' ',' ',' ',' ',' ','.','.',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ','.',' ',' ',' ',' ',' ',' ',' ',' '}};        
char tiletree1[6][13] = {
                        {' ',' ',' ',' ',' ',' ','/','\\',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','/','v','v','\\',' ',' ',' ',' '},
                        {' ',' ',' ',' ','/','v','v','v','v','\\',' ',' ',' '},
                        {' ',' ',' ','/','v','v','v','v','v','v','\\',' ',' '},
                        {' ',' ',' ','/','v','v','v','v','v','v','\\',' ',' '},
                        {' ',' ',' ',' ',' ',' ','|','|',' ',' ',' ',' ',' '}};
char tiletree2[6][13] = {
                        {' ',' ',' ',' ',' ','/','\\',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ','/','v','v','\\',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ','/','v','v','\\',' ',' ',' ',' ',' '},
                        {' ',' ',' ','/','v','v','v','v','\\',' ',' ',' ',' '},
                        {' ',' ',' ','/','v','v','v','v','\\',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','|','|',' ',' ',' ',' ',' ',' '}};
char tiletree3[6][13] = {
                        {' ',' ',' ',' ',' ',' ','/','\\',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','/','v','v','\\',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','/','v','v','\\',' ',' ',' ',' '},
                        {' ',' ',' ',' ','/','v','v','v','v','\\',' ',' ',' '},
                        {' ',' ',' ',' ',' ',' ','|','|',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ',' ','|','|',' ',' ',' ',' ',' '}};

char tilemountain1[6][13] = {
                        {' ',' ',' ',' ',' ','/','\\',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ','/','^',' ',' ','\\',' ',' ',' ',' ',' '},
                        {' ','/','^','/','^','^','\\',' ','^','\\',' ',' ',' '},
                        {'|',' ','/','^',' ',' ',' ',' ','\\',' ','\\',' ',' '},
                        {'|','|',' ','/',' ',' ',' ','/','^','\\',' ','^','\\'},
                        {'/',' ',' ',' ','/','^','^',' ',' ',' ','\\',' ','|'}};        
char tilemountain2[6][13] = {
                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ',' ','/','\\',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','/','^','^','\\',' ',' ',' ',' '},
                        {' ',' ','/','^','^','/','\\',' ',' ','\\',' ',' ',' '},
                        {'/','^',' ',' ',' ',' ','^','^','\\',' ','^','\\',' '},
                        {'|',' ','/','^',' ',' ',' ',' ',' ','^','^','|','\\'}};                                 
char tilemountain3[6][13] = {
                        {' ',' ',' ',' ',' ',' ',' ',' ','/','\\',' ',' ',' '},
                        {' ',' ',' ','/','\\',' ','/','^',' ',' ','\\',' ',' '},
                        {' ',' ','/',' ',' ','^','^','\\',' ',' ','/','\\',' '},
                        {' ','|','/','\\',' ',' ',' ','/','\\','^',' ',' ','\\'},
                        {'/','^',' ',' ','\\',' ','/','^','^','^','\\',' ','|'},
                        {'|',' ',' ',' ',' ','|',' ',' ',' ',' ',' ','^','\\'}};                           
char tileRPC[6][13] = {
                        {' ',' ',' ',' ',' ',' ','_',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','(','=',')','>',' ',' ',' ',' '},
                        {' ',' ',' ',' ','O','\\',' ','/','O',' ',' ','/',' '},
                        {' ',' ',' ',' ','\\','*','=','*',' ','\\','/',' ',' '},
                        {' ',' ',' ',' ',' ','/','|','\\',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','|',' ','|',' ',' ',' ',' ',' '}};
char tileLPC[6][13] = {
                        {' ',' ',' ',' ',' ',' ','_',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ','<','(','=',')',' ',' ',' ',' ',' '},
                        {' ','\\',' ',' ','O','\\',' ','/','O',' ',' ',' ',' '},
                        {' ',' ','\\','/',' ','*','=','*','/',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','/','|','\\',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','|',' ','|',' ',' ',' ',' ',' '}};   
char tileencounter[6][13] = {
                        {' ',' ',' ',' ',' ',' ','_',' ',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','|',' ','|',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','|',' ','|',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','|',' ','|',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','|','_','|',' ',' ',' ',' ',' '},
                        {' ',' ',' ',' ',' ','(','_',')',' ',' ',' ',' ',' '}};      
char tileborder[6][13] = {
                        {'|','|','_','_','_','|','_','_','_','|','_','_','|'},
                        {'|','_','_','|','_','_','_','|','_','_','_','|','|'},
                        {'|','|','_','_','_','|','_','_','_','|','_','_','|'},
                        {'|','_','_','|','_','_','_','|','_','_','_','|','|'},
                        {'|','|','_','_','_','|','_','_','_','|','_','_','|'},
                        {'|','_','_','|','_','_','_','|','_','_','_','|','|'}};                                      
//Tile size 13w x 6t




void LoadOptions(struct options *Options);
void NewWorldOptions(struct gamedata *GameData);

void CreateTerrains(struct gamedata *GameData);
void CreateNewWorld(struct gamedata *GameData);

void GenerateTiling(struct tile *world, int ROW, int COL);
void GenerateTerrain(struct tile *world, struct terrains *terrains, int ROW, int COL);
void GenerateEncounters(struct tile *world, struct terrains *terrains, int ROW, int COL, int EncounterChance);

void InitializeWorld(struct tile *world, struct terrains *terrains, int ROW, int COL);
void InitializePlayer(struct PCstats *PCstats, int ROWS, int COLS);



void moveplayercharacter(struct gamedata *GameData, int move);

int Startup(struct options *Options, struct gamedata *GameData);
int mainmenu(struct options *Options, struct gamedata *GameData);

void PrintGame(struct gamedata *GameData);
void PrintDemo(struct gamedata *GameData);
void PrintMap(struct gamedata *GameData);

void GameWorld(struct gamedata *GameData);
void DemoWorld(struct gamedata *GameData);

void Exit(struct gamedata *GameData);
void Dev(struct gamedata *GameData);

void main(struct tile *world){
    struct termios info;
    tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
    info.c_lflag &= ~ICANON;      /* disable canonical mode */
    info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
    info.c_cc[VTIME] = 0;         /* no timeout */
    tcsetattr(0, TCSANOW, &info); /* set immediately */

    struct options *Options;
    Options = (struct options *)malloc(sizeof(struct options));
    LoadOptions(Options);

    struct gamedata *GameData = (struct gamedata *)malloc(sizeof(struct gamedata));
    GameData->world = NULL;
    GameData->terrains = NULL;
    GameData->PCstats = NULL;

    int exit;
    exit = Startup(Options, GameData);
    while(exit==0){
        exit = mainmenu(Options,GameData);
    }
}
void LoadOptions(struct options *Options){
    Options->num_options = 6;

    if(Options->num_options>0){
        struct option *New;
        New = (struct option *)malloc(sizeof(struct option));
        struct option *Previous;

        Options->list = New;
        New->next = NULL;

        for(int i=0; i<(Options->num_options)-1; i++){
            Previous = New;
            New = (struct option *)malloc(sizeof(struct option));
            Previous->next = New;
        }
        New->next = NULL;
    }

    struct option *Option;
    Option = Options->list;

    strcpy(Option->name,"Create New World");
    Option->action = &CreateNewWorld;
    Option = Option->next;

    strcpy(Option->name,"Enter World");
    Option->action = &GameWorld;
    Option = Option->next;

    strcpy(Option->name,"View Map");
    Option->action = &PrintMap;
    Option = Option->next;

    strcpy(Option->name,"Enter Demo");
    Option->action = &DemoWorld;
    Option = Option->next;

    strcpy(Option->name,"Dev Mode");
    Option->action = &Dev;
    Option = Option->next;

    strcpy(Option->name,"Quit");
    Option->action = &Exit;
    Option = Option->next;
}
void CreateTerrains(struct gamedata *GameData){
    if(GameData->terrains==NULL){
        struct terrains *terrains;
        terrains = (struct terrains *)malloc(sizeof(struct terrains));
        GameData->terrains = terrains;

        terrains->path = ' ';
        terrains->mountain = '#';
        terrains->tree = '^';
        terrains->playercharacter = '$';
        terrains->border = 'X';
        terrains->encounter = '!';
        terrains->HUD = '@';

        struct biomes *Biomes = (struct biomes *)malloc(sizeof(struct biomes));
        terrains->Biomes = Biomes;
        Biomes->num_biomes = 2;
        int i;
        struct biome *Biome;
        struct biome *temp;
        Biome = (struct biome *)malloc(sizeof(struct biome));
        Biomes->start = Biome;
        for(i=1;i<Biomes->num_biomes;i++){
            temp = Biome;
            Biome = (struct biome *)malloc(sizeof(struct biome));
            temp->next = Biome;
        }
        Biome->next = NULL;
        Biome = Biomes->start;

        strcpy(Biome->name,"forest");
        Biome->element = terrains->tree;
        Biome = Biome->next;

        strcpy(Biome->name,"mountains");
        Biome->element = terrains->mountain;
        Biome = Biome->next;
    }
}
void CreateNewWorld(struct gamedata *GameData){

    if(GameData->world!=NULL){
        free(GameData->world);
    }
    if(GameData->PCstats!=NULL){
        free(GameData->PCstats);
    }
    NewWorldOptions(GameData);

    int ROWS;
    ROWS = GameData->ROWS;
    int COLS;
    COLS = GameData->COLS;

    struct tile *world;
    world = (struct tile *)malloc(sizeof(struct tile)*(ROWS*COLS));
    GameData->world = world;
    
    CreateTerrains(GameData);
    
    GameData->mode = 0;
    GameData->zoom = 5;

    struct terrains *terrains;
    terrains = GameData->terrains;
    int EncounterChance;
    EncounterChance = GameData->EncounterChance;

    struct PCstats *PCstats;
    PCstats = (struct PCstats *)malloc(sizeof(PCstats));
    GameData->PCstats = PCstats;


    GenerateTiling(world, ROWS, COLS);
    GenerateTerrain(world, terrains, ROWS, COLS);
    InitializeWorld(world, terrains, ROWS, COLS);
    InitializePlayer(PCstats, ROWS, COLS);
    GenerateEncounters(world, terrains, ROWS, COLS, EncounterChance);
}
void GenerateTiling(struct tile *world, int ROWS, int COLS){
    int i,j;
    int temp;
    for(i=0;i<ROWS;i++){
        for(j=0;j<COLS;j++){
            temp = rand() % (6) + 1;
            world(i,j).randomizer = temp;
        }
    }
}
void GenerateTerrain(struct tile *world, struct terrains *terrains, int ROWS, int COLS){
    /***start path initialization***/
    /*
    This section simply assigns every tile of
    the world map to be terrain of type "path",
    which is essentially to say it makes the entire
    map a flat walkable area.
    */
    int i,j,k;
    for(i=0;i<ROWS;i++){
        for(j=0;j<COLS;j++){
            world(i,j).deflt = terrains->path;
        }
    }
    /***end path initializaion***/



    /***start biome focus generation***/
    /*
    This is rather complicated, so be sure to read carefully.
    This section will create a linked list of focuses, where
    each focus has a coordinate on the map that will represent
    the start of a biome. The map is assigned a magnitude, 
    which is defined to be the diagonal distance from the top 
    left corner to the bottom right corner. A number of focuses
    will be generated equal to a constant multiplier of the magnitude,
    which is defined as "FOCUSRARITY". This number will be very small.

    A focus is restricted to the bounds of the map, as well as having to
    be a distance further than 1/4 of the maps magitude away from any
    other focus. This is to ensure adequate biome seperation. The
    algorithm will attempt to generate the number of desired focuses for
    the map type, but if it gets softlocked into not being able to
    fit another focus into the map given the restrictions, it will
    simply give up and set update the focus count.
    */
    struct fheader *Focuses = (struct fheader *)malloc(sizeof(struct fheader));
    Focuses->focuscount = sqrt(pow(ROWS,2) + pow(COLS,2)) * FOCUSRARITY;
    
    struct focus *rover = (struct focus *)malloc(sizeof(struct focus));
    struct focus *temp;
    
    Focuses->start = rover;
    struct focus *check;

    
    rover->x = rand() % COLS;
    rover->y = rand() % ROWS;
    world((rover->y),(rover->x)).deflt = '0';



    float magnitude = .25 * sqrt(pow(ROWS,2)+pow(COLS,2));
    rover->length = .5 * magnitude;
    
    if(DEVMODE==1){
        printf("focus count %d\n",Focuses->focuscount);
        printf("magnitude %f\n",magnitude);
        printf("vector length %d\n",rover->length);        
        printf("focus 0 at [%d,%d]\n",rover->x,rover->y);
    }
    float distance;
    int flag;
    int iterations;
    int timeout = 0;
    for(i=1;i<Focuses->focuscount;i++){
        temp = rover;
        rover = (struct focus *)malloc(sizeof(struct focus));
        rover->length = .5 * magnitude;
        temp->next = rover;

        flag = 0;
        iterations = 0;
        while(flag==0){
            iterations++;
            
            flag = 1;
            rover->x = rand() % (COLS - 1) + 1;
            rover->y = rand() % (ROWS - 1) + 1;
            check = Focuses->start;
            for(j=0;j<i;j++){
                distance = sqrt(pow((rover->x)-(check->x),2) + pow((rover->y)-(check->y),2));
                if(distance<magnitude){
                    flag=0;
                }            
                check = check->next;
            }
            if(iterations>=1000){
                free(rover);
                temp->next = NULL;
                int k;
                k = i;
                i = Focuses->focuscount;
                Focuses->focuscount = k;
                timeout = 1;
                flag=1;
            }
        }
        if(timeout==0){
            if(DEVMODE==1){
                printf("focus %d at [%d,%d]\n",i, rover->x,rover->y);
            }
            world((rover->y),(rover->x)).deflt = i+0x30;
        }
    }
    if(timeout==0){
        rover->next = NULL;
    }
    if(DEVMODE==1){
        printf("print map?(y)\n");
        i = getchar();
        if(i=='y'){
            for(i=0;i<ROWS;i++){
                for(j=0;j<COLS;j++){
                    printf("%c",world(i,j).deflt);
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("press key to continue\n");
        getchar();
    }
    /***end focus generation***/



    /***start vector generation***/
    /*
    This section creates a vector train starting from each
    of the focus points. The focuses will create vector
    trains of a with a number of vectors equal to "length",
    which is 1/8 the length of the maps diagonal magnitude.
    The distance between each vector in the train is between 
    is decided by the number of vectors in the train, which
    is in turn decided by the size of the map.

    A vector is place a "length" distance away from the
    previous node in the train. There is a limited randomness
    to the generation and the direction from the previous
    vector is completely random, so long as it is within the
    bounds of the map. Each vector in the train is connect in
    memory as a node of a linked list.
    */
    int vectx,vecty;
    rover = Focuses->start;
    struct vector *Vector = NULL;
    struct vector *vecrov = NULL;
    for(j=0;j<Focuses->focuscount;j++){
        rover->vector = NULL;
        int xmax = 1/(sqrt(5/4)) * (rover->length);
        int ymax = 1/(sqrt(5)) * (rover->length);
        if(DEVMODE==1){
            printf("source [%d,%d]\n",rover->x,rover->y);
        }
        for(i=0;i<rover->length;i++){
            vecrov = Vector;
            Vector = (struct vector *)malloc(sizeof(struct vector));
            Vector->x = COLS+1;
            Vector->y = ROWS+1;
            while(Vector->x>=COLS||Vector->x<=0||Vector->y>=ROWS||Vector->y<=0){
                vectx = rand() % (xmax - ((1/4) * xmax) + 1) + ((1/4) * xmax);
                vecty = rand() % (ymax - ((1/4) * ymax) + 1) + ((1/4) * ymax);
                if(rover->vector==NULL){
                    if(rand() % 2){
                        Vector->x = rover->x + vectx;
                    }else{
                        Vector->x = rover->x - vectx;
                    }
                    if(rand() % 2){
                        Vector->y = rover->y + vecty;
                    }else{
                        Vector->y = rover->y - vecty;
                    }
                }else{
                    if(rand() % 2){
                        Vector->x = vecrov->x + vectx;
                    }else{
                        Vector->x = vecrov->x - vectx;
                    }
                    if(rand() % 2){
                        Vector->y = vecrov->y + vecty;
                    }else{
                        Vector->y = vecrov->y - vecty;
                    }                
                }
            }
            if(DEVMODE==1){
                printf("vector %d [%d,%d]\n",i, Vector->x,Vector->y);
            }
            world((Vector->y),(Vector->x)).deflt = 'V';
            if(rover->vector==NULL){
                rover->vector = Vector;
            }else{
                vecrov->next = Vector;
            }
        }
        Vector->next = NULL;
        rover = rover->next;
        if(DEVMODE==1){
            printf("press key to continue\n");
            getchar();
        }
    }
    if(DEVMODE==1){
        printf("print map?(y)\n");
        i = getchar();
        if(i=='y'){
            for(i=0;i<ROWS;i++){
                for(j=0;j<COLS;j++){
                    printf("%c",world(i,j).deflt);
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("press key to continue\n");
        getchar();
    }
    /***end vector generation ***/

    

    /***start biome generation***/
    if(DEVMODE==1){
        printf("\n\nfocus count %d\n",Focuses->focuscount);
    }
    rover = Focuses->start;
    struct biome *terrov;
    int rnum;
    if(DEVMODE==1){
        printf("assigning biomes\n");
    }
    for(i=0;i<Focuses->focuscount;i++){
        terrov = terrains->Biomes->start;
        rnum = rand() % terrains->Biomes->num_biomes;
        for(j=0;j<rnum;j++){
            terrov = terrov->next;
        }
        if(DEVMODE==1){
            printf("biome %d is %s\n",i,terrov->name);
        }
        rover->type = terrov->element;
        rover = rover->next;
    }
    rover = Focuses->start;

    struct vector *tempV;
    int diffx;
    int diffy;
    float iscale = 1/((float)(rover->length));
    float scale = 1/((float)(rover->length));
    int newx;
    int newy;

    for(i=0;i<Focuses->focuscount;i++){
        scale = 1/((float)(rover->length));
        Vector = rover->vector;
        world((rover->y),(rover->x)).deflt = rover->type;
        world((Vector->y),(Vector->x)).deflt = rover->type;

        diffx = rover->x - Vector->x;
        diffy = rover->y - Vector->y;
        if(DEVMODE==1){
            printf("focus %d start at %d,%d\n",i,rover->x,rover->y);
            printf("vector len %d\n",rover->length);
            printf("difference %d,%d\n",diffx,diffy);
            printf("scale %f\n",scale);
        }

        for(j=0;j<rover->length;j++){
            newx = Vector->x + (diffx*scale);
            newy = Vector->y + (diffy*scale);
            world(newy,newx).deflt = rover->type;

            scale = scale + iscale;
        }
        if(DEVMODE==1){
            printf("first line drawn\n");
        }
        for(k=1;k<rover->length;k++){
            tempV = Vector;
            Vector = Vector->next;
            world((Vector->y),(Vector->x)).deflt = rover->type;
            scale = iscale;
            diffx = tempV->x - Vector->x;
            diffy = tempV->y - Vector->y;
            if(DEVMODE==1){
                printf("difference %d,%d\n",diffx,diffy);
            }
            for(j=0;j<rover->length;j++){
                newx = Vector->x + (diffx*scale);
                newy = Vector->y + (diffy*scale);
                world(newy,newx).deflt = rover->type;

                scale = scale + iscale;
            }
            if(DEVMODE==1){
                printf("line drawn\n");
            }
        }
        if(DEVMODE==1){
            printf("print map?(y)\n");
            k = getchar();
            if(k=='y'){
                for(k=0;k<ROWS;k++){
                    for(j=0;j<COLS;j++){
                        printf("%c",world(k,j).deflt);
                    }
                    printf("\n");
                }
                printf("\n");
            }
            printf("press key to continue\n");
            getchar();
        }
        rover = rover->next;
        if(DEVMODE==1){
            printf("rover inc\n");
        }
    }
    /***end biome generation***/



    /***start memory deallocation***/
    struct vector *vrem;
    struct focus *frem;
    rover = Focuses->start;
    for(i=0;i<Focuses->focuscount;i++){
        frem = rover;
        vecrov = rover->vector;
        for(j=0;j<rover->length;j++){
            vrem = vecrov;
            vecrov = vecrov->next;
            free(vrem);
        }
        rover = rover->next;
        free(frem);
    }
    free(Focuses);
    /***end memory deallocation***/



    /***start border overlay***/
    /*
    This section overlays a border around the edges of the map,
    this is done by simply going to each edge as specified by
    the world header and assigning that length the terrain type
    "border".
    */
    for(i=0;i<COLS;i++){
        world(0,i).deflt = terrains->border;
    }
    for(i=0;i<COLS;i++){
        world((ROWS-1),i).deflt = terrains->border;
    }
    for(i=0;i<ROWS;i++){
        world(i,0).deflt = terrains->border;
    }
    for(i=0;i<ROWS;i++){
        world(i,(COLS-1)).deflt = terrains->border;
    }
    /***end border overlay***/
}
void InitializeWorld(struct tile *world, struct terrains *terrains, int ROWS, int COLS){
    int i,j;
    for(i=0;i<ROWS;i++){
        for(j=0;j<COLS;j++){
            world(i,j).current = world(i,j).deflt;
        }
    }
    world((ROWS/2),(COLS/2)).current = terrains->playercharacter;
}
void InitializePlayer(struct PCstats *PCstats, int ROWS, int COLS){
    PCstats->direction = RIGHT;
    PCstats->level = 0;
    strcpy(PCstats->name,"JohnDoe");
    PCstats->X = COLS/2;
    PCstats->Y = ROWS/2;
}
void GenerateEncounters(struct tile *world, struct terrains *terrains, int ROWS, int COLS, int EncounterChance){
    int randx;
    int randy;
    int Encounters = 0;
    int Num_Encounter;
    Num_Encounter = ROWS*COLS*EncounterChance/100;
    while (Encounters<Num_Encounter){
        randx = rand() % COLS;
        randy = rand() % ROWS;
        if(world(randy,randx).current==terrains->path){
            world(randy,randx).current = terrains->encounter;
            Encounters++;
        }
    }
}
void PrintMap(struct gamedata *GameData){
    if(GameData->world!=NULL){
        int ROWS = GameData->ROWS;
        int COLS = GameData->COLS;
        struct tile *world = GameData->world;
        int X = GameData->PCstats->X;
        int Y = GameData->PCstats->Y;
        int i,j;
        system("clear");
        for(i=0;i<ROWS;i++){
            for(j=0;j<COLS;j++){
                printf("%c",world(i,j).current);
            }
            printf("\n");
        }
        printf("\n");
        printf("X==%d Y==%d\n",X,Y); 
        printf("%d, %d\n",ROWS,COLS);
        getchar();
    }else{
        printf("\nERROR!\n--please create a world\n");
        sleep(2);
    }
}
void PrintDemo(struct gamedata *GameData){
    int X = GameData->PCstats->X;
    int Y = GameData->PCstats->Y;
    int COLS = GameData->COLS;
    int ROWS = GameData->ROWS;
    int zoom = GameData->zoom;
    struct tile *world = GameData->world;
    
    int i,j;
    int tempx,tempy;
    
    int xscale = zoom;
    int yscale = xscale/2;

    if(X+xscale>COLS-1){
        tempx = COLS-xscale-1;
    }else if(X-xscale<0){
        tempx = 0 + xscale;
    }else{
        tempx = X;
    }

    if(Y+yscale>ROWS-1){
        tempy = ROWS-yscale-1;
    }else if(Y-yscale<0){
        tempy = 0 + yscale;
    }else{
        tempy = Y;
    }

    for(i=tempy-yscale;i<tempy+yscale+1;i++){
        for(j=tempx-xscale;j<tempx+xscale+1;j++){
            printf("%c",world(i,j).current);
        }
        printf("\n");
    }
    printf("\n");
    printf("X==%d Y==%d\n",X,Y);
    printf("Zoom == %d\n",zoom); 
}
void PrintGame(struct gamedata *GameData){
    int X = GameData->PCstats->X;
    int Y = GameData->PCstats->Y;
    int COLS = GameData->COLS;
    int ROWS = GameData->ROWS;
    int zoom = GameData->zoom;
    struct tile *world = GameData->world;

    int i,j,k,l;
    int tempx,tempy;
    
    int xscale = zoom;
    int yscale = xscale/2;

    if(X+xscale>COLS-1){
        tempx = COLS-xscale-1;
    }else if(X-xscale<0){
        tempx = 0 + xscale;
    }else{
        tempx = X;
    }

    if(Y+yscale>ROWS-1){
        tempy = ROWS-yscale-1;
    }else if(Y-yscale<0){
        tempy = 0 + yscale;
    }else{
        tempy = Y;
    }

    for(i=tempy-yscale;i<(tempy+yscale+1);i++){//for each tile row
        for(k=0;k<6;k++){//for each row of tile row
            for(j=tempx-xscale;j<(tempx+xscale+1);j++){//for each tile column
                if(world(i,j).current=='^'){
                    if(world(i,j).randomizer==1){
                        for(l=0;l<13;l++){//for each tile of column
                            printf("%c",tiletree3[k][l]);
                        }    
                    }else if(world(i,j).randomizer==2){
                        for(l=0;l<13;l++){//for each tile of column
                            printf("%c",tiletree2[k][l]);
                        }                          
                    }else{
                        for(l=0;l<13;l++){//for each tile of column
                            printf("%c",tiletree1[k][l]);
                        }                          
                    }
                    
                }else if(world(i,j).current=='#'){
                    if(world(i,j).randomizer==1){
                        for(l=0;l<13;l++){//for each tile of column
                            printf("%c",tilemountain3[k][l]);
                        } 
                    }else if(world(i,j).randomizer==2){
                        for(l=0;l<13;l++){//for each tile of column
                            printf("%c",tilemountain2[k][l]);
                        }                         
                    }else{
                        for(l=0;l<13;l++){//for each tile of column
                            printf("%c",tilemountain1[k][l]);
                        }                         
                    }
 
                }else if(world(i,j).current=='$'){
                    if(GameData->PCstats->direction==RIGHT){
                        for(l=0;l<13;l++){//for each tile of column
                            printf("%c",tileRPC[k][l]);
                        } 
                    }else{
                        for(l=0;l<13;l++){//for each tile of column
                            printf("%c",tileLPC[k][l]);
                        } 
                    }
 
                }else if(world(i,j).current==' '){
                    if(world(i,j).randomizer==1){
                        for(l=0;l<13;l++){//for each tile of column
                            printf("%c",tilepath3[k][l]);
                        }  
                    }else if(world(i,j).randomizer==2){
                        for(l=0;l<13;l++){//for each tile of column
                            printf("%c",tilepath2[k][l]);
                        }  
                    }else{
                        for(l=0;l<13;l++){//for each tile of column
                            printf("%c",tilepath1[k][l]);
                        }                          
                    }

                }else if(world(i,j).current=='!'){
                    for(l=0;l<13;l++){//for each tile of column
                        printf("%c",tileencounter[k][l]);
                    }  
                }else if(world(i,j).current=='X'){
                    for(l=0;l<13;l++){//for each tile of column
                        printf("%c",tileborder[k][l]);
                    }  
                }else if(world(i,j).current=='@'){
                    if(k==0){
                        printf("level: %02d    ",GameData->PCstats->level);
                    }else if(k==1){
                        printf("name: %s",GameData->PCstats->name);
                    }else if(k==2){
                        printf("X pos: %02d    ",GameData->PCstats->X);
                    }else if(k==3){
                        printf("Y pos: %02d    ",GameData->PCstats->Y);
                    }else{
                        printf("             ");
                    }
                    

                }else{
                    for(l=0;l<13;l++){//for each tile of column
                        printf("%c",world(i,j).current);
                    }                       
                }
            }
            printf("\n");
        } 
    }
    printf("\n");
}
int Startup(struct options *Options, struct gamedata *GameData){

    system("clear");
    printf("__        __   _                          \n");
    printf("\\ \\      / /__| | ___ ___  _ __ ___   ___ \n");
    printf(" \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\\n");
    printf("  \\ V  V /  __/ | (_| (_) | | | | | |  __/\n");
    printf("   \\_/\\_/_\\___|_|\\___\\___/|_| |_| |_|\\___|\n");
    sleep(1);
    printf("        |_   _|__                         \n");
    printf("          | |/ _ \\                        \n");
    printf("          | | (_) |                       \n");
    printf("          |_|\\___/                        \n");
    sleep(1);

    return mainmenu(Options, GameData);
}
int mainmenu(struct options *Options, struct gamedata *GameData){

    system("clear");
    printf("  _____ _____  ______ _____    ____  _    _ ______  _____ _______ \n");
    printf(" / ____|  __ \\|  ____/ ____|  / __ \\| |  | |  ____|/ ____|__   __|\n");
    printf("| |  __| |__) | |__ | |  __  | |  | | |  | | |__  | (___    | |   \n");
    printf("| | |_ |  _  /|  __|| | |_ | | |  | | |  | |  __|  \\___ \\   | |   \n");
    printf("| |__| | | \\ \\| |___| |__| | | |__| | |__| | |____ ____) |  | |   \n");
    printf(" \\_____|_|  \\_\\______\\_____|  \\___\\_\\_____/|______|_____/   |_|   \n");
    printf("\n\t [OPTIONS]\n");
    printf("\t===========\n");

    int num_options = Options->num_options;
    struct option *Option = Options->list;

    int i;
    for(i=0;i<num_options;i++){
        printf("\t%d) %s\n", i+1, Option->name);
        Option = Option->next;
    }
    if(DEVMODE==1){
        printf("***devmode enabled\n");
    }

    char userinput;  
    int flag = 0;

    while(flag==0) {
        userinput = getchar();
        if (userinput < 0) {
            if (ferror(stdin));
            clearerr(stdin);
        }else if((userinput-0x30)>0&&(userinput-0x30)<=num_options){
            flag = 1;
        }else if(userinput==27){
            return 1;
        }
    }
    Option = Options->list;
    for(i=0;i<(userinput-0x30-1);i++){
        Option = Option->next;
    }

    Option->action(GameData);
    return 0;
}
void NewWorldOptions(struct gamedata *GameData){
    int flag;
    char userinput;
    system("clear");
    printf("Please Select a Size\n");
    printf("====================\n");
    printf("\t1) 20x40\n");
    printf("\t2) 40x80\n");
    printf("\t3) 60x120\n");
    flag = 0;
    while(flag==0) {
        userinput = getchar();
        if (userinput < 0) {
            if (ferror(stdin));
            clearerr(stdin);
        }else if((userinput-0x30)>0&&(userinput-0x30)<=3){
            flag = 1;
        }
    }
    
    if(userinput=='1'){
        GameData->ROWS = 20;
        GameData->COLS = (GameData->ROWS)*2;
    }else if(userinput=='2'){
        GameData->ROWS = 40;
        GameData->COLS = (GameData->ROWS)*2;        
    }else{
        GameData->ROWS = 60;
        GameData->COLS = (GameData->ROWS)*2;        
    }
    system("clear");
    printf("Select an Encounter Frequency\n");
    printf("=============================\n");
    printf("\t 1) None\n");
    printf("\t 2) Few\n");
    printf("\t 3) Common\n");
    printf("\t 4) Frequent\n");
    flag = 0;
    while(flag==0){
        userinput = getchar();
        if (userinput < 0) {
            if (ferror(stdin));
            clearerr(stdin);
        }else if((userinput-0x30)>0&&(userinput-0x30)<=4){
            flag = 1;
        }
    }
    if(userinput=='1'){
        GameData->EncounterChance = 0;
    }else if(userinput=='2'){
        GameData->EncounterChance = 0.1;       
    }else if(userinput=='3'){
        GameData->EncounterChance = 1;        
    }else{
        GameData->EncounterChance = 10;
    }
}

void moveplayercharacter(struct gamedata *GameData, int move){
    struct tile *world = GameData->world;
    int X = GameData->PCstats->X;
    int Y = GameData->PCstats->Y;
    int COLS = GameData->COLS;

    if(move!=0){
        world(Y,X).current = world(Y,X).deflt;
        world((Y-2),(X+5)).current = world((Y-2),(X+5)).deflt;
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
    GameData->PCstats->X = X;
    GameData->PCstats->Y = Y;
    world((Y-2),(X+5)).current = GameData->terrains->HUD;
    world(Y,X).current = GameData->terrains->playercharacter;
}

void GameWorld(struct gamedata *GameData){
    if(GameData->world!=NULL){
        int X = GameData->PCstats->X;
        int Y = GameData->PCstats->Y;
        int COLS = GameData->COLS;
        int ROWS = GameData->ROWS;
        struct tile *world = GameData->world;
        
        char userinput;
        int move = 0;
        moveplayercharacter(GameData,move);

        int exit = 0;
        while(exit==0){
            system("clear");
            PrintGame(GameData);
            userinput = getchar();
            if(userinput < 0){
                if(ferror(stdin));
                clearerr(stdin);
            }else{
                X = GameData->PCstats->X;
                Y = GameData->PCstats->Y;
                move = 0;
                if(userinput=='d'){
                    if(X!=COLS-1&&(world(Y,(X+1)).current)==' '){
                        move = 1;
                        GameData->PCstats->direction = RIGHT;
                    }
                }else if(userinput=='a'){
                    if(X!=0&&(world(Y,(X-1)).current)==' '){
                        move = 2;
                        GameData->PCstats->direction = LEFT;
                    }
                }else if(userinput=='w'){
                    if(Y!=0&&(world((Y-1),X).current)==' '){
                        move = 3;
                    }
                }else if(userinput=='s'){
                    if(Y!=ROWS-1&&(world((Y+1),X).current)==' '){
                    move = 4;
                    }
                }else if(userinput=='-'){
                    if(!((GameData->zoom)*2+1>=COLS-1)){
                        GameData->zoom = GameData->zoom + 1;
                    }
                }else if(userinput=='+'){
                    if((GameData->zoom)>5){
                        GameData->zoom = GameData->zoom - 1;
                    }
                }else if(userinput==27){
                    exit = 1;
                }     
                moveplayercharacter(GameData,move);   
            }
        }
    }else{
        printf("\nERROR!\n--please create a world\n");
        sleep(2);
    }
}

void DemoWorld(struct gamedata *GameData){
    if(GameData->world!=NULL){
        int X = GameData->PCstats->X;
        int Y = GameData->PCstats->Y;
        int COLS = GameData->COLS;
        int ROWS = GameData->ROWS;
        struct tile *world = GameData->world;
        
        char userinput;
        int move = 0;
        moveplayercharacter(GameData,move);

        int exit = 0;
        while(exit==0){
            system("clear");
            PrintDemo(GameData);
            userinput = getchar();
            if(userinput < 0){
                if(ferror(stdin));
                clearerr(stdin);
            }else{
                X = GameData->PCstats->X;
                Y = GameData->PCstats->Y;
                move = 0;
                if(userinput=='d'){
                    if(X!=COLS-1&&(world(Y,(X+1)).current)==' '){
                        move = 1;
                        GameData->PCstats->direction = RIGHT;
                    }
                }else if(userinput=='a'){
                    if(X!=0&&(world(Y,(X-1)).current)==' '){
                        move = 2;
                        GameData->PCstats->direction = LEFT;
                    }
                }else if(userinput=='w'){
                    if(Y!=0&&(world((Y-1),X).current)==' '){
                        move = 3;
                    }
                }else if(userinput=='s'){
                    if(Y!=ROWS-1&&(world((Y+1),X).current)==' '){
                    move = 4;
                    }
                }else if(userinput=='-'){
                    if(!((GameData->zoom)*2+1>=COLS-1)){
                        GameData->zoom = GameData->zoom + 1;
                    }
                }else if(userinput=='+'){
                    if((GameData->zoom)>5){
                        GameData->zoom = GameData->zoom - 1;
                    }
                }else if(userinput==27){
                    exit = 1;
                }     
                moveplayercharacter(GameData,move);   
            }
        }
    }else{
        printf("\nERROR!\n--please create a world\n");
        sleep(2);
    }
}
void Exit(struct gamedata *GameData){
    system("clear");
    exit(0);
}
void Dev(struct gamedata *GameData){
    if(DEVMODE==0){
        DEVMODE = 1;
    }else{
        DEVMODE = 0;
    }
}