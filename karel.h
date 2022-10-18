// #ifdef _WIN32
// #include <windows.h>
// #else
// #include <unistd.h>
// #endif

// ms sleep function from https://stackoverflow.com/a/28827188/15595275
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h> // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void msleep(int milliseconds)
{ // cross-platform sleep function
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    if (milliseconds >= 1000)
        sleep(milliseconds / 1000);
    usleep((milliseconds % 1000) * 1000);
#endif
}

#ifndef KAREL_H
#define KAREL_H
#include <stdio.h>   //input output
#include <stdlib.h>  //idk
#include <unistd.h>  //time/milliseconds sleep
#include <stdbool.h> //boolean
#include <string.h>  //strings
//#include <ncurses.h> //color etc output

// COLORS
// basic colors
#define ac_fgRED "\x1b[31;40m"
#define ac_fgGREEN "\x1b[32;40m"
#define ac_fgYELLOW "\x1b[33;40m"
#define ac_fgBLUE "\x1b[34;40m"
#define ac_fgMAGENTA "\x1b[35;40m"
#define ac_fgCYAN "\x1b[36;40m"
#define ac_RESET "\x1b[0;0m"
// colors for elements
#define ac_outerWall "\x1b[34;40m"
#define ac_innerWall "\x1b[96;40m"
#define ac_markerr "\x1b[96;40m"
#define ac_karel "\x1b[35;40m"
#define ac_karelWithBeep "\x1b[95;40m"
#define ac_beeper "\x1b[33;40m"
#define ac_space "\x1b[30;40m"
#define ac_dot "\x1b[37;40m"
#define ac_speed "\x1b[91;40m"

//  extern all def
extern void run();
extern void setup();
extern void loadWorld();
extern void setSpeed(); // time in seconds between frames

// extern shit def
// move
extern void move();
// direction
extern void turnLeft();
// beeper stuff
extern void putBeeper();
extern void pickBeeper();
// Clear checks
extern bool frontIsClear();
extern bool frontIsBlocked();
extern bool leftIsClear();
extern bool leftIsBlocked();
extern bool rightIsClear();
extern bool rightIsBlocked();
// beeper stuff 2nd edition
extern bool beepersPresent();
extern bool noBeepersPresent();
extern bool beepersInBag();
extern bool noBeepersInBag();
// direction dtuff 2nd edition
extern bool facingNorth();
extern bool notFacingNorth();
extern bool facingEast();
extern bool notFacingEast();
extern bool facingSouth();
extern bool notFacingSouth();
extern bool facingWest();
extern bool notFacingWest();

// WORLDS
void worldLivingRoom();
void worldFlagDistance1();
void worldFlagDistance0();
void worldFlagDistance2();
void world1x8();
void world3x5();
void world6x5();
void world7x7();
void world8x1();
void world8x8();
void world40x40();
void worldBeeperPicking();
void worldBeeperPile();
void worldBigBeeperPile();
void worldClimbTwoPyramidsKarel();
void worldDamagedPillar1();
void worldDamagedPillar2();
void worldDeathValley();
void worldDiamondMining1();
void worldDiamondMining2();
void worldFindBeeperCenter1();
void worldFindBeeperCenter2();
void worldFindBeeperCenter3();
void worldHugeCave();
void worldLargeHall();
void worldMediumCave();
void worldMediumHall();
void worldplaceFlag();
void worldSmallCave();
void worldSmallCaveTest();
void worldSmallHall();
void worldSteepleChase();
void worldSteepSteepleChase();
void worldworldArea0();
void worldworldArea1();
void worldworldArea3();
void worldMining3();
void worldMining3Ex();
// YOUR OWN CUSTOM WORLDS:
void world3x1();

// INTERN STUFF
const int maxWidth = 100;
const int maxHeight = 100;
const int maxWalls = (maxWidth - 1) * (maxHeight - 1);
const int maxBeeps = maxWidth * maxHeight;
int timeBetweenFrames = 500; // default speed

void setSpeed(int speed)
{
    timeBetweenFrames = speed;
}

int worldDimension[2] = {0, 0}; // entspricht {Y, X}
/**
* .
* 4 x..
* 3 x x..
* 2 x x x..
* 1 x x x x..
* 0 x x x x x..
J>^ 0 1 2 3 4 ..
* I
*/

int karelPosition[2] = {0, 0}; // entspricht {Y, X}
int karelRotation = 0;         // 0=north, 1=east, 2=south, 3=west
int walls[maxWalls][4];        // array of walls ; field1|field2 ==> [[field1Y,field1X,field1Y,field1X]
int beeper[maxBeeps][3];       // stores Y,X,NUMBER
int numberOfBeeper = 0;
bool hasUlimitedBeeper = false;

// ✨ Beauty ✨
// für farben die sachen unter '//COLORS' oben ändern
const char karelFacesWithoutBeeper[4] = {'^', '>', 'V', '<'}; // corresponding to karelPosition + karelBeeper
const char karelFacesWithBeeper[4] = {'A', '}', 'U', '{'};
const char beeperSymbol = '*';
const char clearSymbol = '.';
// intern shit
int isWallInWallDex(int walls[maxWalls][4], int wall[4])
{
    // printf("a: %d", 0 || (wall[0] == worldDimension[1] + 1 || wall[1] == worldDimension[1] + 1 || wall[2] == worldDimension[1] + 1 || wall[3] == worldDimension[1] + 1) || (wall[0] == worldDimension[0] + 1 || wall[1] == worldDimension[0] + 1 || wall[2] == worldDimension[0] + 1 || wall[3] == worldDimension[0] + 1) || (wall[0] == -1 || wall[1] == -1 || wall[2] == -1 || wall[3] == -1));
    for (int i = 0; i < maxBeeps; i++)
    {
        // printf("we are moving through walls \n");
        //  printf("wo: %d", worldDimension[1] + 1);

        if (walls[i][0] == wall[0] && walls[i][1] == wall[1] && walls[i][2] == wall[2] && walls[i][3] == wall[3])
        {
            // printf("FOUDN WALLL at %d with atm[%d,%d,%d,%d]", i, wall[0], wall[1], wall[2], wall[3]);
            return 1;
        }
        // check if wall is a boundary
        else if (((wall[0] == worldDimension[0] + 1 || wall[1] == worldDimension[1] + 1 || wall[2] == worldDimension[0] + 1 || wall[3] == worldDimension[1] + 1) || (wall[0] == -1 || wall[1] == -1 || wall[2] == -1 || wall[3] == -1)))
        {
            // printf("FOUDN BOUND at %d with atm[%d,%d,%d,%d] and states 1: %d, 2: %d, 3: %d and wd [%d,%d]", i, wall[0], wall[1], wall[2], wall[3], (wall[0] == worldDimension[1] + 1 || wall[1] == worldDimension[1] + 1 || wall[2] == worldDimension[1] + 1 || wall[3] == worldDimension[1] + 1), (wall[0] == worldDimension[0] + 1 || wall[1] == worldDimension[0] + 1 || wall[2] == worldDimension[0] + 1 || wall[3] == worldDimension[0] + 1), (wall[0] == -1 || wall[1] == -1 || wall[2] == -1 || wall[3] == -1), worldDimension[0], worldDimension[1]);
            return 1;
        }
    }

    return 0;
}

int isBeeperInBeepDex(int beeper[maxBeeps][3], int beep[2])
{
    // printf("Beep: %d,%d ", beep[0], beep[1]);
    for (int i = 0; i < maxBeeps; i++)
    {
        if (beeper[i][0] == beep[0] && beeper[i][1] == beep[1])
        {
            // printf("FOUND BEEP");
            return 1;
        }
    }
    return 0;
}

// 0-3 walls, 4 pickup beeper, 5 place beeper
void explosion(int explosionType)
{
    printf("boom - fehler\n");
    printf("Karel is now on: [%d;%d]\n", karelPosition[0] + 1, karelPosition[1] + 1);
    switch (explosionType)
    {
        // diretion of wall
    case 0:
    case 1:
    case 2:
    case 3:
        printf("You cant walk through walls!");
        break;
    case 4:
        printf("You cant pickup a beeper when there is none to be picked up in teh first place!");
        break;
    case 5:
        printf("You cant place a beeper, cause you dont have a beeper!");
    default:
        break;
    }
    exit(0);
}

// place walls func
void placeWallVertical(int i, int j)
{
    int atmWall[4] = {i, j, i, j + 1}; // add one to the x to see if there a wall on the right side
    // printf("Wall atm: %d,%d,%d,%d ", atmWall[0], atmWall[1], atmWall[2], atmWall[3]);
    if (j != worldDimension[1])
    {

        if (isWallInWallDex(walls, atmWall) == 1)
        {
            printf(ac_innerWall "|" ac_RESET);
        }
        else
        {
            printf(ac_space " " ac_RESET);
        }
    }
    else
    {
        printf(ac_outerWall "|" ac_RESET);
    }
}
void placeWallHorizontal(int i, int j)
{
    int atmWall[4] = {i, j, i + 1, j}; // add one to the x to see if there a wall on the right side
    // printf("Wall atm: %d,%d,%d,%d ", atmWall[0], atmWall[1], atmWall[2], atmWall[3]);
    if (isWallInWallDex(walls, atmWall) == 1)
    {
        if (j != worldDimension[1])
        {
            printf(ac_innerWall "-%s%s %s", ac_RESET, ac_space, ac_RESET);
        }
        else
        {
            printf(ac_innerWall "-" ac_RESET);
        }
    }
    else
    {
        if (j != worldDimension[1])
        {
            printf(ac_space "  " ac_RESET);
        }
        else
        {
            printf(ac_space " " ac_RESET);
        }
    }
}

bool DEBUG_Draw = true;

// show the field in the console
void draw()
{
    printf("\e[2;1H\e[2J");
    printf(ac_space "   " ac_RESET);
    for (int i = 0; i <= worldDimension[1]; i++)
    {
        printf(ac_outerWall "--" ac_RESET);
    }
    printf("\n");
    for (int i = worldDimension[0]; i >= 0; i--)
    {
        // printf("%d |", i + 1);

        printf("%s>%s %s|%s", ac_markerr, ac_space, ac_outerWall, ac_RESET);

        for (int j = 0; j <= worldDimension[1]; j++)
        {

            int atmBeep[2] = {i, j};
            // place beepers
            if ((karelPosition[0] != i || karelPosition[1] != j) && isBeeperInBeepDex(beeper, atmBeep) == 1)
            {
                int beepPos;
                for (int a = 0; a < maxBeeps; a++)
                {
                    if (beeper[a][0] == atmBeep[0] && beeper[a][1] == atmBeep[1])
                    {
                        beepPos = a;
                        break;
                    }
                }
                if (beeper[beepPos][2] == 1)
                {
                    printf(ac_beeper "%c" ac_RESET, beeperSymbol);
                }
                else
                {
                    printf(ac_beeper "%d" ac_RESET, beeper[beepPos][2]);
                }
                placeWallVertical(i, j);
                continue;
            }
            // place karel
            if (karelPosition[0] == i && karelPosition[1] == j && isBeeperInBeepDex(beeper, atmBeep) == 0)
            {
                printf(ac_karel "%c" ac_RESET, karelFacesWithoutBeeper[karelRotation]);
                placeWallVertical(i, j);
                continue;
            }
            if (karelPosition[0] == i && karelPosition[1] == j && isBeeperInBeepDex(beeper, atmBeep) == 1)
            {
                printf(ac_karelWithBeep "%c" ac_RESET, karelFacesWithBeeper[karelRotation]);
                placeWallVertical(i, j);
                continue;
            }
            printf(ac_dot "%c" ac_RESET, clearSymbol);
            placeWallVertical(i, j);
        }
        printf("\n");
        if (i != 0)
        {
            printf(ac_space "  %s|" ac_RESET, ac_outerWall);
            for (int j = 0; j <= worldDimension[1]; j++)
            {
                placeWallHorizontal(i - 1, j);
            }
            printf(ac_outerWall "|\n" ac_RESET);
        }
    }
    printf(ac_space "   " ac_RESET);
    for (int i = 0; i <= worldDimension[1]; i++)
    {
        printf(ac_outerWall "--" ac_RESET);
    }
    printf(ac_space "\n   " ac_RESET);
    for (int i = 0; i <= worldDimension[1]; i++)
    {
        // printf("%d ", i + 1);
        printf("%s^%s " ac_RESET, ac_markerr, ac_space);
    }
    printf("\n");
    // printf beeper amount
    if (hasUlimitedBeeper)
    {
        printf(ac_beeper "Beepers in Bag: %sinfinite\n" ac_RESET, ac_fgGREEN);
    }
    else
    {
        printf(ac_beeper "Beepers in Bag: %s%d\n" ac_RESET, ac_fgGREEN, numberOfBeeper);
    }
    // print speed
    printf(ac_speed "Current Speed: %s%d %sms\n" ac_RESET, ac_fgYELLOW, timeBetweenFrames, ac_speed);
    msleep(timeBetweenFrames);
}

void initializeBeeps()
{
    for (int i = 0; i < maxBeeps; i++)
    {
        beeper[i][0] = -1;
        beeper[i][1] = -1;
        beeper[i][2] = 0;
    }
}
void initializeWalls()
{
    for (int i = 0; i < maxWalls; i++)
    {
        walls[i][0] = -2;
        walls[i][1] = -2;
        walls[i][2] = -2;
        walls[i][3] = -2;
    }
}
// setup shit
void loadWorld(char worldName[100])
{
    // pre for all worlds
    initializeBeeps();
    initializeWalls();
    // world selection:
    if (!strcmp(worldName, "LivingRoom"))
    {
        worldLivingRoom();
    }
    else if (!strcmp(worldName, "flagDistance1"))
    {
        worldFlagDistance1();
    }
    else if (!strcmp(worldName, "flagDistance0"))
    {
        worldFlagDistance0();
    }
    else if (!strcmp(worldName, "flagDistance2"))
    {
        worldFlagDistance2();
    }
    else if (!strcmp(worldName, "1x8"))
    {
        world1x8();
    }
    else if (!strcmp(worldName, "3x5"))
    {
        world3x5();
    }
    else if (!strcmp(worldName, "6x5"))
    {
        world6x5();
    }
    else if (!strcmp(worldName, "7x7"))
    {
        world7x7();
    }
    else if (!strcmp(worldName, "8x1"))
    {
        world8x1();
    }
    else if (!strcmp(worldName, "8x8"))
    {
        world8x8();
    }
    else if (!strcmp(worldName, "40x40"))
    {
        world40x40();
    }
    else if (!strcmp(worldName, "BeeperPicking"))
    {
        worldBeeperPicking();
    }
    else if (!strcmp(worldName, "BeeperPile"))
    {
        worldBeeperPile();
    }
    else if (!strcmp(worldName, "BigBeeperPile"))
    {
        worldBigBeeperPile();
    }
    else if (!strcmp(worldName, "ClimbTwoPyramidsKarel"))
    {
        worldClimbTwoPyramidsKarel();
    }
    else if (!strcmp(worldName, "DamagedPillar1"))
    {
        worldDamagedPillar1();
    }
    else if (!strcmp(worldName, "DamagedPillar2"))
    {
        worldDamagedPillar2();
    }
    else if (!strcmp(worldName, "DeathValley"))
    {
        worldDeathValley();
    }
    else if (!strcmp(worldName, "DiamondMining1"))
    {
        worldDiamondMining1();
    }
    else if (!strcmp(worldName, "DiamondMining2"))
    {
        worldDiamondMining2();
    }
    else if (!strcmp(worldName, "FindBeeperCenter1"))
    {
        worldFindBeeperCenter1();
    }
    else if (!strcmp(worldName, "FindBeeperCenter2"))
    {
        worldFindBeeperCenter2();
    }
    else if (!strcmp(worldName, "FindBeeperCenter3"))
    {
        worldFindBeeperCenter3();
    }
    else if (!strcmp(worldName, "HugeCave"))
    {
        worldHugeCave();
    }
    else if (!strcmp(worldName, "LargeHall"))
    {
        worldLargeHall();
    }
    else if (!strcmp(worldName, "MediumCave"))
    {
        worldMediumCave();
    }
    else if (!strcmp(worldName, "MediumHall"))
    {
        worldMediumHall();
    }
    else if (!strcmp(worldName, "placeFlag"))
    {
        worldplaceFlag();
    }
    else if (!strcmp(worldName, "SmallCave"))
    {
        worldSmallCave();
    }
    else if (!strcmp(worldName, "SmallCaveTest"))
    {
        worldSmallCaveTest();
    }
    else if (!strcmp(worldName, "SmallHall"))
    {
        worldSmallHall();
    }
    else if (!strcmp(worldName, "SteepleChase"))
    {
        worldSteepleChase();
    }
    else if (!strcmp(worldName, "SteepSteepleChase"))
    {
        worldSteepSteepleChase();
    }
    else if (!strcmp(worldName, "worldArea0"))
    {
        worldworldArea0();
    }
    else if (!strcmp(worldName, "worldArea1"))
    {
        worldworldArea1();
    }
    else if (!strcmp(worldName, "worldArea3"))
    {
        worldworldArea3();
    }
    else if (!strcmp(worldName, "Mining3"))
    {
        worldMining3();
    }
    else if (!strcmp(worldName, "Mining3Ex"))
    {
        worldMining3Ex();
    }
    else if (!strcmp(worldName, "3x1"))
    {
        world3x1();
    }

    printf("loedade teh wolrd %s \n if you see this, then a error occured.. maybe", worldName);
}

// manual mode - get called each draw frame
void manualMode()
{
    printf("Entered Manual Control Mode. To exit type 'exit();'.\n");
    while (true)
    {
        char input[30];
        scanf("%s", input);
        if (input[0] == 'm' && input[1] == 'o' && input[2] == 'v' && input[3] == 'e' && input[4] == '(' && input[5] == ')')
        {
            move();
            continue;
        }
        if (input[0] == 't' && input[1] == 'u' && input[2] == 'r' && input[3] == 'n' && input[4] == 'L' && input[5] == 'e' && input[6] == 'f' && input[7] == 't' && input[8] == '(' && input[9] == ')')
        {
            turnLeft();
            continue;
        }
        if (input[0] == 'p' && input[1] == 'u' && input[2] == 't' && input[3] == 'B' && input[4] == 'e' && input[5] == 'e' && input[6] == 'p' && input[7] == 'e' && input[8] == 'r' && input[9] == '(' && input[10] == ')')
        {
            putBeeper();
            continue;
        }
        if (input[0] == 'p' && input[1] == 'i' && input[2] == 'c' && input[3] == 'k' && input[4] == 'B' && input[5] == 'e' && input[6] == 'e' && input[7] == 'p' && input[8] == 'e' && input[9] == 'r' && input[10] == '(' && input[11] == ')')
        {
            pickBeeper();
            continue;
        }
        if (input[0] == 'e' && input[1] == 'x' && input[2] == 'i' && input[3] == 't' && input[4] == '(' && input[5] == ')')
        {
            printf("Exited manual modus.");
            return;
        }
    }
}
// action shit

// move
void move()
{
    // bool a = facingNorth();
    switch (karelRotation)
    {
    case 0:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0] + 1, karelPosition[1]};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            explosion(karelRotation);
        }
        karelPosition[0]++;
    }
    break;
    case 1:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0], karelPosition[1] + 1};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            explosion(karelRotation);
        }
        karelPosition[1]++;
    }
    break;
    case 2:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0] - 1, karelPosition[1]};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            explosion(karelRotation);
        }
        karelPosition[0]--;
    }
    break;
    case 3:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0], karelPosition[1] - 1};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            explosion(karelRotation);
        }
        karelPosition[1]--;
    }
    break;
    default:
        break;
    }
    draw();
}

// direction
void turnLeft()
{
    karelRotation--;
    if (karelRotation == -1)
    {
        karelRotation = 3;
    }
    draw();
}
// beeper stuff
void putBeeper()
{
    if (!hasUlimitedBeeper)
    {
        if (numberOfBeeper == 0)
        {
            explosion(5);
        }
        numberOfBeeper--;
    }
    int beep[2] = {karelPosition[0], karelPosition[1]};
    for (int i = 0; i < maxBeeps; i++)
    {
        if (beeper[i][0] == beep[0] && beeper[i][1] == beep[1])
        {
            beeper[i][2]++;
            // printf("added");
            draw();
            return;
        }
    }
    for (int i = 0; i < maxBeeps; i++)
    {
        if (beeper[i][0] == -1 && beeper[i][1] == -1)
        {
            beeper[i][0] = beep[0];
            beeper[i][1] = beep[1];
            beeper[i][2] = 1;
            // printf("new");
            draw();
            return;
        }
    }
}
void pickBeeper()
{

    int beep[2] = {karelPosition[0], karelPosition[1]};
    for (int i = 0; i < maxBeeps; i++)
    {
        if (beeper[i][0] == beep[0] && beeper[i][1] == beep[1])
        {
            if (beeper[i][2] > 1)
            {
                beeper[i][2]--;
            }
            else if (beeper[i][2] == 1)
            {
                beeper[i][0] = -1;
                beeper[i][1] = -1;
            }
            else if (beeper[i][2] <= 0)
            {
                explosion(4);
            }
        }
    }
    if (!hasUlimitedBeeper)
    {
        numberOfBeeper++;
    }
    draw();
}
// Clear checks
bool frontIsClear()
{
    switch (karelRotation)
    {
    case 0:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0] + 1, karelPosition[1]};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            return false;
        }
        return true;
    }
    case 1:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0], karelPosition[1] + 1};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            return false;
        }
        return true;
    }
    case 2:
    {
        int theWallInQuestion[4] = {karelPosition[0] - 1, karelPosition[1], karelPosition[0], karelPosition[1]};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            return false;
        }
        return true;
    }
    case 3:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1] - 1, karelPosition[0], karelPosition[1]};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            return false;
        }
        return true;
    }
    default:
        break;
    }
    return false;
}
bool frontIsBlocked()
{
    return !frontIsClear();
}
bool leftIsClear()
{
    switch (karelRotation)
    {
    case 0:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1] - 1, karelPosition[0], karelPosition[1]};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            return false;
        }
        return true;
    }
    case 1:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0] + 1, karelPosition[1]};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            return false;
        }
        return true;
    }
    case 2:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0], karelPosition[1] + 1};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            return false;
        }
        return true;
    }
    case 3:
    {
        int theWallInQuestion[4] = {karelPosition[0] - 1, karelPosition[1], karelPosition[0], karelPosition[1]};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            return false;
        }
        return true;
    }
    default:
        break;
    }
    return false;
}
bool leftIsBlocked()
{
    return !leftIsClear();
}
bool rightIsClear()
{
    // printf("Krot: %d", karelRotation);
    switch (karelRotation)
    {
    case 0:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0], karelPosition[1] + 1};
        return !isWallInWallDex(walls, theWallInQuestion);
    }
    case 1:
    {
        int theWallInQuestion[4] = {karelPosition[0] - 1, karelPosition[1], karelPosition[0], karelPosition[1]};
        // printf("result %s", isWallInWallDex(walls, theWallInQuestion) ? "true" : "false");
        // printf("wallInQustion: [%d,%d,%d,%d]", theWallInQuestion[0], theWallInQuestion[1], theWallInQuestion[2], theWallInQuestion[3]);
        return !isWallInWallDex(walls, theWallInQuestion);
    }
    case 2:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1] - 1, karelPosition[0], karelPosition[1]};
        return !isWallInWallDex(walls, theWallInQuestion);
    }
    case 3:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0] + 1, karelPosition[1]};
        return !isWallInWallDex(walls, theWallInQuestion);
    }
    default:
        break;
    }
    return false;
}
bool rightIsBlocked()
{
    return !rightIsClear();
}
// beeper stuff 2nd edition
bool beepersPresent()
{
    int theBeepInQuestion[2] = {karelPosition[0], karelPosition[1]};
    if (isBeeperInBeepDex(beeper, theBeepInQuestion))
    {
        return true;
    }
    return false;
}
bool noBeepersPresent()
{
    return !beepersPresent();
}
// direction dtuff 2nd edition
bool facingNorth()
{
    return karelRotation == 0 ? true : false;
}
bool notFacingNorth()
{
    return !facingNorth();
}
bool facingEast()
{
    return karelRotation == 1 ? true : false;
}
bool notFacingEast()
{
    return !facingEast();
}
bool facingSouth()
{
    return karelRotation == 2 ? true : false;
}
bool notFacingSouth()
{
    return !facingSouth();
}
bool facingWest()
{
    return karelRotation == 3 ? true : false;
}
bool notFacingWest()
{
    return !facingWest();
}

int main()
{
    setup();
    draw();
    run();
    printf(ac_fgGREEN "finsihed" ac_RESET);
}

/**
 *
 * WOLRDS
 *
 */

// dont forget to put void worldPreset(); at the top of file

//  void worldPreset()
//  {
//      hasUlimitedBeeper = true;
//      numberOfBeeper = 0;
//      setSpeed(500);       //speed in ms
//     worldDimension[0] = Y - 1; // set to value in file
//     worldDimension[1] = X - 1; // set to value in file
//     karelPosition[0] = Y - 1;   // set to value in file
//     karelPosition[1] = X - 1;   // set to value in file
//     karelRotation = 1;
// //walls
//     walls[0][0] = Y - 1;  // set to value in file
//     walls[0][1] = X - 1;     //...
//     walls[0][2] = Y - 1;
//     walls[0][3] = X - 1;
//     walls[1][0]...
// //beeper
//     beeper[0][0] = Y - 1;        // set to value in file
//     beeper[0][1] = X - 1;        //..
//     beeper[0][2] = N; // number of beeper; PLS DONT SET IT TO ZERO
//     beeper[1][0]..
// }

// livingroom
void worldLivingRoom()
{
    hasUlimitedBeeper = false;
    numberOfBeeper = 0;
    setSpeed(500);

    worldDimension[0] = 10 - 1; // set to value in file
    worldDimension[1] = 10 - 1; // set to value in file
    karelPosition[0] = 1 - 1;   // set to value in file
    karelPosition[1] = 1 - 1;   // set to value in file
    karelRotation = 1;
    walls[0][0] = 1 - 1;
    walls[0][1] = 6 - 1;
    walls[0][2] = 1 - 1;
    walls[0][3] = 7 - 1;
    walls[1][0] = 2 - 1;
    walls[1][1] = 6 - 1;
    walls[1][2] = 2 - 1;
    walls[1][3] = 7 - 1;
    walls[2][0] = 3 - 1;
    walls[2][1] = 6 - 1;
    walls[2][2] = 3 - 1;
    walls[2][3] = 7 - 1;
    walls[3][0] = 3 - 1;
    walls[3][1] = 7 - 1;
    walls[3][2] = 4 - 1;
    walls[3][3] = 7 - 1;
    walls[4][0] = 3 - 1;
    walls[4][1] = 8 - 1;
    walls[4][2] = 4 - 1;
    walls[4][3] = 8 - 1;
    walls[5][0] = 3 - 1;
    walls[5][1] = 9 - 1;
    walls[5][2] = 4 - 1;
    walls[5][3] = 9 - 1;
    walls[6][0] = 3 - 1;
    walls[6][1] = 10 - 1;
    walls[6][2] = 4 - 1;
    walls[6][3] = 10 - 1;

    beeper[0][0] = 4 - 1;
    beeper[0][1] = 8 - 1;
    beeper[0][2] = 1; // number of beeper; PLS DONT SET IT TO ZERO
}

// flag worlds
void worldFlagDistance1()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(250); // speed in ms

    worldDimension[0] = 4 - 1;  // set to value in file
    worldDimension[1] = 12 - 1; // set to value in file
    karelPosition[0] = 1 - 1;   // set to value in file
    karelPosition[1] = 1 - 1;   // set to value in file
    karelRotation = 1;

    // walls
    walls[0][0] = 1 - 1;
    walls[0][1] = 10 - 1;
    walls[0][2] = 1 - 1;
    walls[0][3] = 11 - 1;

    walls[1][0] = 2 - 1;
    walls[1][1] = 10 - 1;
    walls[1][2] = 2 - 1;
    walls[1][3] = 11 - 1;

    walls[2][0] = 3 - 1;
    walls[2][1] = 10 - 1;
    walls[2][2] = 3 - 1;
    walls[2][3] = 11 - 1;

    walls[3][0] = 3 - 1;
    walls[3][1] = 11 - 1;
    walls[3][2] = 3 - 1;
    walls[3][3] = 12 - 1;

    walls[4][0] = 2 - 1;
    walls[4][1] = 11 - 1;
    walls[4][2] = 3 - 1;
    walls[4][3] = 11 - 1;

    walls[5][0] = 3 - 1;
    walls[5][1] = 11 - 1;
    walls[5][2] = 4 - 1;
    walls[5][3] = 11 - 1;
    // beeper
}

void worldFlagDistance0()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(100); // speed in ms

    worldDimension[0] = 4 - 1;  // set to value in file
    worldDimension[1] = 10 - 1; // set to value in file
    karelPosition[0] = 1 - 1;   // set to value in file
    karelPosition[1] = 1 - 1;   // set to value in file
    karelRotation = 1;

    // walls
    walls[0][0] = 1 - 1;
    walls[0][1] = 1 - 1;
    walls[0][2] = 1 - 1;
    walls[0][3] = 2 - 1;

    walls[1][0] = 2 - 1;
    walls[1][1] = 1 - 1;
    walls[1][2] = 2 - 1;
    walls[1][3] = 2 - 1;

    walls[2][0] = 3 - 1;
    walls[2][1] = 1 - 1;
    walls[2][2] = 3 - 1;
    walls[2][3] = 2 - 1;

    walls[3][0] = 3 - 1;
    walls[3][1] = 2 - 1;
    walls[3][2] = 3 - 1;
    walls[3][3] = 3 - 1;

    walls[4][0] = 2 - 1;
    walls[4][1] = 2 - 1;
    walls[4][2] = 3 - 1;
    walls[4][3] = 2 - 1;

    walls[5][0] = 3 - 1;
    walls[5][1] = 2 - 1;
    walls[5][2] = 4 - 1;
    walls[5][3] = 2 - 1;
    // beeper
}

void worldFlagDistance2()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(100); // speed in ms

    worldDimension[0] = 4 - 1;  // set to value in file
    worldDimension[1] = 28 - 1; // set to value in file
    karelPosition[0] = 1 - 1;   // set to value in file
    karelPosition[1] = 1 - 1;   // set to value in file
    karelRotation = 1;

    // walls
    walls[0][0] = 1 - 1;
    walls[0][1] = 25 - 1;
    walls[0][2] = 1 - 1;
    walls[0][3] = 26 - 1;

    walls[1][0] = 2 - 1;
    walls[1][1] = 25 - 1;
    walls[1][2] = 2 - 1;
    walls[1][3] = 26 - 1;

    walls[2][0] = 3 - 1;
    walls[2][1] = 25 - 1;
    walls[2][2] = 3 - 1;
    walls[2][3] = 26 - 1;

    walls[3][0] = 3 - 1;
    walls[3][1] = 26 - 1;
    walls[3][2] = 3 - 1;
    walls[3][3] = 27 - 1;

    walls[4][0] = 2 - 1;
    walls[4][1] = 26 - 1;
    walls[4][2] = 3 - 1;
    walls[4][3] = 26 - 1;

    walls[5][0] = 3 - 1;
    walls[5][1] = 26 - 1;
    walls[5][2] = 4 - 1;
    walls[5][3] = 26 - 1;
    // beeper
}

// empty worlds
void world1x8()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);             // speed in ms
    worldDimension[0] = 8 - 1; // set to value in file
    worldDimension[1] = 1 - 1; // set to value in file
    karelPosition[0] = 1 - 1;  // set to value in file
    karelPosition[1] = 1 - 1;  // set to value in file
    karelRotation = 1;
}

void world3x5()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);             // speed in ms
    worldDimension[0] = 5 - 1; // set to value in file
    worldDimension[1] = 3 - 1; // set to value in file
    karelPosition[0] = 1 - 1;  // set to value in file
    karelPosition[1] = 1 - 1;  // set to value in file
    karelRotation = 1;
}

void world6x5()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);             // speed in ms
    worldDimension[0] = 5 - 1; // set to value in file
    worldDimension[1] = 6 - 1; // set to value in file
    karelPosition[0] = 1 - 1;  // set to value in file
    karelPosition[1] = 1 - 1;  // set to value in file
    karelRotation = 1;
}

void world7x7()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);             // speed in ms
    worldDimension[0] = 7 - 1; // set to value in file
    worldDimension[1] = 7 - 1; // set to value in file
    karelPosition[0] = 1 - 1;  // set to value in file
    karelPosition[1] = 1 - 1;  // set to value in file
    karelRotation = 1;
}

void world8x1()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);             // speed in ms
    worldDimension[0] = 1 - 1; // set to value in file
    worldDimension[1] = 8 - 1; // set to value in file
    karelPosition[0] = 1 - 1;  // set to value in file
    karelPosition[1] = 1 - 1;  // set to value in file
    karelRotation = 1;
}

void world8x8()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);             // speed in ms
    worldDimension[0] = 8 - 1; // set to value in file
    worldDimension[1] = 8 - 1; // set to value in file
    karelPosition[0] = 1 - 1;  // set to value in file
    karelPosition[1] = 1 - 1;  // set to value in file
    karelRotation = 1;
}

void world40x40()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);              // speed in ms
    worldDimension[0] = 40 - 1; // set to value in file
    worldDimension[1] = 40 - 1; // set to value in file
    karelPosition[0] = 1 - 1;   // set to value in file
    karelPosition[1] = 1 - 1;   // set to value in file
    karelRotation = 1;
}

// other worlds?

void worldBeeperPicking()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);             // speed in ms
    worldDimension[0] = 4 - 1; // set to value in file
    worldDimension[1] = 6 - 1; // set to value in file
    karelPosition[0] = 1 - 1;  // set to value in file
    karelPosition[1] = 1 - 1;  // set to value in file
    karelRotation = 1;
    // walls
    walls[0][0] = 1 - 1; // set to value in file
    walls[0][1] = 3 - 1; //...
    walls[0][2] = 1 - 1;
    walls[0][3] = 4 - 1;

    walls[1][0] = 1 - 1; // set to value in file
    walls[1][1] = 4 - 1; //...
    walls[1][2] = 2 - 1;
    walls[1][3] = 4 - 1;

    walls[2][0] = 1 - 1; // set to value in file
    walls[2][1] = 5 - 1; //...
    walls[2][2] = 2 - 1;
    walls[2][3] = 5 - 1;

    walls[3][0] = 1 - 1; // set to value in file
    walls[3][1] = 6 - 1; //...
    walls[3][2] = 2 - 1;
    walls[3][3] = 6 - 1;
    //  beeper
    beeper[0][0] = 1 - 1; // set to value in file
    beeper[0][1] = 2 - 1; //..
    beeper[0][2] = 1;     // number of beeper; PLS DONT SET IT TO ZERO
    // beeper[1][0]..
}

void worldBeeperPile()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);             // speed in ms
    worldDimension[0] = 5 - 1; // set to value in file
    worldDimension[1] = 5 - 1; // set to value in file
    karelPosition[0] = 1 - 1;  // set to value in file
    karelPosition[1] = 1 - 1;  // set to value in file
    karelRotation = 1;
    // walls

    //  beeper
    beeper[0][0] = 1 - 1; // set to value in file
    beeper[0][1] = 2 - 1; //..
    beeper[0][2] = 5;     // number of beeper; PLS DONT SET IT TO ZERO
    // beeper[1][0]..
}

void worldBigBeeperPile()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);              // speed in ms
    worldDimension[0] = 10 - 1; // set to value in file
    worldDimension[1] = 10 - 1; // set to value in file
    karelPosition[0] = 1 - 1;   // set to value in file
    karelPosition[1] = 1 - 1;   // set to value in file
    karelRotation = 1;
    // walls

    //  beeper
    beeper[0][0] = 1 - 1; // set to value in file
    beeper[0][1] = 2 - 1; //..
    beeper[0][2] = 42;    // number of beeper; PLS DONT SET IT TO ZERO
    // beeper[1][0]..
}

void worldClimbTwoPyramidsKarel()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 10 - 1;
    worldDimension[1] = 20 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 1 - 1;
    walls[0][1] = 2 - 1;
    walls[0][2] = 1 - 1;
    walls[0][3] = 3 - 1;
    walls[1][0] = 1 - 1;
    walls[1][1] = 3 - 1;
    walls[1][2] = 2 - 1;
    walls[1][3] = 3 - 1;
    walls[2][0] = 2 - 1;
    walls[2][1] = 3 - 1;
    walls[2][2] = 2 - 1;
    walls[2][3] = 4 - 1;
    walls[3][0] = 2 - 1;
    walls[3][1] = 4 - 1;
    walls[3][2] = 3 - 1;
    walls[3][3] = 4 - 1;
    walls[4][0] = 3 - 1;
    walls[4][1] = 4 - 1;
    walls[4][2] = 3 - 1;
    walls[4][3] = 5 - 1;
    walls[5][0] = 3 - 1;
    walls[5][1] = 5 - 1;
    walls[5][2] = 4 - 1;
    walls[5][3] = 5 - 1;
    walls[6][0] = 4 - 1;
    walls[6][1] = 5 - 1;
    walls[6][2] = 4 - 1;
    walls[6][3] = 6 - 1;
    walls[7][0] = 4 - 1;
    walls[7][1] = 6 - 1;
    walls[7][2] = 5 - 1;
    walls[7][3] = 6 - 1;
    walls[8][0] = 5 - 1;
    walls[8][1] = 6 - 1;
    walls[8][2] = 5 - 1;
    walls[8][3] = 7 - 1;
    walls[9][0] = 5 - 1;
    walls[9][1] = 7 - 1;
    walls[9][2] = 6 - 1;
    walls[9][3] = 7 - 1;
    walls[10][0] = 5 - 1;
    walls[10][1] = 7 - 1;
    walls[10][2] = 5 - 1;
    walls[10][3] = 8 - 1;
    walls[11][0] = 4 - 1;
    walls[11][1] = 8 - 1;
    walls[11][2] = 5 - 1;
    walls[11][3] = 8 - 1;
    walls[12][0] = 4 - 1;
    walls[12][1] = 8 - 1;
    walls[12][2] = 4 - 1;
    walls[12][3] = 9 - 1;
    walls[13][0] = 3 - 1;
    walls[13][1] = 9 - 1;
    walls[13][2] = 4 - 1;
    walls[13][3] = 9 - 1;
    walls[14][0] = 3 - 1;
    walls[14][1] = 9 - 1;
    walls[14][2] = 3 - 1;
    walls[14][3] = 10 - 1;
    walls[15][0] = 2 - 1;
    walls[15][1] = 10 - 1;
    walls[15][2] = 3 - 1;
    walls[15][3] = 10 - 1;
    walls[16][0] = 2 - 1;
    walls[16][1] = 10 - 1;
    walls[16][2] = 2 - 1;
    walls[16][3] = 11 - 1;
    walls[17][0] = 1 - 1;
    walls[17][1] = 11 - 1;
    walls[17][2] = 2 - 1;
    walls[17][3] = 11 - 1;
    walls[18][0] = 1 - 1;
    walls[18][1] = 11 - 1;
    walls[18][2] = 1 - 1;
    walls[18][3] = 12 - 1;
    walls[19][0] = 1 - 1;
    walls[19][1] = 14 - 1;
    walls[19][2] = 1 - 1;
    walls[19][3] = 15 - 1;
    walls[20][0] = 1 - 1;
    walls[20][1] = 15 - 1;
    walls[20][2] = 2 - 1;
    walls[20][3] = 15 - 1;
    walls[21][0] = 2 - 1;
    walls[21][1] = 15 - 1;
    walls[21][2] = 2 - 1;
    walls[21][3] = 16 - 1;
    walls[22][0] = 2 - 1;
    walls[22][1] = 16 - 1;
    walls[22][2] = 3 - 1;
    walls[22][3] = 16 - 1;
    walls[23][0] = 2 - 1;
    walls[23][1] = 16 - 1;
    walls[23][2] = 2 - 1;
    walls[23][3] = 17 - 1;
    walls[24][0] = 1 - 1;
    walls[24][1] = 17 - 1;
    walls[24][2] = 2 - 1;
    walls[24][3] = 17 - 1;
    walls[25][0] = 1 - 1;
    walls[25][1] = 17 - 1;
    walls[25][2] = 1 - 1;
    walls[25][3] = 18 - 1;
    beeper[0][0] = 3 - 1;
    beeper[0][1] = 4 - 1;
    beeper[0][2] = 1;
    beeper[1][0] = 5 - 1;
    beeper[1][1] = 6 - 1;
    beeper[1][2] = 1;
    beeper[2][0] = 3 - 1;
    beeper[2][1] = 10 - 1;
    beeper[2][2] = 1;
    beeper[3][0] = 3 - 1;
    beeper[3][1] = 16 - 1;
    beeper[3][2] = 1;
    beeper[4][0] = 2 - 1;
    beeper[4][1] = 17 - 1;
    beeper[4][2] = 1;
}

void worldDamagedPillar1()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 13 - 1;
    worldDimension[1] = 13 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 5 - 1;
    walls[0][1] = 1 - 1;
    walls[0][2] = 6 - 1;
    walls[0][3] = 1 - 1;
    walls[1][0] = 6 - 1;
    walls[1][1] = 1 - 1;
    walls[1][2] = 6 - 1;
    walls[1][3] = 2 - 1;
    walls[2][0] = 6 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 7 - 1;
    walls[2][3] = 2 - 1;
    walls[3][0] = 7 - 1;
    walls[3][1] = 2 - 1;
    walls[3][2] = 7 - 1;
    walls[3][3] = 3 - 1;
    walls[4][0] = 7 - 1;
    walls[4][1] = 3 - 1;
    walls[4][2] = 8 - 1;
    walls[4][3] = 3 - 1;
    walls[5][0] = 7 - 1;
    walls[5][1] = 3 - 1;
    walls[5][2] = 7 - 1;
    walls[5][3] = 4 - 1;
    walls[6][0] = 6 - 1;
    walls[6][1] = 4 - 1;
    walls[6][2] = 7 - 1;
    walls[6][3] = 4 - 1;
    walls[7][0] = 6 - 1;
    walls[7][1] = 4 - 1;
    walls[7][2] = 6 - 1;
    walls[7][3] = 5 - 1;
    walls[8][0] = 5 - 1;
    walls[8][1] = 5 - 1;
    walls[8][2] = 6 - 1;
    walls[8][3] = 5 - 1;
    walls[9][0] = 6 - 1;
    walls[9][1] = 5 - 1;
    walls[9][2] = 6 - 1;
    walls[9][3] = 6 - 1;
    walls[10][0] = 6 - 1;
    walls[10][1] = 6 - 1;
    walls[10][2] = 7 - 1;
    walls[10][3] = 6 - 1;
    walls[11][0] = 7 - 1;
    walls[11][1] = 6 - 1;
    walls[11][2] = 7 - 1;
    walls[11][3] = 7 - 1;
    walls[12][0] = 7 - 1;
    walls[12][1] = 7 - 1;
    walls[12][2] = 8 - 1;
    walls[12][3] = 7 - 1;
    walls[13][0] = 7 - 1;
    walls[13][1] = 7 - 1;
    walls[13][2] = 7 - 1;
    walls[13][3] = 8 - 1;
    walls[14][0] = 6 - 1;
    walls[14][1] = 8 - 1;
    walls[14][2] = 7 - 1;
    walls[14][3] = 8 - 1;
    walls[15][0] = 6 - 1;
    walls[15][1] = 8 - 1;
    walls[15][2] = 6 - 1;
    walls[15][3] = 9 - 1;
    walls[16][0] = 5 - 1;
    walls[16][1] = 9 - 1;
    walls[16][2] = 6 - 1;
    walls[16][3] = 9 - 1;
    walls[17][0] = 6 - 1;
    walls[17][1] = 9 - 1;
    walls[17][2] = 6 - 1;
    walls[17][3] = 10 - 1;
    walls[18][0] = 6 - 1;
    walls[18][1] = 10 - 1;
    walls[18][2] = 7 - 1;
    walls[18][3] = 10 - 1;
    walls[19][0] = 7 - 1;
    walls[19][1] = 10 - 1;
    walls[19][2] = 7 - 1;
    walls[19][3] = 11 - 1;
    walls[20][0] = 7 - 1;
    walls[20][1] = 11 - 1;
    walls[20][2] = 8 - 1;
    walls[20][3] = 11 - 1;
    walls[21][0] = 7 - 1;
    walls[21][1] = 11 - 1;
    walls[21][2] = 7 - 1;
    walls[21][3] = 12 - 1;
    walls[22][0] = 6 - 1;
    walls[22][1] = 12 - 1;
    walls[22][2] = 7 - 1;
    walls[22][3] = 12 - 1;
    walls[23][0] = 6 - 1;
    walls[23][1] = 12 - 1;
    walls[23][2] = 6 - 1;
    walls[23][3] = 13 - 1;
    walls[24][0] = 5 - 1;
    walls[24][1] = 13 - 1;
    walls[24][2] = 6 - 1;
    walls[24][3] = 13 - 1;
    beeper[0][0] = 4 - 1;
    beeper[0][1] = 1 - 1;
    beeper[0][2] = 1;
    beeper[1][0] = 5 - 1;
    beeper[1][1] = 1 - 1;
    beeper[1][2] = 1;
    beeper[2][0] = 1 - 1;
    beeper[2][1] = 5 - 1;
    beeper[2][2] = 1;
    beeper[3][0] = 2 - 1;
    beeper[3][1] = 5 - 1;
    beeper[3][2] = 1;
    beeper[4][0] = 4 - 1;
    beeper[4][1] = 5 - 1;
    beeper[4][2] = 1;
    beeper[5][0] = 3 - 1;
    beeper[5][1] = 9 - 1;
    beeper[5][2] = 1;
    beeper[6][0] = 5 - 1;
    beeper[6][1] = 9 - 1;
    beeper[6][2] = 1;
    beeper[7][0] = 1 - 1;
    beeper[7][1] = 13 - 1;
    beeper[7][2] = 1;
    beeper[8][0] = 3 - 1;
    beeper[8][1] = 13 - 1;
    beeper[8][2] = 1;
    beeper[9][0] = 5 - 1;
    beeper[9][1] = 13 - 1;
    beeper[9][2] = 1;
}

void worldDamagedPillar2()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 9 - 1;
    worldDimension[1] = 13 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 3 - 1;
    walls[0][1] = 1 - 1;
    walls[0][2] = 4 - 1;
    walls[0][3] = 1 - 1;
    walls[1][0] = 4 - 1;
    walls[1][1] = 1 - 1;
    walls[1][2] = 4 - 1;
    walls[1][3] = 2 - 1;
    walls[2][0] = 4 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 5 - 1;
    walls[2][3] = 2 - 1;
    walls[3][0] = 4 - 1;
    walls[3][1] = 2 - 1;
    walls[3][2] = 4 - 1;
    walls[3][3] = 3 - 1;
    walls[4][0] = 3 - 1;
    walls[4][1] = 3 - 1;
    walls[4][2] = 4 - 1;
    walls[4][3] = 3 - 1;
    walls[5][0] = 4 - 1;
    walls[5][1] = 3 - 1;
    walls[5][2] = 4 - 1;
    walls[5][3] = 4 - 1;
    walls[6][0] = 4 - 1;
    walls[6][1] = 4 - 1;
    walls[6][2] = 5 - 1;
    walls[6][3] = 4 - 1;
    walls[7][0] = 5 - 1;
    walls[7][1] = 4 - 1;
    walls[7][2] = 5 - 1;
    walls[7][3] = 5 - 1;
    walls[8][0] = 5 - 1;
    walls[8][1] = 5 - 1;
    walls[8][2] = 6 - 1;
    walls[8][3] = 5 - 1;
    walls[9][0] = 5 - 1;
    walls[9][1] = 5 - 1;
    walls[9][2] = 5 - 1;
    walls[9][3] = 6 - 1;
    walls[10][0] = 4 - 1;
    walls[10][1] = 6 - 1;
    walls[10][2] = 5 - 1;
    walls[10][3] = 6 - 1;
    walls[11][0] = 4 - 1;
    walls[11][1] = 6 - 1;
    walls[11][2] = 4 - 1;
    walls[11][3] = 7 - 1;
    walls[12][0] = 3 - 1;
    walls[12][1] = 7 - 1;
    walls[12][2] = 4 - 1;
    walls[12][3] = 7 - 1;
    walls[13][0] = 3 - 1;
    walls[13][1] = 7 - 1;
    walls[13][2] = 3 - 1;
    walls[13][3] = 8 - 1;
    walls[14][0] = 2 - 1;
    walls[14][1] = 8 - 1;
    walls[14][2] = 3 - 1;
    walls[14][3] = 8 - 1;
    walls[15][0] = 2 - 1;
    walls[15][1] = 8 - 1;
    walls[15][2] = 2 - 1;
    walls[15][3] = 9 - 1;
    walls[16][0] = 1 - 1;
    walls[16][1] = 9 - 1;
    walls[16][2] = 2 - 1;
    walls[16][3] = 9 - 1;
    walls[17][0] = 2 - 1;
    walls[17][1] = 9 - 1;
    walls[17][2] = 2 - 1;
    walls[17][3] = 10 - 1;
    walls[18][0] = 2 - 1;
    walls[18][1] = 10 - 1;
    walls[18][2] = 3 - 1;
    walls[18][3] = 10 - 1;
    walls[19][0] = 3 - 1;
    walls[19][1] = 10 - 1;
    walls[19][2] = 3 - 1;
    walls[19][3] = 11 - 1;
    walls[20][0] = 3 - 1;
    walls[20][1] = 11 - 1;
    walls[20][2] = 4 - 1;
    walls[20][3] = 11 - 1;
    walls[21][0] = 4 - 1;
    walls[21][1] = 11 - 1;
    walls[21][2] = 4 - 1;
    walls[21][3] = 12 - 1;
    walls[22][0] = 4 - 1;
    walls[22][1] = 12 - 1;
    walls[22][2] = 5 - 1;
    walls[22][3] = 12 - 1;
    walls[23][0] = 4 - 1;
    walls[23][1] = 12 - 1;
    walls[23][2] = 4 - 1;
    walls[23][3] = 13 - 1;
    walls[24][0] = 3 - 1;
    walls[24][1] = 13 - 1;
    walls[24][2] = 4 - 1;
    walls[24][3] = 13 - 1;
    beeper[0][0] = 1 - 1;
    beeper[0][1] = 1 - 1;
    beeper[0][2] = 1;
    beeper[1][0] = 2 - 1;
    beeper[1][1] = 1 - 1;
    beeper[1][2] = 1;
    beeper[2][0] = 1 - 1;
    beeper[2][1] = 5 - 1;
    beeper[2][2] = 1;
    beeper[3][0] = 5 - 1;
    beeper[3][1] = 5 - 1;
    beeper[3][2] = 1;
    beeper[4][0] = 1 - 1;
    beeper[4][1] = 13 - 1;
    beeper[4][2] = 1;
    beeper[5][0] = 3 - 1;
    beeper[5][1] = 13 - 1;
    beeper[5][2] = 1;
}

void worldDeathValley()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 10 - 1;
    worldDimension[1] = 10 - 1;
    karelPosition[0] = 5 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 4 - 1;
    walls[0][1] = 1 - 1;
    walls[0][2] = 5 - 1;
    walls[0][3] = 1 - 1;
    walls[1][0] = 4 - 1;
    walls[1][1] = 2 - 1;
    walls[1][2] = 5 - 1;
    walls[1][3] = 2 - 1;
    walls[2][0] = 4 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 4 - 1;
    walls[2][3] = 3 - 1;
    walls[3][0] = 3 - 1;
    walls[3][1] = 3 - 1;
    walls[3][2] = 4 - 1;
    walls[3][3] = 3 - 1;
    walls[4][0] = 3 - 1;
    walls[4][1] = 3 - 1;
    walls[4][2] = 3 - 1;
    walls[4][3] = 4 - 1;
    walls[5][0] = 2 - 1;
    walls[5][1] = 4 - 1;
    walls[5][2] = 3 - 1;
    walls[5][3] = 4 - 1;
    walls[6][0] = 2 - 1;
    walls[6][1] = 4 - 1;
    walls[6][2] = 2 - 1;
    walls[6][3] = 5 - 1;
    walls[7][0] = 1 - 1;
    walls[7][1] = 5 - 1;
    walls[7][2] = 2 - 1;
    walls[7][3] = 5 - 1;
    walls[8][0] = 2 - 1;
    walls[8][1] = 5 - 1;
    walls[8][2] = 2 - 1;
    walls[8][3] = 6 - 1;
    walls[9][0] = 2 - 1;
    walls[9][1] = 6 - 1;
    walls[9][2] = 3 - 1;
    walls[9][3] = 6 - 1;
    walls[10][0] = 3 - 1;
    walls[10][1] = 6 - 1;
    walls[10][2] = 3 - 1;
    walls[10][3] = 7 - 1;
    walls[11][0] = 3 - 1;
    walls[11][1] = 7 - 1;
    walls[11][2] = 4 - 1;
    walls[11][3] = 7 - 1;
    walls[12][0] = 4 - 1;
    walls[12][1] = 7 - 1;
    walls[12][2] = 4 - 1;
    walls[12][3] = 8 - 1;
    walls[13][0] = 4 - 1;
    walls[13][1] = 8 - 1;
    walls[13][2] = 5 - 1;
    walls[13][3] = 8 - 1;
    walls[14][0] = 4 - 1;
    walls[14][1] = 9 - 1;
    walls[14][2] = 5 - 1;
    walls[14][3] = 9 - 1;
    walls[15][0] = 4 - 1;
    walls[15][1] = 10 - 1;
    walls[15][2] = 5 - 1;
    walls[15][3] = 10 - 1;
}

void worldDiamondMining1()
{
    hasUlimitedBeeper = false;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 20 - 1;
    worldDimension[1] = 50 - 1;
    karelPosition[0] = 17 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 16 - 1;
    walls[0][1] = 1 - 1;
    walls[0][2] = 17 - 1;
    walls[0][3] = 1 - 1;
    walls[1][0] = 10 - 1;
    walls[1][1] = 1 - 1;
    walls[1][2] = 10 - 1;
    walls[1][3] = 2 - 1;
    walls[2][0] = 9 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 10 - 1;
    walls[2][3] = 2 - 1;
    walls[3][0] = 10 - 1;
    walls[3][1] = 2 - 1;
    walls[3][2] = 11 - 1;
    walls[3][3] = 2 - 1;
    walls[4][0] = 16 - 1;
    walls[4][1] = 2 - 1;
    walls[4][2] = 17 - 1;
    walls[4][3] = 2 - 1;
    walls[5][0] = 9 - 1;
    walls[5][1] = 3 - 1;
    walls[5][2] = 10 - 1;
    walls[5][3] = 3 - 1;
    walls[6][0] = 10 - 1;
    walls[6][1] = 3 - 1;
    walls[6][2] = 11 - 1;
    walls[6][3] = 3 - 1;
    walls[7][0] = 16 - 1;
    walls[7][1] = 3 - 1;
    walls[7][2] = 17 - 1;
    walls[7][3] = 3 - 1;
    walls[8][0] = 9 - 1;
    walls[8][1] = 4 - 1;
    walls[8][2] = 10 - 1;
    walls[8][3] = 4 - 1;
    walls[9][0] = 11 - 1;
    walls[9][1] = 3 - 1;
    walls[9][2] = 11 - 1;
    walls[9][3] = 4 - 1;
    walls[10][0] = 12 - 1;
    walls[10][1] = 3 - 1;
    walls[10][2] = 12 - 1;
    walls[10][3] = 4 - 1;
    walls[11][0] = 13 - 1;
    walls[11][1] = 3 - 1;
    walls[11][2] = 13 - 1;
    walls[11][3] = 4 - 1;
    walls[12][0] = 14 - 1;
    walls[12][1] = 3 - 1;
    walls[12][2] = 14 - 1;
    walls[12][3] = 4 - 1;
    walls[13][0] = 15 - 1;
    walls[13][1] = 3 - 1;
    walls[13][2] = 15 - 1;
    walls[13][3] = 4 - 1;
    walls[14][0] = 16 - 1;
    walls[14][1] = 3 - 1;
    walls[14][2] = 16 - 1;
    walls[14][3] = 4 - 1;
    walls[15][0] = 4 - 1;
    walls[15][1] = 4 - 1;
    walls[15][2] = 4 - 1;
    walls[15][3] = 5 - 1;
    walls[16][0] = 3 - 1;
    walls[16][1] = 5 - 1;
    walls[16][2] = 4 - 1;
    walls[16][3] = 5 - 1;
    walls[17][0] = 4 - 1;
    walls[17][1] = 5 - 1;
    walls[17][2] = 5 - 1;
    walls[17][3] = 5 - 1;
    walls[18][0] = 9 - 1;
    walls[18][1] = 5 - 1;
    walls[18][2] = 10 - 1;
    walls[18][3] = 5 - 1;
    walls[19][0] = 11 - 1;
    walls[19][1] = 4 - 1;
    walls[19][2] = 11 - 1;
    walls[19][3] = 5 - 1;
    walls[20][0] = 10 - 1;
    walls[20][1] = 5 - 1;
    walls[20][2] = 11 - 1;
    walls[20][3] = 5 - 1;
    walls[21][0] = 12 - 1;
    walls[21][1] = 4 - 1;
    walls[21][2] = 12 - 1;
    walls[21][3] = 5 - 1;
    walls[22][0] = 13 - 1;
    walls[22][1] = 4 - 1;
    walls[22][2] = 13 - 1;
    walls[22][3] = 5 - 1;
    walls[23][0] = 14 - 1;
    walls[23][1] = 4 - 1;
    walls[23][2] = 14 - 1;
    walls[23][3] = 5 - 1;
    walls[24][0] = 15 - 1;
    walls[24][1] = 4 - 1;
    walls[24][2] = 15 - 1;
    walls[24][3] = 5 - 1;
    walls[25][0] = 16 - 1;
    walls[25][1] = 4 - 1;
    walls[25][2] = 16 - 1;
    walls[25][3] = 5 - 1;
    walls[26][0] = 16 - 1;
    walls[26][1] = 5 - 1;
    walls[26][2] = 17 - 1;
    walls[26][3] = 5 - 1;
    walls[27][0] = 3 - 1;
    walls[27][1] = 6 - 1;
    walls[27][2] = 4 - 1;
    walls[27][3] = 6 - 1;
    walls[28][0] = 4 - 1;
    walls[28][1] = 6 - 1;
    walls[28][2] = 5 - 1;
    walls[28][3] = 6 - 1;
    walls[29][0] = 9 - 1;
    walls[29][1] = 6 - 1;
    walls[29][2] = 10 - 1;
    walls[29][3] = 6 - 1;
    walls[30][0] = 10 - 1;
    walls[30][1] = 6 - 1;
    walls[30][2] = 11 - 1;
    walls[30][3] = 6 - 1;
    walls[31][0] = 16 - 1;
    walls[31][1] = 6 - 1;
    walls[31][2] = 17 - 1;
    walls[31][3] = 6 - 1;
    walls[32][0] = 3 - 1;
    walls[32][1] = 7 - 1;
    walls[32][2] = 4 - 1;
    walls[32][3] = 7 - 1;
    walls[33][0] = 4 - 1;
    walls[33][1] = 7 - 1;
    walls[33][2] = 5 - 1;
    walls[33][3] = 7 - 1;
    walls[34][0] = 9 - 1;
    walls[34][1] = 7 - 1;
    walls[34][2] = 10 - 1;
    walls[34][3] = 7 - 1;
    walls[35][0] = 10 - 1;
    walls[35][1] = 7 - 1;
    walls[35][2] = 11 - 1;
    walls[35][3] = 7 - 1;
    walls[36][0] = 16 - 1;
    walls[36][1] = 7 - 1;
    walls[36][2] = 17 - 1;
    walls[36][3] = 7 - 1;
    walls[37][0] = 3 - 1;
    walls[37][1] = 8 - 1;
    walls[37][2] = 4 - 1;
    walls[37][3] = 8 - 1;
    walls[38][0] = 4 - 1;
    walls[38][1] = 8 - 1;
    walls[38][2] = 5 - 1;
    walls[38][3] = 8 - 1;
    walls[39][0] = 9 - 1;
    walls[39][1] = 8 - 1;
    walls[39][2] = 10 - 1;
    walls[39][3] = 8 - 1;
    walls[40][0] = 10 - 1;
    walls[40][1] = 8 - 1;
    walls[40][2] = 11 - 1;
    walls[40][3] = 8 - 1;
    walls[41][0] = 16 - 1;
    walls[41][1] = 8 - 1;
    walls[41][2] = 17 - 1;
    walls[41][3] = 8 - 1;
    walls[42][0] = 3 - 1;
    walls[42][1] = 9 - 1;
    walls[42][2] = 4 - 1;
    walls[42][3] = 9 - 1;
    walls[43][0] = 4 - 1;
    walls[43][1] = 9 - 1;
    walls[43][2] = 5 - 1;
    walls[43][3] = 9 - 1;
    walls[44][0] = 10 - 1;
    walls[44][1] = 8 - 1;
    walls[44][2] = 10 - 1;
    walls[44][3] = 9 - 1;
    walls[45][0] = 16 - 1;
    walls[45][1] = 9 - 1;
    walls[45][2] = 17 - 1;
    walls[45][3] = 9 - 1;
    walls[46][0] = 3 - 1;
    walls[46][1] = 10 - 1;
    walls[46][2] = 4 - 1;
    walls[46][3] = 10 - 1;
    walls[47][0] = 4 - 1;
    walls[47][1] = 10 - 1;
    walls[47][2] = 5 - 1;
    walls[47][3] = 10 - 1;
    walls[48][0] = 16 - 1;
    walls[48][1] = 10 - 1;
    walls[48][2] = 17 - 1;
    walls[48][3] = 10 - 1;
    walls[49][0] = 3 - 1;
    walls[49][1] = 11 - 1;
    walls[49][2] = 4 - 1;
    walls[49][3] = 11 - 1;
    walls[50][0] = 5 - 1;
    walls[50][1] = 10 - 1;
    walls[50][2] = 5 - 1;
    walls[50][3] = 11 - 1;
    walls[51][0] = 6 - 1;
    walls[51][1] = 10 - 1;
    walls[51][2] = 6 - 1;
    walls[51][3] = 11 - 1;
    walls[52][0] = 7 - 1;
    walls[52][1] = 10 - 1;
    walls[52][2] = 7 - 1;
    walls[52][3] = 11 - 1;
    walls[53][0] = 8 - 1;
    walls[53][1] = 10 - 1;
    walls[53][2] = 8 - 1;
    walls[53][3] = 11 - 1;
    walls[54][0] = 9 - 1;
    walls[54][1] = 10 - 1;
    walls[54][2] = 9 - 1;
    walls[54][3] = 11 - 1;
    walls[55][0] = 10 - 1;
    walls[55][1] = 10 - 1;
    walls[55][2] = 10 - 1;
    walls[55][3] = 11 - 1;
    walls[56][0] = 11 - 1;
    walls[56][1] = 10 - 1;
    walls[56][2] = 11 - 1;
    walls[56][3] = 11 - 1;
    walls[57][0] = 12 - 1;
    walls[57][1] = 10 - 1;
    walls[57][2] = 12 - 1;
    walls[57][3] = 11 - 1;
    walls[58][0] = 13 - 1;
    walls[58][1] = 10 - 1;
    walls[58][2] = 13 - 1;
    walls[58][3] = 11 - 1;
    walls[59][0] = 14 - 1;
    walls[59][1] = 10 - 1;
    walls[59][2] = 14 - 1;
    walls[59][3] = 11 - 1;
    walls[60][0] = 15 - 1;
    walls[60][1] = 10 - 1;
    walls[60][2] = 15 - 1;
    walls[60][3] = 11 - 1;
    walls[61][0] = 16 - 1;
    walls[61][1] = 10 - 1;
    walls[61][2] = 16 - 1;
    walls[61][3] = 11 - 1;
    walls[62][0] = 3 - 1;
    walls[62][1] = 12 - 1;
    walls[62][2] = 4 - 1;
    walls[62][3] = 12 - 1;
    walls[63][0] = 5 - 1;
    walls[63][1] = 11 - 1;
    walls[63][2] = 5 - 1;
    walls[63][3] = 12 - 1;
    walls[64][0] = 4 - 1;
    walls[64][1] = 12 - 1;
    walls[64][2] = 5 - 1;
    walls[64][3] = 12 - 1;
    walls[65][0] = 6 - 1;
    walls[65][1] = 11 - 1;
    walls[65][2] = 6 - 1;
    walls[65][3] = 12 - 1;
    walls[66][0] = 7 - 1;
    walls[66][1] = 11 - 1;
    walls[66][2] = 7 - 1;
    walls[66][3] = 12 - 1;
    walls[67][0] = 8 - 1;
    walls[67][1] = 11 - 1;
    walls[67][2] = 8 - 1;
    walls[67][3] = 12 - 1;
    walls[68][0] = 9 - 1;
    walls[68][1] = 11 - 1;
    walls[68][2] = 9 - 1;
    walls[68][3] = 12 - 1;
    walls[69][0] = 10 - 1;
    walls[69][1] = 11 - 1;
    walls[69][2] = 10 - 1;
    walls[69][3] = 12 - 1;
    walls[70][0] = 11 - 1;
    walls[70][1] = 11 - 1;
    walls[70][2] = 11 - 1;
    walls[70][3] = 12 - 1;
    walls[71][0] = 12 - 1;
    walls[71][1] = 11 - 1;
    walls[71][2] = 12 - 1;
    walls[71][3] = 12 - 1;
    walls[72][0] = 13 - 1;
    walls[72][1] = 11 - 1;
    walls[72][2] = 13 - 1;
    walls[72][3] = 12 - 1;
    walls[73][0] = 14 - 1;
    walls[73][1] = 11 - 1;
    walls[73][2] = 14 - 1;
    walls[73][3] = 12 - 1;
    walls[74][0] = 15 - 1;
    walls[74][1] = 11 - 1;
    walls[74][2] = 15 - 1;
    walls[74][3] = 12 - 1;
    walls[75][0] = 16 - 1;
    walls[75][1] = 11 - 1;
    walls[75][2] = 16 - 1;
    walls[75][3] = 12 - 1;
    walls[76][0] = 16 - 1;
    walls[76][1] = 12 - 1;
    walls[76][2] = 17 - 1;
    walls[76][3] = 12 - 1;
    walls[77][0] = 3 - 1;
    walls[77][1] = 13 - 1;
    walls[77][2] = 4 - 1;
    walls[77][3] = 13 - 1;
    walls[78][0] = 4 - 1;
    walls[78][1] = 13 - 1;
    walls[78][2] = 5 - 1;
    walls[78][3] = 13 - 1;
    walls[79][0] = 16 - 1;
    walls[79][1] = 13 - 1;
    walls[79][2] = 17 - 1;
    walls[79][3] = 13 - 1;
    walls[80][0] = 4 - 1;
    walls[80][1] = 13 - 1;
    walls[80][2] = 4 - 1;
    walls[80][3] = 14 - 1;
    walls[81][0] = 16 - 1;
    walls[81][1] = 14 - 1;
    walls[81][2] = 17 - 1;
    walls[81][3] = 14 - 1;
    walls[82][0] = 16 - 1;
    walls[82][1] = 15 - 1;
    walls[82][2] = 17 - 1;
    walls[82][3] = 15 - 1;
    walls[83][0] = 16 - 1;
    walls[83][1] = 16 - 1;
    walls[83][2] = 17 - 1;
    walls[83][3] = 16 - 1;
    walls[84][0] = 14 - 1;
    walls[84][1] = 16 - 1;
    walls[84][2] = 14 - 1;
    walls[84][3] = 17 - 1;
    walls[85][0] = 13 - 1;
    walls[85][1] = 17 - 1;
    walls[85][2] = 14 - 1;
    walls[85][3] = 17 - 1;
    walls[86][0] = 14 - 1;
    walls[86][1] = 17 - 1;
    walls[86][2] = 15 - 1;
    walls[86][3] = 17 - 1;
    walls[87][0] = 16 - 1;
    walls[87][1] = 17 - 1;
    walls[87][2] = 17 - 1;
    walls[87][3] = 17 - 1;
    walls[88][0] = 13 - 1;
    walls[88][1] = 18 - 1;
    walls[88][2] = 14 - 1;
    walls[88][3] = 18 - 1;
    walls[89][0] = 14 - 1;
    walls[89][1] = 18 - 1;
    walls[89][2] = 15 - 1;
    walls[89][3] = 18 - 1;
    walls[90][0] = 16 - 1;
    walls[90][1] = 18 - 1;
    walls[90][2] = 17 - 1;
    walls[90][3] = 18 - 1;
    walls[91][0] = 13 - 1;
    walls[91][1] = 19 - 1;
    walls[91][2] = 14 - 1;
    walls[91][3] = 19 - 1;
    walls[92][0] = 14 - 1;
    walls[92][1] = 19 - 1;
    walls[92][2] = 15 - 1;
    walls[92][3] = 19 - 1;
    walls[93][0] = 16 - 1;
    walls[93][1] = 19 - 1;
    walls[93][2] = 17 - 1;
    walls[93][3] = 19 - 1;
    walls[94][0] = 13 - 1;
    walls[94][1] = 20 - 1;
    walls[94][2] = 14 - 1;
    walls[94][3] = 20 - 1;
    walls[95][0] = 14 - 1;
    walls[95][1] = 20 - 1;
    walls[95][2] = 15 - 1;
    walls[95][3] = 20 - 1;
    walls[96][0] = 16 - 1;
    walls[96][1] = 20 - 1;
    walls[96][2] = 17 - 1;
    walls[96][3] = 20 - 1;
    walls[97][0] = 2 - 1;
    walls[97][1] = 20 - 1;
    walls[97][2] = 2 - 1;
    walls[97][3] = 21 - 1;
    walls[98][0] = 1 - 1;
    walls[98][1] = 21 - 1;
    walls[98][2] = 2 - 1;
    walls[98][3] = 21 - 1;
    walls[99][0] = 2 - 1;
    walls[99][1] = 21 - 1;
    walls[99][2] = 3 - 1;
    walls[99][3] = 21 - 1;
    walls[100][0] = 13 - 1;
    walls[100][1] = 21 - 1;
    walls[100][2] = 14 - 1;
    walls[100][3] = 21 - 1;
    walls[101][0] = 15 - 1;
    walls[101][1] = 20 - 1;
    walls[101][2] = 15 - 1;
    walls[101][3] = 21 - 1;
    walls[102][0] = 16 - 1;
    walls[102][1] = 20 - 1;
    walls[102][2] = 16 - 1;
    walls[102][3] = 21 - 1;
    walls[103][0] = 1 - 1;
    walls[103][1] = 22 - 1;
    walls[103][2] = 2 - 1;
    walls[103][3] = 22 - 1;
    walls[104][0] = 2 - 1;
    walls[104][1] = 22 - 1;
    walls[104][2] = 3 - 1;
    walls[104][3] = 22 - 1;
    walls[105][0] = 13 - 1;
    walls[105][1] = 22 - 1;
    walls[105][2] = 14 - 1;
    walls[105][3] = 22 - 1;
    walls[106][0] = 15 - 1;
    walls[106][1] = 21 - 1;
    walls[106][2] = 15 - 1;
    walls[106][3] = 22 - 1;
    walls[107][0] = 14 - 1;
    walls[107][1] = 22 - 1;
    walls[107][2] = 15 - 1;
    walls[107][3] = 22 - 1;
    walls[108][0] = 16 - 1;
    walls[108][1] = 21 - 1;
    walls[108][2] = 16 - 1;
    walls[108][3] = 22 - 1;
    walls[109][0] = 16 - 1;
    walls[109][1] = 22 - 1;
    walls[109][2] = 17 - 1;
    walls[109][3] = 22 - 1;
    walls[110][0] = 1 - 1;
    walls[110][1] = 23 - 1;
    walls[110][2] = 2 - 1;
    walls[110][3] = 23 - 1;
    walls[111][0] = 2 - 1;
    walls[111][1] = 23 - 1;
    walls[111][2] = 3 - 1;
    walls[111][3] = 23 - 1;
    walls[112][0] = 13 - 1;
    walls[112][1] = 23 - 1;
    walls[112][2] = 14 - 1;
    walls[112][3] = 23 - 1;
    walls[113][0] = 14 - 1;
    walls[113][1] = 23 - 1;
    walls[113][2] = 15 - 1;
    walls[113][3] = 23 - 1;
    walls[114][0] = 16 - 1;
    walls[114][1] = 23 - 1;
    walls[114][2] = 17 - 1;
    walls[114][3] = 23 - 1;
    walls[115][0] = 1 - 1;
    walls[115][1] = 24 - 1;
    walls[115][2] = 2 - 1;
    walls[115][3] = 24 - 1;
    walls[116][0] = 2 - 1;
    walls[116][1] = 24 - 1;
    walls[116][2] = 3 - 1;
    walls[116][3] = 24 - 1;
    walls[117][0] = 13 - 1;
    walls[117][1] = 24 - 1;
    walls[117][2] = 14 - 1;
    walls[117][3] = 24 - 1;
    walls[118][0] = 14 - 1;
    walls[118][1] = 24 - 1;
    walls[118][2] = 15 - 1;
    walls[118][3] = 24 - 1;
    walls[119][0] = 16 - 1;
    walls[119][1] = 24 - 1;
    walls[119][2] = 17 - 1;
    walls[119][3] = 24 - 1;
    walls[120][0] = 1 - 1;
    walls[120][1] = 25 - 1;
    walls[120][2] = 2 - 1;
    walls[120][3] = 25 - 1;
    walls[121][0] = 2 - 1;
    walls[121][1] = 25 - 1;
    walls[121][2] = 3 - 1;
    walls[121][3] = 25 - 1;
    walls[122][0] = 13 - 1;
    walls[122][1] = 25 - 1;
    walls[122][2] = 14 - 1;
    walls[122][3] = 25 - 1;
    walls[123][0] = 14 - 1;
    walls[123][1] = 25 - 1;
    walls[123][2] = 15 - 1;
    walls[123][3] = 25 - 1;
    walls[124][0] = 16 - 1;
    walls[124][1] = 25 - 1;
    walls[124][2] = 17 - 1;
    walls[124][3] = 25 - 1;
    walls[125][0] = 1 - 1;
    walls[125][1] = 26 - 1;
    walls[125][2] = 2 - 1;
    walls[125][3] = 26 - 1;
    walls[126][0] = 2 - 1;
    walls[126][1] = 26 - 1;
    walls[126][2] = 3 - 1;
    walls[126][3] = 26 - 1;
    walls[127][0] = 14 - 1;
    walls[127][1] = 25 - 1;
    walls[127][2] = 14 - 1;
    walls[127][3] = 26 - 1;
    walls[128][0] = 16 - 1;
    walls[128][1] = 26 - 1;
    walls[128][2] = 17 - 1;
    walls[128][3] = 26 - 1;
    walls[129][0] = 1 - 1;
    walls[129][1] = 27 - 1;
    walls[129][2] = 2 - 1;
    walls[129][3] = 27 - 1;
    walls[130][0] = 2 - 1;
    walls[130][1] = 27 - 1;
    walls[130][2] = 3 - 1;
    walls[130][3] = 27 - 1;
    walls[131][0] = 16 - 1;
    walls[131][1] = 27 - 1;
    walls[131][2] = 17 - 1;
    walls[131][3] = 27 - 1;
    walls[132][0] = 1 - 1;
    walls[132][1] = 28 - 1;
    walls[132][2] = 2 - 1;
    walls[132][3] = 28 - 1;
    walls[133][0] = 2 - 1;
    walls[133][1] = 28 - 1;
    walls[133][2] = 3 - 1;
    walls[133][3] = 28 - 1;
    walls[134][0] = 8 - 1;
    walls[134][1] = 27 - 1;
    walls[134][2] = 8 - 1;
    walls[134][3] = 28 - 1;
    walls[135][0] = 7 - 1;
    walls[135][1] = 28 - 1;
    walls[135][2] = 8 - 1;
    walls[135][3] = 28 - 1;
    walls[136][0] = 8 - 1;
    walls[136][1] = 28 - 1;
    walls[136][2] = 9 - 1;
    walls[136][3] = 28 - 1;
    walls[137][0] = 16 - 1;
    walls[137][1] = 28 - 1;
    walls[137][2] = 17 - 1;
    walls[137][3] = 28 - 1;
    walls[138][0] = 1 - 1;
    walls[138][1] = 29 - 1;
    walls[138][2] = 2 - 1;
    walls[138][3] = 29 - 1;
    walls[139][0] = 2 - 1;
    walls[139][1] = 29 - 1;
    walls[139][2] = 3 - 1;
    walls[139][3] = 29 - 1;
    walls[140][0] = 7 - 1;
    walls[140][1] = 29 - 1;
    walls[140][2] = 8 - 1;
    walls[140][3] = 29 - 1;
    walls[141][0] = 9 - 1;
    walls[141][1] = 28 - 1;
    walls[141][2] = 9 - 1;
    walls[141][3] = 29 - 1;
    walls[142][0] = 10 - 1;
    walls[142][1] = 28 - 1;
    walls[142][2] = 10 - 1;
    walls[142][3] = 29 - 1;
    walls[143][0] = 11 - 1;
    walls[143][1] = 28 - 1;
    walls[143][2] = 11 - 1;
    walls[143][3] = 29 - 1;
    walls[144][0] = 12 - 1;
    walls[144][1] = 28 - 1;
    walls[144][2] = 12 - 1;
    walls[144][3] = 29 - 1;
    walls[145][0] = 13 - 1;
    walls[145][1] = 28 - 1;
    walls[145][2] = 13 - 1;
    walls[145][3] = 29 - 1;
    walls[146][0] = 14 - 1;
    walls[146][1] = 28 - 1;
    walls[146][2] = 14 - 1;
    walls[146][3] = 29 - 1;
    walls[147][0] = 15 - 1;
    walls[147][1] = 28 - 1;
    walls[147][2] = 15 - 1;
    walls[147][3] = 29 - 1;
    walls[148][0] = 16 - 1;
    walls[148][1] = 28 - 1;
    walls[148][2] = 16 - 1;
    walls[148][3] = 29 - 1;
    walls[149][0] = 1 - 1;
    walls[149][1] = 30 - 1;
    walls[149][2] = 2 - 1;
    walls[149][3] = 30 - 1;
    walls[150][0] = 2 - 1;
    walls[150][1] = 30 - 1;
    walls[150][2] = 3 - 1;
    walls[150][3] = 30 - 1;
    walls[151][0] = 7 - 1;
    walls[151][1] = 30 - 1;
    walls[151][2] = 8 - 1;
    walls[151][3] = 30 - 1;
    walls[152][0] = 9 - 1;
    walls[152][1] = 29 - 1;
    walls[152][2] = 9 - 1;
    walls[152][3] = 30 - 1;
    walls[153][0] = 8 - 1;
    walls[153][1] = 30 - 1;
    walls[153][2] = 9 - 1;
    walls[153][3] = 30 - 1;
    walls[154][0] = 10 - 1;
    walls[154][1] = 29 - 1;
    walls[154][2] = 10 - 1;
    walls[154][3] = 30 - 1;
    walls[155][0] = 11 - 1;
    walls[155][1] = 29 - 1;
    walls[155][2] = 11 - 1;
    walls[155][3] = 30 - 1;
    walls[156][0] = 12 - 1;
    walls[156][1] = 29 - 1;
    walls[156][2] = 12 - 1;
    walls[156][3] = 30 - 1;
    walls[157][0] = 13 - 1;
    walls[157][1] = 29 - 1;
    walls[157][2] = 13 - 1;
    walls[157][3] = 30 - 1;
    walls[158][0] = 14 - 1;
    walls[158][1] = 29 - 1;
    walls[158][2] = 14 - 1;
    walls[158][3] = 30 - 1;
    walls[159][0] = 15 - 1;
    walls[159][1] = 29 - 1;
    walls[159][2] = 15 - 1;
    walls[159][3] = 30 - 1;
    walls[160][0] = 16 - 1;
    walls[160][1] = 29 - 1;
    walls[160][2] = 16 - 1;
    walls[160][3] = 30 - 1;
    walls[161][0] = 16 - 1;
    walls[161][1] = 30 - 1;
    walls[161][2] = 17 - 1;
    walls[161][3] = 30 - 1;
    walls[162][0] = 1 - 1;
    walls[162][1] = 31 - 1;
    walls[162][2] = 2 - 1;
    walls[162][3] = 31 - 1;
    walls[163][0] = 2 - 1;
    walls[163][1] = 31 - 1;
    walls[163][2] = 3 - 1;
    walls[163][3] = 31 - 1;
    walls[164][0] = 7 - 1;
    walls[164][1] = 31 - 1;
    walls[164][2] = 8 - 1;
    walls[164][3] = 31 - 1;
    walls[165][0] = 8 - 1;
    walls[165][1] = 31 - 1;
    walls[165][2] = 9 - 1;
    walls[165][3] = 31 - 1;
    walls[166][0] = 16 - 1;
    walls[166][1] = 31 - 1;
    walls[166][2] = 17 - 1;
    walls[166][3] = 31 - 1;
    walls[167][0] = 1 - 1;
    walls[167][1] = 32 - 1;
    walls[167][2] = 2 - 1;
    walls[167][3] = 32 - 1;
    walls[168][0] = 2 - 1;
    walls[168][1] = 32 - 1;
    walls[168][2] = 3 - 1;
    walls[168][3] = 32 - 1;
    walls[169][0] = 7 - 1;
    walls[169][1] = 32 - 1;
    walls[169][2] = 8 - 1;
    walls[169][3] = 32 - 1;
    walls[170][0] = 8 - 1;
    walls[170][1] = 32 - 1;
    walls[170][2] = 9 - 1;
    walls[170][3] = 32 - 1;
    walls[171][0] = 16 - 1;
    walls[171][1] = 32 - 1;
    walls[171][2] = 17 - 1;
    walls[171][3] = 32 - 1;
    walls[172][0] = 1 - 1;
    walls[172][1] = 33 - 1;
    walls[172][2] = 2 - 1;
    walls[172][3] = 33 - 1;
    walls[173][0] = 2 - 1;
    walls[173][1] = 33 - 1;
    walls[173][2] = 3 - 1;
    walls[173][3] = 33 - 1;
    walls[174][0] = 7 - 1;
    walls[174][1] = 33 - 1;
    walls[174][2] = 8 - 1;
    walls[174][3] = 33 - 1;
    walls[175][0] = 8 - 1;
    walls[175][1] = 33 - 1;
    walls[175][2] = 9 - 1;
    walls[175][3] = 33 - 1;
    walls[176][0] = 16 - 1;
    walls[176][1] = 33 - 1;
    walls[176][2] = 17 - 1;
    walls[176][3] = 33 - 1;
    walls[177][0] = 1 - 1;
    walls[177][1] = 34 - 1;
    walls[177][2] = 2 - 1;
    walls[177][3] = 34 - 1;
    walls[178][0] = 2 - 1;
    walls[178][1] = 34 - 1;
    walls[178][2] = 3 - 1;
    walls[178][3] = 34 - 1;
    walls[179][0] = 8 - 1;
    walls[179][1] = 33 - 1;
    walls[179][2] = 8 - 1;
    walls[179][3] = 34 - 1;
    walls[180][0] = 16 - 1;
    walls[180][1] = 34 - 1;
    walls[180][2] = 17 - 1;
    walls[180][3] = 34 - 1;
    walls[181][0] = 1 - 1;
    walls[181][1] = 35 - 1;
    walls[181][2] = 2 - 1;
    walls[181][3] = 35 - 1;
    walls[182][0] = 2 - 1;
    walls[182][1] = 35 - 1;
    walls[182][2] = 3 - 1;
    walls[182][3] = 35 - 1;
    walls[183][0] = 16 - 1;
    walls[183][1] = 35 - 1;
    walls[183][2] = 17 - 1;
    walls[183][3] = 35 - 1;
    walls[184][0] = 1 - 1;
    walls[184][1] = 36 - 1;
    walls[184][2] = 2 - 1;
    walls[184][3] = 36 - 1;
    walls[185][0] = 2 - 1;
    walls[185][1] = 36 - 1;
    walls[185][2] = 3 - 1;
    walls[185][3] = 36 - 1;
    walls[186][0] = 16 - 1;
    walls[186][1] = 36 - 1;
    walls[186][2] = 17 - 1;
    walls[186][3] = 36 - 1;
    walls[187][0] = 1 - 1;
    walls[187][1] = 37 - 1;
    walls[187][2] = 2 - 1;
    walls[187][3] = 37 - 1;
    walls[188][0] = 3 - 1;
    walls[188][1] = 36 - 1;
    walls[188][2] = 3 - 1;
    walls[188][3] = 37 - 1;
    walls[189][0] = 4 - 1;
    walls[189][1] = 36 - 1;
    walls[189][2] = 4 - 1;
    walls[189][3] = 37 - 1;
    walls[190][0] = 5 - 1;
    walls[190][1] = 36 - 1;
    walls[190][2] = 5 - 1;
    walls[190][3] = 37 - 1;
    walls[191][0] = 6 - 1;
    walls[191][1] = 36 - 1;
    walls[191][2] = 6 - 1;
    walls[191][3] = 37 - 1;
    walls[192][0] = 7 - 1;
    walls[192][1] = 36 - 1;
    walls[192][2] = 7 - 1;
    walls[192][3] = 37 - 1;
    walls[193][0] = 8 - 1;
    walls[193][1] = 36 - 1;
    walls[193][2] = 8 - 1;
    walls[193][3] = 37 - 1;
    walls[194][0] = 9 - 1;
    walls[194][1] = 36 - 1;
    walls[194][2] = 9 - 1;
    walls[194][3] = 37 - 1;
    walls[195][0] = 10 - 1;
    walls[195][1] = 36 - 1;
    walls[195][2] = 10 - 1;
    walls[195][3] = 37 - 1;
    walls[196][0] = 11 - 1;
    walls[196][1] = 36 - 1;
    walls[196][2] = 11 - 1;
    walls[196][3] = 37 - 1;
    walls[197][0] = 12 - 1;
    walls[197][1] = 36 - 1;
    walls[197][2] = 12 - 1;
    walls[197][3] = 37 - 1;
    walls[198][0] = 13 - 1;
    walls[198][1] = 36 - 1;
    walls[198][2] = 13 - 1;
    walls[198][3] = 37 - 1;
    walls[199][0] = 14 - 1;
    walls[199][1] = 36 - 1;
    walls[199][2] = 14 - 1;
    walls[199][3] = 37 - 1;
    walls[200][0] = 15 - 1;
    walls[200][1] = 36 - 1;
    walls[200][2] = 15 - 1;
    walls[200][3] = 37 - 1;
    walls[201][0] = 16 - 1;
    walls[201][1] = 36 - 1;
    walls[201][2] = 16 - 1;
    walls[201][3] = 37 - 1;
    walls[202][0] = 1 - 1;
    walls[202][1] = 38 - 1;
    walls[202][2] = 2 - 1;
    walls[202][3] = 38 - 1;
    walls[203][0] = 3 - 1;
    walls[203][1] = 37 - 1;
    walls[203][2] = 3 - 1;
    walls[203][3] = 38 - 1;
    walls[204][0] = 2 - 1;
    walls[204][1] = 38 - 1;
    walls[204][2] = 3 - 1;
    walls[204][3] = 38 - 1;
    walls[205][0] = 4 - 1;
    walls[205][1] = 37 - 1;
    walls[205][2] = 4 - 1;
    walls[205][3] = 38 - 1;
    walls[206][0] = 5 - 1;
    walls[206][1] = 37 - 1;
    walls[206][2] = 5 - 1;
    walls[206][3] = 38 - 1;
    walls[207][0] = 6 - 1;
    walls[207][1] = 37 - 1;
    walls[207][2] = 6 - 1;
    walls[207][3] = 38 - 1;
    walls[208][0] = 7 - 1;
    walls[208][1] = 37 - 1;
    walls[208][2] = 7 - 1;
    walls[208][3] = 38 - 1;
    walls[209][0] = 8 - 1;
    walls[209][1] = 37 - 1;
    walls[209][2] = 8 - 1;
    walls[209][3] = 38 - 1;
    walls[210][0] = 9 - 1;
    walls[210][1] = 37 - 1;
    walls[210][2] = 9 - 1;
    walls[210][3] = 38 - 1;
    walls[211][0] = 10 - 1;
    walls[211][1] = 37 - 1;
    walls[211][2] = 10 - 1;
    walls[211][3] = 38 - 1;
    walls[212][0] = 11 - 1;
    walls[212][1] = 37 - 1;
    walls[212][2] = 11 - 1;
    walls[212][3] = 38 - 1;
    walls[213][0] = 12 - 1;
    walls[213][1] = 37 - 1;
    walls[213][2] = 12 - 1;
    walls[213][3] = 38 - 1;
    walls[214][0] = 13 - 1;
    walls[214][1] = 37 - 1;
    walls[214][2] = 13 - 1;
    walls[214][3] = 38 - 1;
    walls[215][0] = 14 - 1;
    walls[215][1] = 37 - 1;
    walls[215][2] = 14 - 1;
    walls[215][3] = 38 - 1;
    walls[216][0] = 15 - 1;
    walls[216][1] = 37 - 1;
    walls[216][2] = 15 - 1;
    walls[216][3] = 38 - 1;
    walls[217][0] = 16 - 1;
    walls[217][1] = 37 - 1;
    walls[217][2] = 16 - 1;
    walls[217][3] = 38 - 1;
    walls[218][0] = 16 - 1;
    walls[218][1] = 38 - 1;
    walls[218][2] = 17 - 1;
    walls[218][3] = 38 - 1;
    walls[219][0] = 1 - 1;
    walls[219][1] = 39 - 1;
    walls[219][2] = 2 - 1;
    walls[219][3] = 39 - 1;
    walls[220][0] = 2 - 1;
    walls[220][1] = 39 - 1;
    walls[220][2] = 3 - 1;
    walls[220][3] = 39 - 1;
    walls[221][0] = 14 - 1;
    walls[221][1] = 38 - 1;
    walls[221][2] = 14 - 1;
    walls[221][3] = 39 - 1;
    walls[222][0] = 13 - 1;
    walls[222][1] = 39 - 1;
    walls[222][2] = 14 - 1;
    walls[222][3] = 39 - 1;
    walls[223][0] = 14 - 1;
    walls[223][1] = 39 - 1;
    walls[223][2] = 15 - 1;
    walls[223][3] = 39 - 1;
    walls[224][0] = 16 - 1;
    walls[224][1] = 39 - 1;
    walls[224][2] = 17 - 1;
    walls[224][3] = 39 - 1;
    walls[225][0] = 1 - 1;
    walls[225][1] = 40 - 1;
    walls[225][2] = 2 - 1;
    walls[225][3] = 40 - 1;
    walls[226][0] = 2 - 1;
    walls[226][1] = 40 - 1;
    walls[226][2] = 3 - 1;
    walls[226][3] = 40 - 1;
    walls[227][0] = 13 - 1;
    walls[227][1] = 40 - 1;
    walls[227][2] = 14 - 1;
    walls[227][3] = 40 - 1;
    walls[228][0] = 14 - 1;
    walls[228][1] = 40 - 1;
    walls[228][2] = 15 - 1;
    walls[228][3] = 40 - 1;
    walls[229][0] = 16 - 1;
    walls[229][1] = 40 - 1;
    walls[229][2] = 17 - 1;
    walls[229][3] = 40 - 1;
    walls[230][0] = 1 - 1;
    walls[230][1] = 41 - 1;
    walls[230][2] = 2 - 1;
    walls[230][3] = 41 - 1;
    walls[231][0] = 2 - 1;
    walls[231][1] = 41 - 1;
    walls[231][2] = 3 - 1;
    walls[231][3] = 41 - 1;
    walls[232][0] = 13 - 1;
    walls[232][1] = 41 - 1;
    walls[232][2] = 14 - 1;
    walls[232][3] = 41 - 1;
    walls[233][0] = 15 - 1;
    walls[233][1] = 40 - 1;
    walls[233][2] = 15 - 1;
    walls[233][3] = 41 - 1;
    walls[234][0] = 16 - 1;
    walls[234][1] = 40 - 1;
    walls[234][2] = 16 - 1;
    walls[234][3] = 41 - 1;
    walls[235][0] = 1 - 1;
    walls[235][1] = 42 - 1;
    walls[235][2] = 2 - 1;
    walls[235][3] = 42 - 1;
    walls[236][0] = 2 - 1;
    walls[236][1] = 42 - 1;
    walls[236][2] = 3 - 1;
    walls[236][3] = 42 - 1;
    walls[237][0] = 13 - 1;
    walls[237][1] = 42 - 1;
    walls[237][2] = 14 - 1;
    walls[237][3] = 42 - 1;
    walls[238][0] = 15 - 1;
    walls[238][1] = 41 - 1;
    walls[238][2] = 15 - 1;
    walls[238][3] = 42 - 1;
    walls[239][0] = 14 - 1;
    walls[239][1] = 42 - 1;
    walls[239][2] = 15 - 1;
    walls[239][3] = 42 - 1;
    walls[240][0] = 16 - 1;
    walls[240][1] = 41 - 1;
    walls[240][2] = 16 - 1;
    walls[240][3] = 42 - 1;
    walls[241][0] = 16 - 1;
    walls[241][1] = 42 - 1;
    walls[241][2] = 17 - 1;
    walls[241][3] = 42 - 1;
    walls[242][0] = 1 - 1;
    walls[242][1] = 43 - 1;
    walls[242][2] = 2 - 1;
    walls[242][3] = 43 - 1;
    walls[243][0] = 2 - 1;
    walls[243][1] = 43 - 1;
    walls[243][2] = 3 - 1;
    walls[243][3] = 43 - 1;
    walls[244][0] = 8 - 1;
    walls[244][1] = 42 - 1;
    walls[244][2] = 8 - 1;
    walls[244][3] = 43 - 1;
    walls[245][0] = 7 - 1;
    walls[245][1] = 43 - 1;
    walls[245][2] = 8 - 1;
    walls[245][3] = 43 - 1;
    walls[246][0] = 8 - 1;
    walls[246][1] = 43 - 1;
    walls[246][2] = 9 - 1;
    walls[246][3] = 43 - 1;
    walls[247][0] = 13 - 1;
    walls[247][1] = 43 - 1;
    walls[247][2] = 14 - 1;
    walls[247][3] = 43 - 1;
    walls[248][0] = 14 - 1;
    walls[248][1] = 43 - 1;
    walls[248][2] = 15 - 1;
    walls[248][3] = 43 - 1;
    walls[249][0] = 16 - 1;
    walls[249][1] = 43 - 1;
    walls[249][2] = 17 - 1;
    walls[249][3] = 43 - 1;
    walls[250][0] = 1 - 1;
    walls[250][1] = 44 - 1;
    walls[250][2] = 2 - 1;
    walls[250][3] = 44 - 1;
    walls[251][0] = 2 - 1;
    walls[251][1] = 44 - 1;
    walls[251][2] = 3 - 1;
    walls[251][3] = 44 - 1;
    walls[252][0] = 7 - 1;
    walls[252][1] = 44 - 1;
    walls[252][2] = 8 - 1;
    walls[252][3] = 44 - 1;
    walls[253][0] = 8 - 1;
    walls[253][1] = 44 - 1;
    walls[253][2] = 9 - 1;
    walls[253][3] = 44 - 1;
    walls[254][0] = 13 - 1;
    walls[254][1] = 44 - 1;
    walls[254][2] = 14 - 1;
    walls[254][3] = 44 - 1;
    walls[255][0] = 14 - 1;
    walls[255][1] = 44 - 1;
    walls[255][2] = 15 - 1;
    walls[255][3] = 44 - 1;
    walls[256][0] = 16 - 1;
    walls[256][1] = 44 - 1;
    walls[256][2] = 17 - 1;
    walls[256][3] = 44 - 1;
    walls[257][0] = 1 - 1;
    walls[257][1] = 45 - 1;
    walls[257][2] = 2 - 1;
    walls[257][3] = 45 - 1;
    walls[258][0] = 2 - 1;
    walls[258][1] = 45 - 1;
    walls[258][2] = 3 - 1;
    walls[258][3] = 45 - 1;
    walls[259][0] = 7 - 1;
    walls[259][1] = 45 - 1;
    walls[259][2] = 8 - 1;
    walls[259][3] = 45 - 1;
    walls[260][0] = 8 - 1;
    walls[260][1] = 45 - 1;
    walls[260][2] = 9 - 1;
    walls[260][3] = 45 - 1;
    walls[261][0] = 14 - 1;
    walls[261][1] = 44 - 1;
    walls[261][2] = 14 - 1;
    walls[261][3] = 45 - 1;
    walls[262][0] = 16 - 1;
    walls[262][1] = 45 - 1;
    walls[262][2] = 17 - 1;
    walls[262][3] = 45 - 1;
    walls[263][0] = 1 - 1;
    walls[263][1] = 46 - 1;
    walls[263][2] = 2 - 1;
    walls[263][3] = 46 - 1;
    walls[264][0] = 2 - 1;
    walls[264][1] = 46 - 1;
    walls[264][2] = 3 - 1;
    walls[264][3] = 46 - 1;
    walls[265][0] = 7 - 1;
    walls[265][1] = 46 - 1;
    walls[265][2] = 8 - 1;
    walls[265][3] = 46 - 1;
    walls[266][0] = 9 - 1;
    walls[266][1] = 45 - 1;
    walls[266][2] = 9 - 1;
    walls[266][3] = 46 - 1;
    walls[267][0] = 10 - 1;
    walls[267][1] = 45 - 1;
    walls[267][2] = 10 - 1;
    walls[267][3] = 46 - 1;
    walls[268][0] = 11 - 1;
    walls[268][1] = 45 - 1;
    walls[268][2] = 11 - 1;
    walls[268][3] = 46 - 1;
    walls[269][0] = 12 - 1;
    walls[269][1] = 45 - 1;
    walls[269][2] = 12 - 1;
    walls[269][3] = 46 - 1;
    walls[270][0] = 13 - 1;
    walls[270][1] = 45 - 1;
    walls[270][2] = 13 - 1;
    walls[270][3] = 46 - 1;
    walls[271][0] = 14 - 1;
    walls[271][1] = 45 - 1;
    walls[271][2] = 14 - 1;
    walls[271][3] = 46 - 1;
    walls[272][0] = 15 - 1;
    walls[272][1] = 45 - 1;
    walls[272][2] = 15 - 1;
    walls[272][3] = 46 - 1;
    walls[273][0] = 16 - 1;
    walls[273][1] = 45 - 1;
    walls[273][2] = 16 - 1;
    walls[273][3] = 46 - 1;
    walls[274][0] = 1 - 1;
    walls[274][1] = 47 - 1;
    walls[274][2] = 2 - 1;
    walls[274][3] = 47 - 1;
    walls[275][0] = 2 - 1;
    walls[275][1] = 47 - 1;
    walls[275][2] = 3 - 1;
    walls[275][3] = 47 - 1;
    walls[276][0] = 7 - 1;
    walls[276][1] = 47 - 1;
    walls[276][2] = 8 - 1;
    walls[276][3] = 47 - 1;
    walls[277][0] = 9 - 1;
    walls[277][1] = 46 - 1;
    walls[277][2] = 9 - 1;
    walls[277][3] = 47 - 1;
    walls[278][0] = 8 - 1;
    walls[278][1] = 47 - 1;
    walls[278][2] = 9 - 1;
    walls[278][3] = 47 - 1;
    walls[279][0] = 10 - 1;
    walls[279][1] = 46 - 1;
    walls[279][2] = 10 - 1;
    walls[279][3] = 47 - 1;
    walls[280][0] = 11 - 1;
    walls[280][1] = 46 - 1;
    walls[280][2] = 11 - 1;
    walls[280][3] = 47 - 1;
    walls[281][0] = 12 - 1;
    walls[281][1] = 46 - 1;
    walls[281][2] = 12 - 1;
    walls[281][3] = 47 - 1;
    walls[282][0] = 13 - 1;
    walls[282][1] = 46 - 1;
    walls[282][2] = 13 - 1;
    walls[282][3] = 47 - 1;
    walls[283][0] = 14 - 1;
    walls[283][1] = 46 - 1;
    walls[283][2] = 14 - 1;
    walls[283][3] = 47 - 1;
    walls[284][0] = 15 - 1;
    walls[284][1] = 46 - 1;
    walls[284][2] = 15 - 1;
    walls[284][3] = 47 - 1;
    walls[285][0] = 16 - 1;
    walls[285][1] = 46 - 1;
    walls[285][2] = 16 - 1;
    walls[285][3] = 47 - 1;
    walls[286][0] = 16 - 1;
    walls[286][1] = 47 - 1;
    walls[286][2] = 17 - 1;
    walls[286][3] = 47 - 1;
    walls[287][0] = 2 - 1;
    walls[287][1] = 47 - 1;
    walls[287][2] = 2 - 1;
    walls[287][3] = 48 - 1;
    walls[288][0] = 8 - 1;
    walls[288][1] = 47 - 1;
    walls[288][2] = 8 - 1;
    walls[288][3] = 48 - 1;
    walls[289][0] = 16 - 1;
    walls[289][1] = 48 - 1;
    walls[289][2] = 17 - 1;
    walls[289][3] = 48 - 1;
    walls[290][0] = 16 - 1;
    walls[290][1] = 49 - 1;
    walls[290][2] = 17 - 1;
    walls[290][3] = 49 - 1;
    walls[291][0] = 16 - 1;
    walls[291][1] = 50 - 1;
    walls[291][2] = 17 - 1;
    walls[291][3] = 50 - 1;
    beeper[0][0] = 10 - 1;
    beeper[0][1] = 3 - 1;
    beeper[0][2] = 1;
    beeper[1][0] = 10 - 1;
    beeper[1][1] = 4 - 1;
    beeper[1][2] = 1;
    beeper[2][0] = 4 - 1;
    beeper[2][1] = 6 - 1;
    beeper[2][2] = 1;
    beeper[3][0] = 10 - 1;
    beeper[3][1] = 6 - 1;
    beeper[3][2] = 1;
    beeper[4][0] = 4 - 1;
    beeper[4][1] = 7 - 1;
    beeper[4][2] = 1;
    beeper[5][0] = 4 - 1;
    beeper[5][1] = 8 - 1;
    beeper[5][2] = 1;
    beeper[6][0] = 4 - 1;
    beeper[6][1] = 13 - 1;
    beeper[6][2] = 1;
    beeper[7][0] = 2 - 1;
    beeper[7][1] = 22 - 1;
    beeper[7][2] = 1;
    beeper[8][0] = 2 - 1;
    beeper[8][1] = 23 - 1;
    beeper[8][2] = 1;
    beeper[9][0] = 14 - 1;
    beeper[9][1] = 23 - 1;
    beeper[9][2] = 1;
    beeper[10][0] = 14 - 1;
    beeper[10][1] = 24 - 1;
    beeper[10][2] = 1;
    beeper[11][0] = 2 - 1;
    beeper[11][1] = 25 - 1;
    beeper[11][2] = 1;
    beeper[12][0] = 14 - 1;
    beeper[12][1] = 25 - 1;
    beeper[12][2] = 1;
    beeper[13][0] = 2 - 1;
    beeper[13][1] = 28 - 1;
    beeper[13][2] = 1;
    beeper[14][0] = 8 - 1;
    beeper[14][1] = 29 - 1;
    beeper[14][2] = 1;
    beeper[15][0] = 8 - 1;
    beeper[15][1] = 31 - 1;
    beeper[15][2] = 1;
    beeper[16][0] = 2 - 1;
    beeper[16][1] = 32 - 1;
    beeper[16][2] = 1;
    beeper[17][0] = 8 - 1;
    beeper[17][1] = 32 - 1;
    beeper[17][2] = 1;
    beeper[18][0] = 2 - 1;
    beeper[18][1] = 35 - 1;
    beeper[18][2] = 1;
    beeper[19][0] = 2 - 1;
    beeper[19][1] = 36 - 1;
    beeper[19][2] = 1;
    beeper[20][0] = 2 - 1;
    beeper[20][1] = 39 - 1;
    beeper[20][2] = 1;
    beeper[21][0] = 14 - 1;
    beeper[21][1] = 39 - 1;
    beeper[21][2] = 1;
    beeper[22][0] = 14 - 1;
    beeper[22][1] = 43 - 1;
    beeper[22][2] = 1;
    beeper[23][0] = 2 - 1;
    beeper[23][1] = 44 - 1;
    beeper[23][2] = 1;
    beeper[24][0] = 8 - 1;
    beeper[24][1] = 44 - 1;
    beeper[24][2] = 1;
    beeper[25][0] = 8 - 1;
    beeper[25][1] = 45 - 1;
    beeper[25][2] = 1;
    beeper[26][0] = 2 - 1;
    beeper[26][1] = 46 - 1;
    beeper[26][2] = 1;
    beeper[27][0] = 8 - 1;
    beeper[27][1] = 46 - 1;
    beeper[27][2] = 1;
}

void worldDiamondMining2()
{
    hasUlimitedBeeper = false;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 9 - 1;
    worldDimension[1] = 15 - 1;
    karelPosition[0] = 5 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 4 - 1;
    walls[0][1] = 1 - 1;
    walls[0][2] = 5 - 1;
    walls[0][3] = 1 - 1;
    walls[1][0] = 2 - 1;
    walls[1][1] = 1 - 1;
    walls[1][2] = 2 - 1;
    walls[1][3] = 2 - 1;
    walls[2][0] = 1 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 2 - 1;
    walls[2][3] = 2 - 1;
    walls[3][0] = 3 - 1;
    walls[3][1] = 1 - 1;
    walls[3][2] = 3 - 1;
    walls[3][3] = 2 - 1;
    walls[4][0] = 4 - 1;
    walls[4][1] = 1 - 1;
    walls[4][2] = 4 - 1;
    walls[4][3] = 2 - 1;
    walls[5][0] = 1 - 1;
    walls[5][1] = 3 - 1;
    walls[5][2] = 2 - 1;
    walls[5][3] = 3 - 1;
    walls[6][0] = 3 - 1;
    walls[6][1] = 2 - 1;
    walls[6][2] = 3 - 1;
    walls[6][3] = 3 - 1;
    walls[7][0] = 2 - 1;
    walls[7][1] = 3 - 1;
    walls[7][2] = 3 - 1;
    walls[7][3] = 3 - 1;
    walls[8][0] = 4 - 1;
    walls[8][1] = 2 - 1;
    walls[8][2] = 4 - 1;
    walls[8][3] = 3 - 1;
    walls[9][0] = 4 - 1;
    walls[9][1] = 3 - 1;
    walls[9][2] = 5 - 1;
    walls[9][3] = 3 - 1;
    walls[10][0] = 2 - 1;
    walls[10][1] = 3 - 1;
    walls[10][2] = 2 - 1;
    walls[10][3] = 4 - 1;
    walls[11][0] = 4 - 1;
    walls[11][1] = 4 - 1;
    walls[11][2] = 5 - 1;
    walls[11][3] = 4 - 1;
    walls[12][0] = 3 - 1;
    walls[12][1] = 4 - 1;
    walls[12][2] = 3 - 1;
    walls[12][3] = 5 - 1;
    walls[13][0] = 2 - 1;
    walls[13][1] = 5 - 1;
    walls[13][2] = 3 - 1;
    walls[13][3] = 5 - 1;
    walls[14][0] = 3 - 1;
    walls[14][1] = 5 - 1;
    walls[14][2] = 4 - 1;
    walls[14][3] = 5 - 1;
    walls[15][0] = 4 - 1;
    walls[15][1] = 5 - 1;
    walls[15][2] = 5 - 1;
    walls[15][3] = 5 - 1;
    walls[16][0] = 2 - 1;
    walls[16][1] = 6 - 1;
    walls[16][2] = 3 - 1;
    walls[16][3] = 6 - 1;
    walls[17][0] = 4 - 1;
    walls[17][1] = 5 - 1;
    walls[17][2] = 4 - 1;
    walls[17][3] = 6 - 1;
    walls[18][0] = 2 - 1;
    walls[18][1] = 7 - 1;
    walls[18][2] = 3 - 1;
    walls[18][3] = 7 - 1;
    walls[19][0] = 4 - 1;
    walls[19][1] = 6 - 1;
    walls[19][2] = 4 - 1;
    walls[19][3] = 7 - 1;
    walls[20][0] = 3 - 1;
    walls[20][1] = 7 - 1;
    walls[20][2] = 4 - 1;
    walls[20][3] = 7 - 1;
    walls[21][0] = 4 - 1;
    walls[21][1] = 7 - 1;
    walls[21][2] = 5 - 1;
    walls[21][3] = 7 - 1;
    walls[22][0] = 3 - 1;
    walls[22][1] = 7 - 1;
    walls[22][2] = 3 - 1;
    walls[22][3] = 8 - 1;
    walls[23][0] = 4 - 1;
    walls[23][1] = 8 - 1;
    walls[23][2] = 5 - 1;
    walls[23][3] = 8 - 1;
    walls[24][0] = 4 - 1;
    walls[24][1] = 9 - 1;
    walls[24][2] = 5 - 1;
    walls[24][3] = 9 - 1;
    walls[25][0] = 2 - 1;
    walls[25][1] = 9 - 1;
    walls[25][2] = 2 - 1;
    walls[25][3] = 10 - 1;
    walls[26][0] = 1 - 1;
    walls[26][1] = 10 - 1;
    walls[26][2] = 2 - 1;
    walls[26][3] = 10 - 1;
    walls[27][0] = 2 - 1;
    walls[27][1] = 10 - 1;
    walls[27][2] = 3 - 1;
    walls[27][3] = 10 - 1;
    walls[28][0] = 4 - 1;
    walls[28][1] = 10 - 1;
    walls[28][2] = 5 - 1;
    walls[28][3] = 10 - 1;
    walls[29][0] = 1 - 1;
    walls[29][1] = 11 - 1;
    walls[29][2] = 2 - 1;
    walls[29][3] = 11 - 1;
    walls[30][0] = 3 - 1;
    walls[30][1] = 10 - 1;
    walls[30][2] = 3 - 1;
    walls[30][3] = 11 - 1;
    walls[31][0] = 4 - 1;
    walls[31][1] = 10 - 1;
    walls[31][2] = 4 - 1;
    walls[31][3] = 11 - 1;
    walls[32][0] = 1 - 1;
    walls[32][1] = 12 - 1;
    walls[32][2] = 2 - 1;
    walls[32][3] = 12 - 1;
    walls[33][0] = 3 - 1;
    walls[33][1] = 11 - 1;
    walls[33][2] = 3 - 1;
    walls[33][3] = 12 - 1;
    walls[34][0] = 2 - 1;
    walls[34][1] = 12 - 1;
    walls[34][2] = 3 - 1;
    walls[34][3] = 12 - 1;
    walls[35][0] = 4 - 1;
    walls[35][1] = 11 - 1;
    walls[35][2] = 4 - 1;
    walls[35][3] = 12 - 1;
    walls[36][0] = 4 - 1;
    walls[36][1] = 12 - 1;
    walls[36][2] = 5 - 1;
    walls[36][3] = 12 - 1;
    walls[37][0] = 2 - 1;
    walls[37][1] = 12 - 1;
    walls[37][2] = 2 - 1;
    walls[37][3] = 13 - 1;
    walls[38][0] = 4 - 1;
    walls[38][1] = 13 - 1;
    walls[38][2] = 5 - 1;
    walls[38][3] = 13 - 1;
    walls[39][0] = 4 - 1;
    walls[39][1] = 14 - 1;
    walls[39][2] = 5 - 1;
    walls[39][3] = 14 - 1;
    walls[40][0] = 4 - 1;
    walls[40][1] = 15 - 1;
    walls[40][2] = 5 - 1;
    walls[40][3] = 15 - 1;
    beeper[0][0] = 2 - 1;
    beeper[0][1] = 3 - 1;
    beeper[0][2] = 1;
    beeper[1][0] = 3 - 1;
    beeper[1][1] = 5 - 1;
    beeper[1][2] = 1;
    beeper[2][0] = 3 - 1;
    beeper[2][1] = 7 - 1;
    beeper[2][2] = 1;
    beeper[3][0] = 2 - 1;
    beeper[3][1] = 11 - 1;
    beeper[3][2] = 1;
    beeper[4][0] = 2 - 1;
    beeper[4][1] = 12 - 1;
    beeper[4][2] = 1;
}

void worldFindBeeperCenter1()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 5 - 1;
    worldDimension[1] = 5 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
}

void worldFindBeeperCenter2()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 30 - 1;
    worldDimension[1] = 22 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
}

void worldFindBeeperCenter3()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 10 - 1;
    worldDimension[1] = 10 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
}

void worldHugeCave()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 30 - 1;
    worldDimension[1] = 20 - 1;
    karelPosition[0] = 15 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 14 - 1;
    walls[0][1] = 1 - 1;
    walls[0][2] = 15 - 1;
    walls[0][3] = 1 - 1;
    walls[1][0] = 15 - 1;
    walls[1][1] = 1 - 1;
    walls[1][2] = 16 - 1;
    walls[1][3] = 1 - 1;
    walls[2][0] = 14 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 15 - 1;
    walls[2][3] = 2 - 1;
    walls[3][0] = 15 - 1;
    walls[3][1] = 2 - 1;
    walls[3][2] = 16 - 1;
    walls[3][3] = 2 - 1;
    walls[4][0] = 13 - 1;
    walls[4][1] = 2 - 1;
    walls[4][2] = 13 - 1;
    walls[4][3] = 3 - 1;
    walls[5][0] = 12 - 1;
    walls[5][1] = 3 - 1;
    walls[5][2] = 13 - 1;
    walls[5][3] = 3 - 1;
    walls[6][0] = 14 - 1;
    walls[6][1] = 2 - 1;
    walls[6][2] = 14 - 1;
    walls[6][3] = 3 - 1;
    walls[7][0] = 16 - 1;
    walls[7][1] = 2 - 1;
    walls[7][2] = 16 - 1;
    walls[7][3] = 3 - 1;
    walls[8][0] = 17 - 1;
    walls[8][1] = 2 - 1;
    walls[8][2] = 17 - 1;
    walls[8][3] = 3 - 1;
    walls[9][0] = 17 - 1;
    walls[9][1] = 3 - 1;
    walls[9][2] = 18 - 1;
    walls[9][3] = 3 - 1;
    walls[10][0] = 12 - 1;
    walls[10][1] = 4 - 1;
    walls[10][2] = 13 - 1;
    walls[10][3] = 4 - 1;
    walls[11][0] = 17 - 1;
    walls[11][1] = 4 - 1;
    walls[11][2] = 18 - 1;
    walls[11][3] = 4 - 1;
    walls[12][0] = 10 - 1;
    walls[12][1] = 4 - 1;
    walls[12][2] = 10 - 1;
    walls[12][3] = 5 - 1;
    walls[13][0] = 9 - 1;
    walls[13][1] = 5 - 1;
    walls[13][2] = 10 - 1;
    walls[13][3] = 5 - 1;
    walls[14][0] = 11 - 1;
    walls[14][1] = 4 - 1;
    walls[14][2] = 11 - 1;
    walls[14][3] = 5 - 1;
    walls[15][0] = 12 - 1;
    walls[15][1] = 4 - 1;
    walls[15][2] = 12 - 1;
    walls[15][3] = 5 - 1;
    walls[16][0] = 18 - 1;
    walls[16][1] = 4 - 1;
    walls[16][2] = 18 - 1;
    walls[16][3] = 5 - 1;
    walls[17][0] = 18 - 1;
    walls[17][1] = 5 - 1;
    walls[17][2] = 19 - 1;
    walls[17][3] = 5 - 1;
    walls[18][0] = 9 - 1;
    walls[18][1] = 6 - 1;
    walls[18][2] = 10 - 1;
    walls[18][3] = 6 - 1;
    walls[19][0] = 19 - 1;
    walls[19][1] = 5 - 1;
    walls[19][2] = 19 - 1;
    walls[19][3] = 6 - 1;
    walls[20][0] = 20 - 1;
    walls[20][1] = 5 - 1;
    walls[20][2] = 20 - 1;
    walls[20][3] = 6 - 1;
    walls[21][0] = 21 - 1;
    walls[21][1] = 5 - 1;
    walls[21][2] = 21 - 1;
    walls[21][3] = 6 - 1;
    walls[22][0] = 22 - 1;
    walls[22][1] = 5 - 1;
    walls[22][2] = 22 - 1;
    walls[22][3] = 6 - 1;
    walls[23][0] = 23 - 1;
    walls[23][1] = 5 - 1;
    walls[23][2] = 23 - 1;
    walls[23][3] = 6 - 1;
    walls[24][0] = 24 - 1;
    walls[24][1] = 5 - 1;
    walls[24][2] = 24 - 1;
    walls[24][3] = 6 - 1;
    walls[25][0] = 24 - 1;
    walls[25][1] = 6 - 1;
    walls[25][2] = 25 - 1;
    walls[25][3] = 6 - 1;
    walls[26][0] = 9 - 1;
    walls[26][1] = 7 - 1;
    walls[26][2] = 10 - 1;
    walls[26][3] = 7 - 1;
    walls[27][0] = 24 - 1;
    walls[27][1] = 7 - 1;
    walls[27][2] = 25 - 1;
    walls[27][3] = 7 - 1;
    walls[28][0] = 8 - 1;
    walls[28][1] = 7 - 1;
    walls[28][2] = 8 - 1;
    walls[28][3] = 8 - 1;
    walls[29][0] = 7 - 1;
    walls[29][1] = 8 - 1;
    walls[29][2] = 8 - 1;
    walls[29][3] = 8 - 1;
    walls[30][0] = 9 - 1;
    walls[30][1] = 7 - 1;
    walls[30][2] = 9 - 1;
    walls[30][3] = 8 - 1;
    walls[31][0] = 24 - 1;
    walls[31][1] = 8 - 1;
    walls[31][2] = 25 - 1;
    walls[31][3] = 8 - 1;
    walls[32][0] = 7 - 1;
    walls[32][1] = 9 - 1;
    walls[32][2] = 8 - 1;
    walls[32][3] = 9 - 1;
    walls[33][0] = 25 - 1;
    walls[33][1] = 8 - 1;
    walls[33][2] = 25 - 1;
    walls[33][3] = 9 - 1;
    walls[34][0] = 25 - 1;
    walls[34][1] = 9 - 1;
    walls[34][2] = 26 - 1;
    walls[34][3] = 9 - 1;
    walls[35][0] = 7 - 1;
    walls[35][1] = 9 - 1;
    walls[35][2] = 7 - 1;
    walls[35][3] = 10 - 1;
    walls[36][0] = 6 - 1;
    walls[36][1] = 10 - 1;
    walls[36][2] = 7 - 1;
    walls[36][3] = 10 - 1;
    walls[37][0] = 25 - 1;
    walls[37][1] = 10 - 1;
    walls[37][2] = 26 - 1;
    walls[37][3] = 10 - 1;
    walls[38][0] = 6 - 1;
    walls[38][1] = 10 - 1;
    walls[38][2] = 6 - 1;
    walls[38][3] = 11 - 1;
    walls[39][0] = 5 - 1;
    walls[39][1] = 11 - 1;
    walls[39][2] = 6 - 1;
    walls[39][3] = 11 - 1;
    walls[40][0] = 26 - 1;
    walls[40][1] = 10 - 1;
    walls[40][2] = 26 - 1;
    walls[40][3] = 11 - 1;
    walls[41][0] = 26 - 1;
    walls[41][1] = 11 - 1;
    walls[41][2] = 27 - 1;
    walls[41][3] = 11 - 1;
    walls[42][0] = 5 - 1;
    walls[42][1] = 11 - 1;
    walls[42][2] = 5 - 1;
    walls[42][3] = 12 - 1;
    walls[43][0] = 4 - 1;
    walls[43][1] = 12 - 1;
    walls[43][2] = 5 - 1;
    walls[43][3] = 12 - 1;
    walls[44][0] = 26 - 1;
    walls[44][1] = 12 - 1;
    walls[44][2] = 27 - 1;
    walls[44][3] = 12 - 1;
    walls[45][0] = 4 - 1;
    walls[45][1] = 13 - 1;
    walls[45][2] = 5 - 1;
    walls[45][3] = 13 - 1;
    walls[46][0] = 26 - 1;
    walls[46][1] = 13 - 1;
    walls[46][2] = 27 - 1;
    walls[46][3] = 13 - 1;
    walls[47][0] = 4 - 1;
    walls[47][1] = 13 - 1;
    walls[47][2] = 4 - 1;
    walls[47][3] = 14 - 1;
    walls[48][0] = 3 - 1;
    walls[48][1] = 14 - 1;
    walls[48][2] = 4 - 1;
    walls[48][3] = 14 - 1;
    walls[49][0] = 27 - 1;
    walls[49][1] = 13 - 1;
    walls[49][2] = 27 - 1;
    walls[49][3] = 14 - 1;
    walls[50][0] = 27 - 1;
    walls[50][1] = 14 - 1;
    walls[50][2] = 28 - 1;
    walls[50][3] = 14 - 1;
    walls[51][0] = 3 - 1;
    walls[51][1] = 14 - 1;
    walls[51][2] = 3 - 1;
    walls[51][3] = 15 - 1;
    walls[52][0] = 2 - 1;
    walls[52][1] = 15 - 1;
    walls[52][2] = 3 - 1;
    walls[52][3] = 15 - 1;
    walls[53][0] = 28 - 1;
    walls[53][1] = 14 - 1;
    walls[53][2] = 28 - 1;
    walls[53][3] = 15 - 1;
    walls[54][0] = 28 - 1;
    walls[54][1] = 15 - 1;
    walls[54][2] = 29 - 1;
    walls[54][3] = 15 - 1;
    walls[55][0] = 2 - 1;
    walls[55][1] = 16 - 1;
    walls[55][2] = 3 - 1;
    walls[55][3] = 16 - 1;
    walls[56][0] = 28 - 1;
    walls[56][1] = 16 - 1;
    walls[56][2] = 29 - 1;
    walls[56][3] = 16 - 1;
    walls[57][0] = 2 - 1;
    walls[57][1] = 17 - 1;
    walls[57][2] = 3 - 1;
    walls[57][3] = 17 - 1;
    walls[58][0] = 28 - 1;
    walls[58][1] = 17 - 1;
    walls[58][2] = 29 - 1;
    walls[58][3] = 17 - 1;
    walls[59][0] = 1 - 1;
    walls[59][1] = 17 - 1;
    walls[59][2] = 1 - 1;
    walls[59][3] = 18 - 1;
    walls[60][0] = 2 - 1;
    walls[60][1] = 17 - 1;
    walls[60][2] = 2 - 1;
    walls[60][3] = 18 - 1;
    walls[61][0] = 29 - 1;
    walls[61][1] = 17 - 1;
    walls[61][2] = 29 - 1;
    walls[61][3] = 18 - 1;
    walls[62][0] = 29 - 1;
    walls[62][1] = 18 - 1;
    walls[62][2] = 30 - 1;
    walls[62][3] = 18 - 1;
    walls[63][0] = 29 - 1;
    walls[63][1] = 19 - 1;
    walls[63][2] = 30 - 1;
    walls[63][3] = 19 - 1;
    walls[64][0] = 30 - 1;
    walls[64][1] = 19 - 1;
    walls[64][2] = 30 - 1;
    walls[64][3] = 20 - 1;
    beeper[0][0] = 13 - 1;
    beeper[0][1] = 4 - 1;
    beeper[0][2] = 1;
    beeper[1][0] = 14 - 1;
    beeper[1][1] = 4 - 1;
    beeper[1][2] = 1;
    beeper[2][0] = 15 - 1;
    beeper[2][1] = 4 - 1;
    beeper[2][2] = 1;
    beeper[3][0] = 15 - 1;
    beeper[3][1] = 5 - 1;
    beeper[3][2] = 1;
    beeper[4][0] = 16 - 1;
    beeper[4][1] = 5 - 1;
    beeper[4][2] = 1;
    beeper[5][0] = 17 - 1;
    beeper[5][1] = 5 - 1;
    beeper[5][2] = 1;
    beeper[6][0] = 18 - 1;
    beeper[6][1] = 5 - 1;
    beeper[6][2] = 1;
    beeper[7][0] = 10 - 1;
    beeper[7][1] = 6 - 1;
    beeper[7][2] = 1;
    beeper[8][0] = 11 - 1;
    beeper[8][1] = 6 - 1;
    beeper[8][2] = 1;
    beeper[9][0] = 12 - 1;
    beeper[9][1] = 6 - 1;
    beeper[9][2] = 1;
    beeper[10][0] = 13 - 1;
    beeper[10][1] = 6 - 1;
    beeper[10][2] = 1;
    beeper[11][0] = 14 - 1;
    beeper[11][1] = 6 - 1;
    beeper[11][2] = 1;
    beeper[12][0] = 17 - 1;
    beeper[12][1] = 7 - 1;
    beeper[12][2] = 1;
    beeper[13][0] = 18 - 1;
    beeper[13][1] = 7 - 1;
    beeper[13][2] = 1;
    beeper[14][0] = 19 - 1;
    beeper[14][1] = 7 - 1;
    beeper[14][2] = 1;
    beeper[15][0] = 20 - 1;
    beeper[15][1] = 7 - 1;
    beeper[15][2] = 1;
    beeper[16][0] = 21 - 1;
    beeper[16][1] = 7 - 1;
    beeper[16][2] = 1;
    beeper[17][0] = 22 - 1;
    beeper[17][1] = 7 - 1;
    beeper[17][2] = 1;
    beeper[18][0] = 23 - 1;
    beeper[18][1] = 7 - 1;
    beeper[18][2] = 1;
    beeper[19][0] = 24 - 1;
    beeper[19][1] = 7 - 1;
    beeper[19][2] = 1;
    beeper[20][0] = 8 - 1;
    beeper[20][1] = 8 - 1;
    beeper[20][2] = 1;
    beeper[21][0] = 9 - 1;
    beeper[21][1] = 8 - 1;
    beeper[21][2] = 1;
    beeper[22][0] = 10 - 1;
    beeper[22][1] = 8 - 1;
    beeper[22][2] = 1;
    beeper[23][0] = 11 - 1;
    beeper[23][1] = 8 - 1;
    beeper[23][2] = 1;
    beeper[24][0] = 12 - 1;
    beeper[24][1] = 8 - 1;
    beeper[24][2] = 1;
    beeper[25][0] = 13 - 1;
    beeper[25][1] = 8 - 1;
    beeper[25][2] = 1;
    beeper[26][0] = 14 - 1;
    beeper[26][1] = 8 - 1;
    beeper[26][2] = 1;
    beeper[27][0] = 15 - 1;
    beeper[27][1] = 8 - 1;
    beeper[27][2] = 1;
    beeper[28][0] = 21 - 1;
    beeper[28][1] = 8 - 1;
    beeper[28][2] = 1;
    beeper[29][0] = 22 - 1;
    beeper[29][1] = 8 - 1;
    beeper[29][2] = 1;
    beeper[30][0] = 23 - 1;
    beeper[30][1] = 8 - 1;
    beeper[30][2] = 1;
    beeper[31][0] = 24 - 1;
    beeper[31][1] = 8 - 1;
    beeper[31][2] = 1;
    beeper[32][0] = 7 - 1;
    beeper[32][1] = 10 - 1;
    beeper[32][2] = 1;
    beeper[33][0] = 8 - 1;
    beeper[33][1] = 10 - 1;
    beeper[33][2] = 1;
    beeper[34][0] = 9 - 1;
    beeper[34][1] = 10 - 1;
    beeper[34][2] = 1;
    beeper[35][0] = 10 - 1;
    beeper[35][1] = 10 - 1;
    beeper[35][2] = 1;
    beeper[36][0] = 11 - 1;
    beeper[36][1] = 10 - 1;
    beeper[36][2] = 1;
    beeper[37][0] = 12 - 1;
    beeper[37][1] = 10 - 1;
    beeper[37][2] = 1;
    beeper[38][0] = 13 - 1;
    beeper[38][1] = 10 - 1;
    beeper[38][2] = 1;
    beeper[39][0] = 14 - 1;
    beeper[39][1] = 10 - 1;
    beeper[39][2] = 1;
    beeper[40][0] = 15 - 1;
    beeper[40][1] = 10 - 1;
    beeper[40][2] = 1;
    beeper[41][0] = 16 - 1;
    beeper[41][1] = 10 - 1;
    beeper[41][2] = 1;
    beeper[42][0] = 22 - 1;
    beeper[42][1] = 10 - 1;
    beeper[42][2] = 1;
    beeper[43][0] = 23 - 1;
    beeper[43][1] = 10 - 1;
    beeper[43][2] = 1;
    beeper[44][0] = 24 - 1;
    beeper[44][1] = 10 - 1;
    beeper[44][2] = 1;
    beeper[45][0] = 25 - 1;
    beeper[45][1] = 10 - 1;
    beeper[45][2] = 1;
    beeper[46][0] = 22 - 1;
    beeper[46][1] = 11 - 1;
    beeper[46][2] = 1;
    beeper[47][0] = 23 - 1;
    beeper[47][1] = 11 - 1;
    beeper[47][2] = 1;
    beeper[48][0] = 24 - 1;
    beeper[48][1] = 11 - 1;
    beeper[48][2] = 1;
    beeper[49][0] = 25 - 1;
    beeper[49][1] = 11 - 1;
    beeper[49][2] = 1;
    beeper[50][0] = 26 - 1;
    beeper[50][1] = 11 - 1;
    beeper[50][2] = 1;
    beeper[51][0] = 5 - 1;
    beeper[51][1] = 12 - 1;
    beeper[51][2] = 1;
    beeper[52][0] = 6 - 1;
    beeper[52][1] = 12 - 1;
    beeper[52][2] = 1;
    beeper[53][0] = 7 - 1;
    beeper[53][1] = 12 - 1;
    beeper[53][2] = 1;
    beeper[54][0] = 8 - 1;
    beeper[54][1] = 12 - 1;
    beeper[54][2] = 1;
    beeper[55][0] = 9 - 1;
    beeper[55][1] = 12 - 1;
    beeper[55][2] = 1;
    beeper[56][0] = 10 - 1;
    beeper[56][1] = 12 - 1;
    beeper[56][2] = 1;
    beeper[57][0] = 11 - 1;
    beeper[57][1] = 12 - 1;
    beeper[57][2] = 1;
    beeper[58][0] = 12 - 1;
    beeper[58][1] = 12 - 1;
    beeper[58][2] = 1;
    beeper[59][0] = 4 - 1;
    beeper[59][1] = 14 - 1;
    beeper[59][2] = 1;
    beeper[60][0] = 5 - 1;
    beeper[60][1] = 14 - 1;
    beeper[60][2] = 1;
    beeper[61][0] = 6 - 1;
    beeper[61][1] = 14 - 1;
    beeper[61][2] = 1;
    beeper[62][0] = 7 - 1;
    beeper[62][1] = 14 - 1;
    beeper[62][2] = 1;
    beeper[63][0] = 8 - 1;
    beeper[63][1] = 14 - 1;
    beeper[63][2] = 1;
    beeper[64][0] = 9 - 1;
    beeper[64][1] = 14 - 1;
    beeper[64][2] = 1;
    beeper[65][0] = 10 - 1;
    beeper[65][1] = 14 - 1;
    beeper[65][2] = 1;
    beeper[66][0] = 11 - 1;
    beeper[66][1] = 14 - 1;
    beeper[66][2] = 1;
    beeper[67][0] = 12 - 1;
    beeper[67][1] = 14 - 1;
    beeper[67][2] = 1;
    beeper[68][0] = 13 - 1;
    beeper[68][1] = 14 - 1;
    beeper[68][2] = 1;
    beeper[69][0] = 14 - 1;
    beeper[69][1] = 14 - 1;
    beeper[69][2] = 1;
    beeper[70][0] = 15 - 1;
    beeper[70][1] = 14 - 1;
    beeper[70][2] = 1;
    beeper[71][0] = 20 - 1;
    beeper[71][1] = 14 - 1;
    beeper[71][2] = 1;
    beeper[72][0] = 21 - 1;
    beeper[72][1] = 14 - 1;
    beeper[72][2] = 1;
    beeper[73][0] = 22 - 1;
    beeper[73][1] = 14 - 1;
    beeper[73][2] = 1;
    beeper[74][0] = 23 - 1;
    beeper[74][1] = 14 - 1;
    beeper[74][2] = 1;
    beeper[75][0] = 24 - 1;
    beeper[75][1] = 14 - 1;
    beeper[75][2] = 1;
    beeper[76][0] = 25 - 1;
    beeper[76][1] = 14 - 1;
    beeper[76][2] = 1;
    beeper[77][0] = 26 - 1;
    beeper[77][1] = 14 - 1;
    beeper[77][2] = 1;
    beeper[78][0] = 27 - 1;
    beeper[78][1] = 14 - 1;
    beeper[78][2] = 1;
    beeper[79][0] = 3 - 1;
    beeper[79][1] = 15 - 1;
    beeper[79][2] = 1;
    beeper[80][0] = 4 - 1;
    beeper[80][1] = 15 - 1;
    beeper[80][2] = 1;
    beeper[81][0] = 5 - 1;
    beeper[81][1] = 15 - 1;
    beeper[81][2] = 1;
    beeper[82][0] = 6 - 1;
    beeper[82][1] = 15 - 1;
    beeper[82][2] = 1;
    beeper[83][0] = 7 - 1;
    beeper[83][1] = 15 - 1;
    beeper[83][2] = 1;
    beeper[84][0] = 8 - 1;
    beeper[84][1] = 15 - 1;
    beeper[84][2] = 1;
    beeper[85][0] = 9 - 1;
    beeper[85][1] = 15 - 1;
    beeper[85][2] = 1;
    beeper[86][0] = 10 - 1;
    beeper[86][1] = 15 - 1;
    beeper[86][2] = 1;
    beeper[87][0] = 11 - 1;
    beeper[87][1] = 15 - 1;
    beeper[87][2] = 1;
    beeper[88][0] = 12 - 1;
    beeper[88][1] = 15 - 1;
    beeper[88][2] = 1;
    beeper[89][0] = 13 - 1;
    beeper[89][1] = 15 - 1;
    beeper[89][2] = 1;
    beeper[90][0] = 23 - 1;
    beeper[90][1] = 15 - 1;
    beeper[90][2] = 1;
    beeper[91][0] = 24 - 1;
    beeper[91][1] = 15 - 1;
    beeper[91][2] = 1;
    beeper[92][0] = 25 - 1;
    beeper[92][1] = 15 - 1;
    beeper[92][2] = 1;
    beeper[93][0] = 26 - 1;
    beeper[93][1] = 15 - 1;
    beeper[93][2] = 1;
    beeper[94][0] = 27 - 1;
    beeper[94][1] = 15 - 1;
    beeper[94][2] = 1;
    beeper[95][0] = 28 - 1;
    beeper[95][1] = 15 - 1;
    beeper[95][2] = 1;
    beeper[96][0] = 3 - 1;
    beeper[96][1] = 17 - 1;
    beeper[96][2] = 1;
    beeper[97][0] = 4 - 1;
    beeper[97][1] = 17 - 1;
    beeper[97][2] = 1;
    beeper[98][0] = 5 - 1;
    beeper[98][1] = 17 - 1;
    beeper[98][2] = 1;
    beeper[99][0] = 6 - 1;
    beeper[99][1] = 17 - 1;
    beeper[99][2] = 1;
    beeper[100][0] = 7 - 1;
    beeper[100][1] = 17 - 1;
    beeper[100][2] = 1;
    beeper[101][0] = 8 - 1;
    beeper[101][1] = 17 - 1;
    beeper[101][2] = 1;
    beeper[102][0] = 9 - 1;
    beeper[102][1] = 17 - 1;
    beeper[102][2] = 1;
    beeper[103][0] = 10 - 1;
    beeper[103][1] = 17 - 1;
    beeper[103][2] = 1;
    beeper[104][0] = 11 - 1;
    beeper[104][1] = 17 - 1;
    beeper[104][2] = 1;
    beeper[105][0] = 12 - 1;
    beeper[105][1] = 17 - 1;
    beeper[105][2] = 1;
    beeper[106][0] = 13 - 1;
    beeper[106][1] = 17 - 1;
    beeper[106][2] = 1;
    beeper[107][0] = 14 - 1;
    beeper[107][1] = 17 - 1;
    beeper[107][2] = 1;
    beeper[108][0] = 15 - 1;
    beeper[108][1] = 17 - 1;
    beeper[108][2] = 1;
    beeper[109][0] = 16 - 1;
    beeper[109][1] = 17 - 1;
    beeper[109][2] = 1;
    beeper[110][0] = 22 - 1;
    beeper[110][1] = 17 - 1;
    beeper[110][2] = 1;
    beeper[111][0] = 23 - 1;
    beeper[111][1] = 17 - 1;
    beeper[111][2] = 1;
    beeper[112][0] = 24 - 1;
    beeper[112][1] = 17 - 1;
    beeper[112][2] = 1;
    beeper[113][0] = 25 - 1;
    beeper[113][1] = 17 - 1;
    beeper[113][2] = 1;
    beeper[114][0] = 26 - 1;
    beeper[114][1] = 17 - 1;
    beeper[114][2] = 1;
    beeper[115][0] = 27 - 1;
    beeper[115][1] = 17 - 1;
    beeper[115][2] = 1;
    beeper[116][0] = 28 - 1;
    beeper[116][1] = 17 - 1;
    beeper[116][2] = 1;
    beeper[117][0] = 1 - 1;
    beeper[117][1] = 18 - 1;
    beeper[117][2] = 1;
    beeper[118][0] = 2 - 1;
    beeper[118][1] = 18 - 1;
    beeper[118][2] = 1;
    beeper[119][0] = 3 - 1;
    beeper[119][1] = 18 - 1;
    beeper[119][2] = 1;
    beeper[120][0] = 4 - 1;
    beeper[120][1] = 18 - 1;
    beeper[120][2] = 1;
    beeper[121][0] = 5 - 1;
    beeper[121][1] = 18 - 1;
    beeper[121][2] = 1;
    beeper[122][0] = 6 - 1;
    beeper[122][1] = 18 - 1;
    beeper[122][2] = 1;
    beeper[123][0] = 7 - 1;
    beeper[123][1] = 18 - 1;
    beeper[123][2] = 1;
    beeper[124][0] = 8 - 1;
    beeper[124][1] = 18 - 1;
    beeper[124][2] = 1;
    beeper[125][0] = 9 - 1;
    beeper[125][1] = 18 - 1;
    beeper[125][2] = 1;
    beeper[126][0] = 10 - 1;
    beeper[126][1] = 18 - 1;
    beeper[126][2] = 1;
    beeper[127][0] = 11 - 1;
    beeper[127][1] = 18 - 1;
    beeper[127][2] = 1;
    beeper[128][0] = 1 - 1;
    beeper[128][1] = 19 - 1;
    beeper[128][2] = 1;
    beeper[129][0] = 2 - 1;
    beeper[129][1] = 19 - 1;
    beeper[129][2] = 1;
    beeper[130][0] = 3 - 1;
    beeper[130][1] = 19 - 1;
    beeper[130][2] = 1;
    beeper[131][0] = 4 - 1;
    beeper[131][1] = 19 - 1;
    beeper[131][2] = 1;
    beeper[132][0] = 5 - 1;
    beeper[132][1] = 19 - 1;
    beeper[132][2] = 1;
    beeper[133][0] = 6 - 1;
    beeper[133][1] = 19 - 1;
    beeper[133][2] = 1;
    beeper[134][0] = 1 - 1;
    beeper[134][1] = 20 - 1;
    beeper[134][2] = 1;
    beeper[135][0] = 2 - 1;
    beeper[135][1] = 20 - 1;
    beeper[135][2] = 1;
    beeper[136][0] = 3 - 1;
    beeper[136][1] = 20 - 1;
    beeper[136][2] = 1;
    beeper[137][0] = 4 - 1;
    beeper[137][1] = 20 - 1;
    beeper[137][2] = 1;
    beeper[138][0] = 5 - 1;
    beeper[138][1] = 20 - 1;
    beeper[138][2] = 1;
    beeper[139][0] = 6 - 1;
    beeper[139][1] = 20 - 1;
    beeper[139][2] = 1;
    beeper[140][0] = 7 - 1;
    beeper[140][1] = 20 - 1;
    beeper[140][2] = 1;
    beeper[141][0] = 8 - 1;
    beeper[141][1] = 20 - 1;
    beeper[141][2] = 1;
    beeper[142][0] = 9 - 1;
    beeper[142][1] = 20 - 1;
    beeper[142][2] = 1;
    beeper[143][0] = 10 - 1;
    beeper[143][1] = 20 - 1;
    beeper[143][2] = 1;
    beeper[144][0] = 11 - 1;
    beeper[144][1] = 20 - 1;
    beeper[144][2] = 1;
    beeper[145][0] = 15 - 1;
    beeper[145][1] = 20 - 1;
    beeper[145][2] = 1;
    beeper[146][0] = 16 - 1;
    beeper[146][1] = 20 - 1;
    beeper[146][2] = 1;
    beeper[147][0] = 17 - 1;
    beeper[147][1] = 20 - 1;
    beeper[147][2] = 1;
    beeper[148][0] = 18 - 1;
    beeper[148][1] = 20 - 1;
    beeper[148][2] = 1;
    beeper[149][0] = 19 - 1;
    beeper[149][1] = 20 - 1;
    beeper[149][2] = 1;
    beeper[150][0] = 20 - 1;
    beeper[150][1] = 20 - 1;
    beeper[150][2] = 1;
    beeper[151][0] = 21 - 1;
    beeper[151][1] = 20 - 1;
    beeper[151][2] = 1;
    beeper[152][0] = 22 - 1;
    beeper[152][1] = 20 - 1;
    beeper[152][2] = 1;
    beeper[153][0] = 23 - 1;
    beeper[153][1] = 20 - 1;
    beeper[153][2] = 1;
    beeper[154][0] = 24 - 1;
    beeper[154][1] = 20 - 1;
    beeper[154][2] = 1;
    beeper[155][0] = 25 - 1;
    beeper[155][1] = 20 - 1;
    beeper[155][2] = 1;
    beeper[156][0] = 26 - 1;
    beeper[156][1] = 20 - 1;
    beeper[156][2] = 1;
    beeper[157][0] = 27 - 1;
    beeper[157][1] = 20 - 1;
    beeper[157][2] = 1;
    beeper[158][0] = 28 - 1;
    beeper[158][1] = 20 - 1;
    beeper[158][2] = 1;
    beeper[159][0] = 29 - 1;
    beeper[159][1] = 20 - 1;
    beeper[159][2] = 1;
    beeper[160][0] = 30 - 1;
    beeper[160][1] = 20 - 1;
    beeper[160][2] = 1;
}

void worldLargeHall()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 22 - 1;
    worldDimension[1] = 22 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
}

void worldMediumCave()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 14 - 1;
    worldDimension[1] = 10 - 1;
    karelPosition[0] = 7 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 6 - 1;
    walls[0][1] = 1 - 1;
    walls[0][2] = 7 - 1;
    walls[0][3] = 1 - 1;
    walls[1][0] = 7 - 1;
    walls[1][1] = 1 - 1;
    walls[1][2] = 8 - 1;
    walls[1][3] = 1 - 1;
    walls[2][0] = 6 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 7 - 1;
    walls[2][3] = 2 - 1;
    walls[3][0] = 7 - 1;
    walls[3][1] = 2 - 1;
    walls[3][2] = 8 - 1;
    walls[3][3] = 2 - 1;
    walls[4][0] = 5 - 1;
    walls[4][1] = 2 - 1;
    walls[4][2] = 5 - 1;
    walls[4][3] = 3 - 1;
    walls[5][0] = 4 - 1;
    walls[5][1] = 3 - 1;
    walls[5][2] = 5 - 1;
    walls[5][3] = 3 - 1;
    walls[6][0] = 6 - 1;
    walls[6][1] = 2 - 1;
    walls[6][2] = 6 - 1;
    walls[6][3] = 3 - 1;
    walls[7][0] = 8 - 1;
    walls[7][1] = 2 - 1;
    walls[7][2] = 8 - 1;
    walls[7][3] = 3 - 1;
    walls[8][0] = 8 - 1;
    walls[8][1] = 3 - 1;
    walls[8][2] = 9 - 1;
    walls[8][3] = 3 - 1;
    walls[9][0] = 4 - 1;
    walls[9][1] = 4 - 1;
    walls[9][2] = 5 - 1;
    walls[9][3] = 4 - 1;
    walls[10][0] = 9 - 1;
    walls[10][1] = 3 - 1;
    walls[10][2] = 9 - 1;
    walls[10][3] = 4 - 1;
    walls[11][0] = 10 - 1;
    walls[11][1] = 3 - 1;
    walls[11][2] = 10 - 1;
    walls[11][3] = 4 - 1;
    walls[12][0] = 10 - 1;
    walls[12][1] = 4 - 1;
    walls[12][2] = 11 - 1;
    walls[12][3] = 4 - 1;
    walls[13][0] = 4 - 1;
    walls[13][1] = 4 - 1;
    walls[13][2] = 4 - 1;
    walls[13][3] = 5 - 1;
    walls[14][0] = 3 - 1;
    walls[14][1] = 5 - 1;
    walls[14][2] = 4 - 1;
    walls[14][3] = 5 - 1;
    walls[15][0] = 11 - 1;
    walls[15][1] = 4 - 1;
    walls[15][2] = 11 - 1;
    walls[15][3] = 5 - 1;
    walls[16][0] = 12 - 1;
    walls[16][1] = 4 - 1;
    walls[16][2] = 12 - 1;
    walls[16][3] = 5 - 1;
    walls[17][0] = 12 - 1;
    walls[17][1] = 5 - 1;
    walls[17][2] = 13 - 1;
    walls[17][3] = 5 - 1;
    walls[18][0] = 3 - 1;
    walls[18][1] = 6 - 1;
    walls[18][2] = 4 - 1;
    walls[18][3] = 6 - 1;
    walls[19][0] = 12 - 1;
    walls[19][1] = 6 - 1;
    walls[19][2] = 13 - 1;
    walls[19][3] = 6 - 1;
    walls[20][0] = 3 - 1;
    walls[20][1] = 7 - 1;
    walls[20][2] = 4 - 1;
    walls[20][3] = 7 - 1;
    walls[21][0] = 13 - 1;
    walls[21][1] = 6 - 1;
    walls[21][2] = 13 - 1;
    walls[21][3] = 7 - 1;
    walls[22][0] = 13 - 1;
    walls[22][1] = 7 - 1;
    walls[22][2] = 14 - 1;
    walls[22][3] = 7 - 1;
    walls[23][0] = 2 - 1;
    walls[23][1] = 7 - 1;
    walls[23][2] = 2 - 1;
    walls[23][3] = 8 - 1;
    walls[24][0] = 1 - 1;
    walls[24][1] = 8 - 1;
    walls[24][2] = 2 - 1;
    walls[24][3] = 8 - 1;
    walls[25][0] = 3 - 1;
    walls[25][1] = 7 - 1;
    walls[25][2] = 3 - 1;
    walls[25][3] = 8 - 1;
    walls[26][0] = 13 - 1;
    walls[26][1] = 8 - 1;
    walls[26][2] = 14 - 1;
    walls[26][3] = 8 - 1;
    walls[27][0] = 1 - 1;
    walls[27][1] = 8 - 1;
    walls[27][2] = 1 - 1;
    walls[27][3] = 9 - 1;
    walls[28][0] = 13 - 1;
    walls[28][1] = 9 - 1;
    walls[28][2] = 14 - 1;
    walls[28][3] = 9 - 1;
    walls[29][0] = 14 - 1;
    walls[29][1] = 9 - 1;
    walls[29][2] = 14 - 1;
    walls[29][3] = 10 - 1;
    beeper[0][0] = 5 - 1;
    beeper[0][1] = 3 - 1;
    beeper[0][2] = 1;
    beeper[1][0] = 6 - 1;
    beeper[1][1] = 3 - 1;
    beeper[1][2] = 1;
    beeper[2][0] = 8 - 1;
    beeper[2][1] = 4 - 1;
    beeper[2][2] = 1;
    beeper[3][0] = 9 - 1;
    beeper[3][1] = 4 - 1;
    beeper[3][2] = 1;
    beeper[4][0] = 10 - 1;
    beeper[4][1] = 4 - 1;
    beeper[4][2] = 1;
    beeper[5][0] = 4 - 1;
    beeper[5][1] = 6 - 1;
    beeper[5][2] = 1;
    beeper[6][0] = 5 - 1;
    beeper[6][1] = 6 - 1;
    beeper[6][2] = 1;
    beeper[7][0] = 6 - 1;
    beeper[7][1] = 6 - 1;
    beeper[7][2] = 1;
    beeper[8][0] = 7 - 1;
    beeper[8][1] = 6 - 1;
    beeper[8][2] = 1;
    beeper[9][0] = 9 - 1;
    beeper[9][1] = 6 - 1;
    beeper[9][2] = 1;
    beeper[10][0] = 10 - 1;
    beeper[10][1] = 6 - 1;
    beeper[10][2] = 1;
    beeper[11][0] = 11 - 1;
    beeper[11][1] = 6 - 1;
    beeper[11][2] = 1;
    beeper[12][0] = 12 - 1;
    beeper[12][1] = 6 - 1;
    beeper[12][2] = 1;
    beeper[13][0] = 8 - 1;
    beeper[13][1] = 7 - 1;
    beeper[13][2] = 1;
    beeper[14][0] = 9 - 1;
    beeper[14][1] = 7 - 1;
    beeper[14][2] = 1;
    beeper[15][0] = 10 - 1;
    beeper[15][1] = 7 - 1;
    beeper[15][2] = 1;
    beeper[16][0] = 11 - 1;
    beeper[16][1] = 7 - 1;
    beeper[16][2] = 1;
    beeper[17][0] = 12 - 1;
    beeper[17][1] = 7 - 1;
    beeper[17][2] = 1;
    beeper[18][0] = 13 - 1;
    beeper[18][1] = 7 - 1;
    beeper[18][2] = 1;
    beeper[19][0] = 2 - 1;
    beeper[19][1] = 8 - 1;
    beeper[19][2] = 1;
    beeper[20][0] = 3 - 1;
    beeper[20][1] = 8 - 1;
    beeper[20][2] = 1;
    beeper[21][0] = 4 - 1;
    beeper[21][1] = 8 - 1;
    beeper[21][2] = 1;
    beeper[22][0] = 1 - 1;
    beeper[22][1] = 9 - 1;
    beeper[22][2] = 1;
    beeper[23][0] = 2 - 1;
    beeper[23][1] = 9 - 1;
    beeper[23][2] = 1;
    beeper[24][0] = 3 - 1;
    beeper[24][1] = 9 - 1;
    beeper[24][2] = 1;
    beeper[25][0] = 4 - 1;
    beeper[25][1] = 9 - 1;
    beeper[25][2] = 1;
    beeper[26][0] = 5 - 1;
    beeper[26][1] = 9 - 1;
    beeper[26][2] = 1;
    beeper[27][0] = 11 - 1;
    beeper[27][1] = 9 - 1;
    beeper[27][2] = 1;
    beeper[28][0] = 12 - 1;
    beeper[28][1] = 9 - 1;
    beeper[28][2] = 1;
    beeper[29][0] = 13 - 1;
    beeper[29][1] = 9 - 1;
    beeper[29][2] = 1;
    beeper[30][0] = 8 - 1;
    beeper[30][1] = 10 - 1;
    beeper[30][2] = 1;
    beeper[31][0] = 9 - 1;
    beeper[31][1] = 10 - 1;
    beeper[31][2] = 1;
    beeper[32][0] = 10 - 1;
    beeper[32][1] = 10 - 1;
    beeper[32][2] = 1;
    beeper[33][0] = 11 - 1;
    beeper[33][1] = 10 - 1;
    beeper[33][2] = 1;
    beeper[34][0] = 12 - 1;
    beeper[34][1] = 10 - 1;
    beeper[34][2] = 1;
    beeper[35][0] = 13 - 1;
    beeper[35][1] = 10 - 1;
    beeper[35][2] = 1;
    beeper[36][0] = 14 - 1;
    beeper[36][1] = 10 - 1;
    beeper[36][2] = 1;
}

void worldMediumHall()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 10 - 1;
    worldDimension[1] = 10 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
}

void worldplaceFlag()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 10 - 1;
    worldDimension[1] = 10 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 1 - 1;
    walls[0][1] = 3 - 1;
    walls[0][2] = 1 - 1;
    walls[0][3] = 4 - 1;
    walls[1][0] = 2 - 1;
    walls[1][1] = 3 - 1;
    walls[1][2] = 2 - 1;
    walls[1][3] = 4 - 1;
    walls[2][0] = 3 - 1;
    walls[2][1] = 3 - 1;
    walls[2][2] = 3 - 1;
    walls[2][3] = 4 - 1;
    walls[3][0] = 4 - 1;
    walls[3][1] = 3 - 1;
    walls[3][2] = 4 - 1;
    walls[3][3] = 4 - 1;
    walls[4][0] = 4 - 1;
    walls[4][1] = 4 - 1;
    walls[4][2] = 5 - 1;
    walls[4][3] = 4 - 1;
    walls[5][0] = 1 - 1;
    walls[5][1] = 4 - 1;
    walls[5][2] = 1 - 1;
    walls[5][3] = 5 - 1;
    walls[6][0] = 2 - 1;
    walls[6][1] = 4 - 1;
    walls[6][2] = 2 - 1;
    walls[6][3] = 5 - 1;
    walls[7][0] = 3 - 1;
    walls[7][1] = 4 - 1;
    walls[7][2] = 3 - 1;
    walls[7][3] = 5 - 1;
    walls[8][0] = 4 - 1;
    walls[8][1] = 4 - 1;
    walls[8][2] = 4 - 1;
    walls[8][3] = 5 - 1;
    walls[9][0] = 1 - 1;
    walls[9][1] = 7 - 1;
    walls[9][2] = 1 - 1;
    walls[9][3] = 8 - 1;
    walls[10][0] = 1 - 1;
    walls[10][1] = 8 - 1;
    walls[10][2] = 2 - 1;
    walls[10][3] = 8 - 1;
    walls[11][0] = 2 - 1;
    walls[11][1] = 8 - 1;
    walls[11][2] = 2 - 1;
    walls[11][3] = 9 - 1;
    walls[12][0] = 2 - 1;
    walls[12][1] = 9 - 1;
    walls[12][2] = 3 - 1;
    walls[12][3] = 9 - 1;
    walls[13][0] = 3 - 1;
    walls[13][1] = 9 - 1;
    walls[13][2] = 3 - 1;
    walls[13][3] = 10 - 1;
    walls[14][0] = 3 - 1;
    walls[14][1] = 10 - 1;
    walls[14][2] = 4 - 1;
    walls[14][3] = 10 - 1;
    beeper[0][0] = 4 - 1;
    beeper[0][1] = 10 - 1;
    beeper[0][2] = 1;
}

void worldSmallCave()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 10 - 1;
    worldDimension[1] = 7 - 1;
    karelPosition[0] = 5 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 4 - 1;
    walls[0][1] = 1 - 1;
    walls[0][2] = 5 - 1;
    walls[0][3] = 1 - 1;
    walls[1][0] = 5 - 1;
    walls[1][1] = 1 - 1;
    walls[1][2] = 6 - 1;
    walls[1][3] = 1 - 1;
    walls[2][0] = 4 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 5 - 1;
    walls[2][3] = 2 - 1;
    walls[3][0] = 5 - 1;
    walls[3][1] = 2 - 1;
    walls[3][2] = 6 - 1;
    walls[3][3] = 2 - 1;
    walls[4][0] = 4 - 1;
    walls[4][1] = 2 - 1;
    walls[4][2] = 4 - 1;
    walls[4][3] = 3 - 1;
    walls[5][0] = 3 - 1;
    walls[5][1] = 3 - 1;
    walls[5][2] = 4 - 1;
    walls[5][3] = 3 - 1;
    walls[6][0] = 6 - 1;
    walls[6][1] = 2 - 1;
    walls[6][2] = 6 - 1;
    walls[6][3] = 3 - 1;
    walls[7][0] = 7 - 1;
    walls[7][1] = 2 - 1;
    walls[7][2] = 7 - 1;
    walls[7][3] = 3 - 1;
    walls[8][0] = 7 - 1;
    walls[8][1] = 3 - 1;
    walls[8][2] = 8 - 1;
    walls[8][3] = 3 - 1;
    walls[9][0] = 3 - 1;
    walls[9][1] = 4 - 1;
    walls[9][2] = 4 - 1;
    walls[9][3] = 4 - 1;
    walls[10][0] = 8 - 1;
    walls[10][1] = 3 - 1;
    walls[10][2] = 8 - 1;
    walls[10][3] = 4 - 1;
    walls[11][0] = 8 - 1;
    walls[11][1] = 4 - 1;
    walls[11][2] = 9 - 1;
    walls[11][3] = 4 - 1;
    walls[12][0] = 3 - 1;
    walls[12][1] = 4 - 1;
    walls[12][2] = 3 - 1;
    walls[12][3] = 5 - 1;
    walls[13][0] = 2 - 1;
    walls[13][1] = 5 - 1;
    walls[13][2] = 3 - 1;
    walls[13][3] = 5 - 1;
    walls[14][0] = 8 - 1;
    walls[14][1] = 5 - 1;
    walls[14][2] = 9 - 1;
    walls[14][3] = 5 - 1;
    walls[15][0] = 1 - 1;
    walls[15][1] = 5 - 1;
    walls[15][2] = 1 - 1;
    walls[15][3] = 6 - 1;
    walls[16][0] = 2 - 1;
    walls[16][1] = 5 - 1;
    walls[16][2] = 2 - 1;
    walls[16][3] = 6 - 1;
    walls[17][0] = 9 - 1;
    walls[17][1] = 5 - 1;
    walls[17][2] = 9 - 1;
    walls[17][3] = 6 - 1;
    walls[18][0] = 9 - 1;
    walls[18][1] = 6 - 1;
    walls[18][2] = 10 - 1;
    walls[18][3] = 6 - 1;
    walls[19][0] = 10 - 1;
    walls[19][1] = 6 - 1;
    walls[19][2] = 10 - 1;
    walls[19][3] = 7 - 1;
    beeper[0][0] = 4 - 1;
    beeper[0][1] = 3 - 1;
    beeper[0][2] = 1;
    beeper[1][0] = 5 - 1;
    beeper[1][1] = 3 - 1;
    beeper[1][2] = 1;
    beeper[2][0] = 6 - 1;
    beeper[2][1] = 3 - 1;
    beeper[2][2] = 1;
    beeper[3][0] = 3 - 1;
    beeper[3][1] = 5 - 1;
    beeper[3][2] = 1;
    beeper[4][0] = 4 - 1;
    beeper[4][1] = 5 - 1;
    beeper[4][2] = 1;
    beeper[5][0] = 5 - 1;
    beeper[5][1] = 5 - 1;
    beeper[5][2] = 1;
    beeper[6][0] = 7 - 1;
    beeper[6][1] = 5 - 1;
    beeper[6][2] = 1;
    beeper[7][0] = 8 - 1;
    beeper[7][1] = 5 - 1;
    beeper[7][2] = 1;
    beeper[8][0] = 6 - 1;
    beeper[8][1] = 6 - 1;
    beeper[8][2] = 1;
    beeper[9][0] = 7 - 1;
    beeper[9][1] = 6 - 1;
    beeper[9][2] = 1;
    beeper[10][0] = 8 - 1;
    beeper[10][1] = 6 - 1;
    beeper[10][2] = 1;
    beeper[11][0] = 9 - 1;
    beeper[11][1] = 6 - 1;
    beeper[11][2] = 1;
    beeper[12][0] = 1 - 1;
    beeper[12][1] = 7 - 1;
    beeper[12][2] = 1;
    beeper[13][0] = 2 - 1;
    beeper[13][1] = 7 - 1;
    beeper[13][2] = 1;
    beeper[14][0] = 3 - 1;
    beeper[14][1] = 7 - 1;
    beeper[14][2] = 1;
    beeper[15][0] = 4 - 1;
    beeper[15][1] = 7 - 1;
    beeper[15][2] = 1;
    beeper[16][0] = 5 - 1;
    beeper[16][1] = 7 - 1;
    beeper[16][2] = 1;
}

void worldSmallCaveTest()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 10 - 1;
    worldDimension[1] = 7 - 1;
    karelPosition[0] = 5 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 4 - 1;
    walls[0][1] = 1 - 1;
    walls[0][2] = 5 - 1;
    walls[0][3] = 1 - 1;
    walls[1][0] = 5 - 1;
    walls[1][1] = 1 - 1;
    walls[1][2] = 6 - 1;
    walls[1][3] = 1 - 1;
    walls[2][0] = 4 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 5 - 1;
    walls[2][3] = 2 - 1;
    walls[3][0] = 5 - 1;
    walls[3][1] = 2 - 1;
    walls[3][2] = 6 - 1;
    walls[3][3] = 2 - 1;
    walls[4][0] = 4 - 1;
    walls[4][1] = 2 - 1;
    walls[4][2] = 4 - 1;
    walls[4][3] = 3 - 1;
    walls[5][0] = 3 - 1;
    walls[5][1] = 3 - 1;
    walls[5][2] = 4 - 1;
    walls[5][3] = 3 - 1;
    walls[6][0] = 6 - 1;
    walls[6][1] = 2 - 1;
    walls[6][2] = 6 - 1;
    walls[6][3] = 3 - 1;
    walls[7][0] = 7 - 1;
    walls[7][1] = 2 - 1;
    walls[7][2] = 7 - 1;
    walls[7][3] = 3 - 1;
    walls[8][0] = 7 - 1;
    walls[8][1] = 3 - 1;
    walls[8][2] = 8 - 1;
    walls[8][3] = 3 - 1;
    walls[9][0] = 3 - 1;
    walls[9][1] = 4 - 1;
    walls[9][2] = 4 - 1;
    walls[9][3] = 4 - 1;
    walls[10][0] = 8 - 1;
    walls[10][1] = 3 - 1;
    walls[10][2] = 8 - 1;
    walls[10][3] = 4 - 1;
    walls[11][0] = 8 - 1;
    walls[11][1] = 4 - 1;
    walls[11][2] = 9 - 1;
    walls[11][3] = 4 - 1;
    walls[12][0] = 3 - 1;
    walls[12][1] = 4 - 1;
    walls[12][2] = 3 - 1;
    walls[12][3] = 5 - 1;
    walls[13][0] = 2 - 1;
    walls[13][1] = 5 - 1;
    walls[13][2] = 3 - 1;
    walls[13][3] = 5 - 1;
    walls[14][0] = 8 - 1;
    walls[14][1] = 5 - 1;
    walls[14][2] = 9 - 1;
    walls[14][3] = 5 - 1;
    walls[15][0] = 1 - 1;
    walls[15][1] = 5 - 1;
    walls[15][2] = 1 - 1;
    walls[15][3] = 6 - 1;
    walls[16][0] = 2 - 1;
    walls[16][1] = 5 - 1;
    walls[16][2] = 2 - 1;
    walls[16][3] = 6 - 1;
    walls[17][0] = 9 - 1;
    walls[17][1] = 5 - 1;
    walls[17][2] = 9 - 1;
    walls[17][3] = 6 - 1;
    walls[18][0] = 9 - 1;
    walls[18][1] = 6 - 1;
    walls[18][2] = 10 - 1;
    walls[18][3] = 6 - 1;
    walls[19][0] = 10 - 1;
    walls[19][1] = 6 - 1;
    walls[19][2] = 10 - 1;
    walls[19][3] = 7 - 1;
    beeper[0][0] = 4 - 1;
    beeper[0][1] = 3 - 1;
    beeper[0][2] = 1;
    beeper[1][0] = 5 - 1;
    beeper[1][1] = 3 - 1;
    beeper[1][2] = 1;
    beeper[2][0] = 6 - 1;
    beeper[2][1] = 3 - 1;
    beeper[2][2] = 1;
    beeper[3][0] = 3 - 1;
    beeper[3][1] = 5 - 1;
    beeper[3][2] = 1;
    beeper[4][0] = 4 - 1;
    beeper[4][1] = 5 - 1;
    beeper[4][2] = 1;
    beeper[5][0] = 5 - 1;
    beeper[5][1] = 5 - 1;
    beeper[5][2] = 1;
    beeper[6][0] = 7 - 1;
    beeper[6][1] = 5 - 1;
    beeper[6][2] = 1;
    beeper[7][0] = 8 - 1;
    beeper[7][1] = 5 - 1;
    beeper[7][2] = 1;
    beeper[8][0] = 6 - 1;
    beeper[8][1] = 6 - 1;
    beeper[8][2] = 1;
    beeper[9][0] = 7 - 1;
    beeper[9][1] = 6 - 1;
    beeper[9][2] = 1;
    beeper[10][0] = 8 - 1;
    beeper[10][1] = 6 - 1;
    beeper[10][2] = 1;
    beeper[11][0] = 9 - 1;
    beeper[11][1] = 6 - 1;
    beeper[11][2] = 1;
    beeper[12][0] = 1 - 1;
    beeper[12][1] = 7 - 1;
    beeper[12][2] = 1;
    beeper[13][0] = 2 - 1;
    beeper[13][1] = 7 - 1;
    beeper[13][2] = 1;
    beeper[14][0] = 3 - 1;
    beeper[14][1] = 7 - 1;
    beeper[14][2] = 1;
    beeper[15][0] = 4 - 1;
    beeper[15][1] = 7 - 1;
    beeper[15][2] = 1;
    beeper[16][0] = 5 - 1;
    beeper[16][1] = 7 - 1;
    beeper[16][2] = 1;
}

void worldSmallHall()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 6 - 1;
    worldDimension[1] = 6 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
}

void worldSteepleChase()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 10 - 1;
    worldDimension[1] = 15 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 1 - 1;
    walls[0][1] = 2 - 1;
    walls[0][2] = 1 - 1;
    walls[0][3] = 3 - 1;
    walls[1][0] = 2 - 1;
    walls[1][1] = 2 - 1;
    walls[1][2] = 2 - 1;
    walls[1][3] = 3 - 1;
    walls[2][0] = 3 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 3 - 1;
    walls[2][3] = 3 - 1;
    walls[3][0] = 4 - 1;
    walls[3][1] = 2 - 1;
    walls[3][2] = 4 - 1;
    walls[3][3] = 3 - 1;
    walls[4][0] = 5 - 1;
    walls[4][1] = 2 - 1;
    walls[4][2] = 5 - 1;
    walls[4][3] = 3 - 1;
    walls[5][0] = 1 - 1;
    walls[5][1] = 4 - 1;
    walls[5][2] = 1 - 1;
    walls[5][3] = 5 - 1;
    walls[6][0] = 2 - 1;
    walls[6][1] = 4 - 1;
    walls[6][2] = 2 - 1;
    walls[6][3] = 5 - 1;
    walls[7][0] = 3 - 1;
    walls[7][1] = 4 - 1;
    walls[7][2] = 3 - 1;
    walls[7][3] = 5 - 1;
    walls[8][0] = 4 - 1;
    walls[8][1] = 4 - 1;
    walls[8][2] = 4 - 1;
    walls[8][3] = 5 - 1;
    walls[9][0] = 5 - 1;
    walls[9][1] = 4 - 1;
    walls[9][2] = 5 - 1;
    walls[9][3] = 5 - 1;
    walls[10][0] = 1 - 1;
    walls[10][1] = 6 - 1;
    walls[10][2] = 1 - 1;
    walls[10][3] = 7 - 1;
    walls[11][0] = 2 - 1;
    walls[11][1] = 6 - 1;
    walls[11][2] = 2 - 1;
    walls[11][3] = 7 - 1;
    walls[12][0] = 3 - 1;
    walls[12][1] = 6 - 1;
    walls[12][2] = 3 - 1;
    walls[12][3] = 7 - 1;
    walls[13][0] = 4 - 1;
    walls[13][1] = 6 - 1;
    walls[13][2] = 4 - 1;
    walls[13][3] = 7 - 1;
    walls[14][0] = 5 - 1;
    walls[14][1] = 6 - 1;
    walls[14][2] = 5 - 1;
    walls[14][3] = 7 - 1;
    walls[15][0] = 6 - 1;
    walls[15][1] = 6 - 1;
    walls[15][2] = 6 - 1;
    walls[15][3] = 7 - 1;
    walls[16][0] = 7 - 1;
    walls[16][1] = 6 - 1;
    walls[16][2] = 7 - 1;
    walls[16][3] = 7 - 1;
    walls[17][0] = 8 - 1;
    walls[17][1] = 6 - 1;
    walls[17][2] = 8 - 1;
    walls[17][3] = 7 - 1;
    walls[18][0] = 1 - 1;
    walls[18][1] = 8 - 1;
    walls[18][2] = 1 - 1;
    walls[18][3] = 9 - 1;
    walls[19][0] = 2 - 1;
    walls[19][1] = 8 - 1;
    walls[19][2] = 2 - 1;
    walls[19][3] = 9 - 1;
    walls[20][0] = 1 - 1;
    walls[20][1] = 9 - 1;
    walls[20][2] = 1 - 1;
    walls[20][3] = 10 - 1;
    walls[21][0] = 2 - 1;
    walls[21][1] = 9 - 1;
    walls[21][2] = 2 - 1;
    walls[21][3] = 10 - 1;
    walls[22][0] = 1 - 1;
    walls[22][1] = 11 - 1;
    walls[22][2] = 1 - 1;
    walls[22][3] = 12 - 1;
    walls[23][0] = 2 - 1;
    walls[23][1] = 11 - 1;
    walls[23][2] = 2 - 1;
    walls[23][3] = 12 - 1;
    walls[24][0] = 3 - 1;
    walls[24][1] = 11 - 1;
    walls[24][2] = 3 - 1;
    walls[24][3] = 12 - 1;
    walls[25][0] = 4 - 1;
    walls[25][1] = 11 - 1;
    walls[25][2] = 4 - 1;
    walls[25][3] = 12 - 1;
    walls[26][0] = 1 - 1;
    walls[26][1] = 13 - 1;
    walls[26][2] = 1 - 1;
    walls[26][3] = 14 - 1;
    walls[27][0] = 2 - 1;
    walls[27][1] = 13 - 1;
    walls[27][2] = 2 - 1;
    walls[27][3] = 14 - 1;
    walls[28][0] = 3 - 1;
    walls[28][1] = 13 - 1;
    walls[28][2] = 3 - 1;
    walls[28][3] = 14 - 1;
    walls[29][0] = 4 - 1;
    walls[29][1] = 13 - 1;
    walls[29][2] = 4 - 1;
    walls[29][3] = 14 - 1;
}

void worldSteepSteepleChase()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 32 - 1;
    worldDimension[1] = 15 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 1 - 1;
    walls[0][1] = 2 - 1;
    walls[0][2] = 1 - 1;
    walls[0][3] = 3 - 1;
    walls[1][0] = 2 - 1;
    walls[1][1] = 2 - 1;
    walls[1][2] = 2 - 1;
    walls[1][3] = 3 - 1;
    walls[2][0] = 3 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 3 - 1;
    walls[2][3] = 3 - 1;
    walls[3][0] = 4 - 1;
    walls[3][1] = 2 - 1;
    walls[3][2] = 4 - 1;
    walls[3][3] = 3 - 1;
    walls[4][0] = 5 - 1;
    walls[4][1] = 2 - 1;
    walls[4][2] = 5 - 1;
    walls[4][3] = 3 - 1;
    walls[5][0] = 6 - 1;
    walls[5][1] = 2 - 1;
    walls[5][2] = 6 - 1;
    walls[5][3] = 3 - 1;
    walls[6][0] = 7 - 1;
    walls[6][1] = 2 - 1;
    walls[6][2] = 7 - 1;
    walls[6][3] = 3 - 1;
    walls[7][0] = 8 - 1;
    walls[7][1] = 2 - 1;
    walls[7][2] = 8 - 1;
    walls[7][3] = 3 - 1;
    walls[8][0] = 1 - 1;
    walls[8][1] = 3 - 1;
    walls[8][2] = 1 - 1;
    walls[8][3] = 4 - 1;
    walls[9][0] = 2 - 1;
    walls[9][1] = 3 - 1;
    walls[9][2] = 2 - 1;
    walls[9][3] = 4 - 1;
    walls[10][0] = 3 - 1;
    walls[10][1] = 3 - 1;
    walls[10][2] = 3 - 1;
    walls[10][3] = 4 - 1;
    walls[11][0] = 4 - 1;
    walls[11][1] = 3 - 1;
    walls[11][2] = 4 - 1;
    walls[11][3] = 4 - 1;
    walls[12][0] = 5 - 1;
    walls[12][1] = 3 - 1;
    walls[12][2] = 5 - 1;
    walls[12][3] = 4 - 1;
    walls[13][0] = 6 - 1;
    walls[13][1] = 3 - 1;
    walls[13][2] = 6 - 1;
    walls[13][3] = 4 - 1;
    walls[14][0] = 7 - 1;
    walls[14][1] = 3 - 1;
    walls[14][2] = 7 - 1;
    walls[14][3] = 4 - 1;
    walls[15][0] = 8 - 1;
    walls[15][1] = 3 - 1;
    walls[15][2] = 8 - 1;
    walls[15][3] = 4 - 1;
    walls[16][0] = 9 - 1;
    walls[16][1] = 3 - 1;
    walls[16][2] = 9 - 1;
    walls[16][3] = 4 - 1;
    walls[17][0] = 10 - 1;
    walls[17][1] = 3 - 1;
    walls[17][2] = 10 - 1;
    walls[17][3] = 4 - 1;
    walls[18][0] = 11 - 1;
    walls[18][1] = 3 - 1;
    walls[18][2] = 11 - 1;
    walls[18][3] = 4 - 1;
    walls[19][0] = 12 - 1;
    walls[19][1] = 3 - 1;
    walls[19][2] = 12 - 1;
    walls[19][3] = 4 - 1;
    walls[20][0] = 13 - 1;
    walls[20][1] = 3 - 1;
    walls[20][2] = 13 - 1;
    walls[20][3] = 4 - 1;
    walls[21][0] = 14 - 1;
    walls[21][1] = 3 - 1;
    walls[21][2] = 14 - 1;
    walls[21][3] = 4 - 1;
    walls[22][0] = 15 - 1;
    walls[22][1] = 3 - 1;
    walls[22][2] = 15 - 1;
    walls[22][3] = 4 - 1;
    walls[23][0] = 16 - 1;
    walls[23][1] = 3 - 1;
    walls[23][2] = 16 - 1;
    walls[23][3] = 4 - 1;
    walls[24][0] = 17 - 1;
    walls[24][1] = 3 - 1;
    walls[24][2] = 17 - 1;
    walls[24][3] = 4 - 1;
    walls[25][0] = 18 - 1;
    walls[25][1] = 3 - 1;
    walls[25][2] = 18 - 1;
    walls[25][3] = 4 - 1;
    walls[26][0] = 19 - 1;
    walls[26][1] = 3 - 1;
    walls[26][2] = 19 - 1;
    walls[26][3] = 4 - 1;
    walls[27][0] = 20 - 1;
    walls[27][1] = 3 - 1;
    walls[27][2] = 20 - 1;
    walls[27][3] = 4 - 1;
    walls[28][0] = 21 - 1;
    walls[28][1] = 3 - 1;
    walls[28][2] = 21 - 1;
    walls[28][3] = 4 - 1;
    walls[29][0] = 22 - 1;
    walls[29][1] = 3 - 1;
    walls[29][2] = 22 - 1;
    walls[29][3] = 4 - 1;
    walls[30][0] = 23 - 1;
    walls[30][1] = 3 - 1;
    walls[30][2] = 23 - 1;
    walls[30][3] = 4 - 1;
    walls[31][0] = 24 - 1;
    walls[31][1] = 3 - 1;
    walls[31][2] = 24 - 1;
    walls[31][3] = 4 - 1;
    walls[32][0] = 25 - 1;
    walls[32][1] = 3 - 1;
    walls[32][2] = 25 - 1;
    walls[32][3] = 4 - 1;
    walls[33][0] = 26 - 1;
    walls[33][1] = 3 - 1;
    walls[33][2] = 26 - 1;
    walls[33][3] = 4 - 1;
    walls[34][0] = 27 - 1;
    walls[34][1] = 3 - 1;
    walls[34][2] = 27 - 1;
    walls[34][3] = 4 - 1;
    walls[35][0] = 28 - 1;
    walls[35][1] = 3 - 1;
    walls[35][2] = 28 - 1;
    walls[35][3] = 4 - 1;
    walls[36][0] = 29 - 1;
    walls[36][1] = 3 - 1;
    walls[36][2] = 29 - 1;
    walls[36][3] = 4 - 1;
    walls[37][0] = 30 - 1;
    walls[37][1] = 3 - 1;
    walls[37][2] = 30 - 1;
    walls[37][3] = 4 - 1;
    walls[38][0] = 1 - 1;
    walls[38][1] = 5 - 1;
    walls[38][2] = 1 - 1;
    walls[38][3] = 6 - 1;
    walls[39][0] = 2 - 1;
    walls[39][1] = 5 - 1;
    walls[39][2] = 2 - 1;
    walls[39][3] = 6 - 1;
    walls[40][0] = 3 - 1;
    walls[40][1] = 5 - 1;
    walls[40][2] = 3 - 1;
    walls[40][3] = 6 - 1;
    walls[41][0] = 4 - 1;
    walls[41][1] = 5 - 1;
    walls[41][2] = 4 - 1;
    walls[41][3] = 6 - 1;
    walls[42][0] = 5 - 1;
    walls[42][1] = 5 - 1;
    walls[42][2] = 5 - 1;
    walls[42][3] = 6 - 1;
    walls[43][0] = 6 - 1;
    walls[43][1] = 5 - 1;
    walls[43][2] = 6 - 1;
    walls[43][3] = 6 - 1;
    walls[44][0] = 7 - 1;
    walls[44][1] = 5 - 1;
    walls[44][2] = 7 - 1;
    walls[44][3] = 6 - 1;
    walls[45][0] = 8 - 1;
    walls[45][1] = 5 - 1;
    walls[45][2] = 8 - 1;
    walls[45][3] = 6 - 1;
    walls[46][0] = 9 - 1;
    walls[46][1] = 5 - 1;
    walls[46][2] = 9 - 1;
    walls[46][3] = 6 - 1;
    walls[47][0] = 10 - 1;
    walls[47][1] = 5 - 1;
    walls[47][2] = 10 - 1;
    walls[47][3] = 6 - 1;
    walls[48][0] = 11 - 1;
    walls[48][1] = 5 - 1;
    walls[48][2] = 11 - 1;
    walls[48][3] = 6 - 1;
    walls[49][0] = 12 - 1;
    walls[49][1] = 5 - 1;
    walls[49][2] = 12 - 1;
    walls[49][3] = 6 - 1;
    walls[50][0] = 13 - 1;
    walls[50][1] = 5 - 1;
    walls[50][2] = 13 - 1;
    walls[50][3] = 6 - 1;
    walls[51][0] = 14 - 1;
    walls[51][1] = 5 - 1;
    walls[51][2] = 14 - 1;
    walls[51][3] = 6 - 1;
    walls[52][0] = 15 - 1;
    walls[52][1] = 5 - 1;
    walls[52][2] = 15 - 1;
    walls[52][3] = 6 - 1;
    walls[53][0] = 16 - 1;
    walls[53][1] = 5 - 1;
    walls[53][2] = 16 - 1;
    walls[53][3] = 6 - 1;
    walls[54][0] = 17 - 1;
    walls[54][1] = 5 - 1;
    walls[54][2] = 17 - 1;
    walls[54][3] = 6 - 1;
    walls[55][0] = 18 - 1;
    walls[55][1] = 5 - 1;
    walls[55][2] = 18 - 1;
    walls[55][3] = 6 - 1;
    walls[56][0] = 19 - 1;
    walls[56][1] = 5 - 1;
    walls[56][2] = 19 - 1;
    walls[56][3] = 6 - 1;
    walls[57][0] = 20 - 1;
    walls[57][1] = 5 - 1;
    walls[57][2] = 20 - 1;
    walls[57][3] = 6 - 1;
    walls[58][0] = 21 - 1;
    walls[58][1] = 5 - 1;
    walls[58][2] = 21 - 1;
    walls[58][3] = 6 - 1;
    walls[59][0] = 22 - 1;
    walls[59][1] = 5 - 1;
    walls[59][2] = 22 - 1;
    walls[59][3] = 6 - 1;
    walls[60][0] = 23 - 1;
    walls[60][1] = 5 - 1;
    walls[60][2] = 23 - 1;
    walls[60][3] = 6 - 1;
    walls[61][0] = 24 - 1;
    walls[61][1] = 5 - 1;
    walls[61][2] = 24 - 1;
    walls[61][3] = 6 - 1;
    walls[62][0] = 25 - 1;
    walls[62][1] = 5 - 1;
    walls[62][2] = 25 - 1;
    walls[62][3] = 6 - 1;
    walls[63][0] = 26 - 1;
    walls[63][1] = 5 - 1;
    walls[63][2] = 26 - 1;
    walls[63][3] = 6 - 1;
    walls[64][0] = 27 - 1;
    walls[64][1] = 5 - 1;
    walls[64][2] = 27 - 1;
    walls[64][3] = 6 - 1;
    walls[65][0] = 28 - 1;
    walls[65][1] = 5 - 1;
    walls[65][2] = 28 - 1;
    walls[65][3] = 6 - 1;
    walls[66][0] = 1 - 1;
    walls[66][1] = 7 - 1;
    walls[66][2] = 1 - 1;
    walls[66][3] = 8 - 1;
    walls[67][0] = 2 - 1;
    walls[67][1] = 7 - 1;
    walls[67][2] = 2 - 1;
    walls[67][3] = 8 - 1;
    walls[68][0] = 3 - 1;
    walls[68][1] = 7 - 1;
    walls[68][2] = 3 - 1;
    walls[68][3] = 8 - 1;
    walls[69][0] = 4 - 1;
    walls[69][1] = 7 - 1;
    walls[69][2] = 4 - 1;
    walls[69][3] = 8 - 1;
    walls[70][0] = 5 - 1;
    walls[70][1] = 7 - 1;
    walls[70][2] = 5 - 1;
    walls[70][3] = 8 - 1;
    walls[71][0] = 6 - 1;
    walls[71][1] = 7 - 1;
    walls[71][2] = 6 - 1;
    walls[71][3] = 8 - 1;
    walls[72][0] = 7 - 1;
    walls[72][1] = 7 - 1;
    walls[72][2] = 7 - 1;
    walls[72][3] = 8 - 1;
    walls[73][0] = 8 - 1;
    walls[73][1] = 7 - 1;
    walls[73][2] = 8 - 1;
    walls[73][3] = 8 - 1;
    walls[74][0] = 9 - 1;
    walls[74][1] = 7 - 1;
    walls[74][2] = 9 - 1;
    walls[74][3] = 8 - 1;
    walls[75][0] = 10 - 1;
    walls[75][1] = 7 - 1;
    walls[75][2] = 10 - 1;
    walls[75][3] = 8 - 1;
    walls[76][0] = 11 - 1;
    walls[76][1] = 7 - 1;
    walls[76][2] = 11 - 1;
    walls[76][3] = 8 - 1;
    walls[77][0] = 12 - 1;
    walls[77][1] = 7 - 1;
    walls[77][2] = 12 - 1;
    walls[77][3] = 8 - 1;
    walls[78][0] = 13 - 1;
    walls[78][1] = 7 - 1;
    walls[78][2] = 13 - 1;
    walls[78][3] = 8 - 1;
    walls[79][0] = 14 - 1;
    walls[79][1] = 7 - 1;
    walls[79][2] = 14 - 1;
    walls[79][3] = 8 - 1;
    walls[80][0] = 15 - 1;
    walls[80][1] = 7 - 1;
    walls[80][2] = 15 - 1;
    walls[80][3] = 8 - 1;
    walls[81][0] = 16 - 1;
    walls[81][1] = 7 - 1;
    walls[81][2] = 16 - 1;
    walls[81][3] = 8 - 1;
    walls[82][0] = 17 - 1;
    walls[82][1] = 7 - 1;
    walls[82][2] = 17 - 1;
    walls[82][3] = 8 - 1;
    walls[83][0] = 18 - 1;
    walls[83][1] = 7 - 1;
    walls[83][2] = 18 - 1;
    walls[83][3] = 8 - 1;
    walls[84][0] = 19 - 1;
    walls[84][1] = 7 - 1;
    walls[84][2] = 19 - 1;
    walls[84][3] = 8 - 1;
    walls[85][0] = 20 - 1;
    walls[85][1] = 7 - 1;
    walls[85][2] = 20 - 1;
    walls[85][3] = 8 - 1;
    walls[86][0] = 21 - 1;
    walls[86][1] = 7 - 1;
    walls[86][2] = 21 - 1;
    walls[86][3] = 8 - 1;
    walls[87][0] = 22 - 1;
    walls[87][1] = 7 - 1;
    walls[87][2] = 22 - 1;
    walls[87][3] = 8 - 1;
    walls[88][0] = 23 - 1;
    walls[88][1] = 7 - 1;
    walls[88][2] = 23 - 1;
    walls[88][3] = 8 - 1;
    walls[89][0] = 24 - 1;
    walls[89][1] = 7 - 1;
    walls[89][2] = 24 - 1;
    walls[89][3] = 8 - 1;
    walls[90][0] = 25 - 1;
    walls[90][1] = 7 - 1;
    walls[90][2] = 25 - 1;
    walls[90][3] = 8 - 1;
    walls[91][0] = 26 - 1;
    walls[91][1] = 7 - 1;
    walls[91][2] = 26 - 1;
    walls[91][3] = 8 - 1;
    walls[92][0] = 27 - 1;
    walls[92][1] = 7 - 1;
    walls[92][2] = 27 - 1;
    walls[92][3] = 8 - 1;
    walls[93][0] = 28 - 1;
    walls[93][1] = 7 - 1;
    walls[93][2] = 28 - 1;
    walls[93][3] = 8 - 1;
    walls[94][0] = 1 - 1;
    walls[94][1] = 11 - 1;
    walls[94][2] = 1 - 1;
    walls[94][3] = 12 - 1;
    walls[95][0] = 2 - 1;
    walls[95][1] = 11 - 1;
    walls[95][2] = 2 - 1;
    walls[95][3] = 12 - 1;
    walls[96][0] = 3 - 1;
    walls[96][1] = 11 - 1;
    walls[96][2] = 3 - 1;
    walls[96][3] = 12 - 1;
    walls[97][0] = 4 - 1;
    walls[97][1] = 11 - 1;
    walls[97][2] = 4 - 1;
    walls[97][3] = 12 - 1;
    walls[98][0] = 5 - 1;
    walls[98][1] = 11 - 1;
    walls[98][2] = 5 - 1;
    walls[98][3] = 12 - 1;
    walls[99][0] = 6 - 1;
    walls[99][1] = 11 - 1;
    walls[99][2] = 6 - 1;
    walls[99][3] = 12 - 1;
    walls[100][0] = 7 - 1;
    walls[100][1] = 11 - 1;
    walls[100][2] = 7 - 1;
    walls[100][3] = 12 - 1;
    walls[101][0] = 8 - 1;
    walls[101][1] = 11 - 1;
    walls[101][2] = 8 - 1;
    walls[101][3] = 12 - 1;
    walls[102][0] = 9 - 1;
    walls[102][1] = 11 - 1;
    walls[102][2] = 9 - 1;
    walls[102][3] = 12 - 1;
    walls[103][0] = 10 - 1;
    walls[103][1] = 11 - 1;
    walls[103][2] = 10 - 1;
    walls[103][3] = 12 - 1;
    walls[104][0] = 11 - 1;
    walls[104][1] = 11 - 1;
    walls[104][2] = 11 - 1;
    walls[104][3] = 12 - 1;
    walls[105][0] = 12 - 1;
    walls[105][1] = 11 - 1;
    walls[105][2] = 12 - 1;
    walls[105][3] = 12 - 1;
    walls[106][0] = 13 - 1;
    walls[106][1] = 11 - 1;
    walls[106][2] = 13 - 1;
    walls[106][3] = 12 - 1;
    walls[107][0] = 14 - 1;
    walls[107][1] = 11 - 1;
    walls[107][2] = 14 - 1;
    walls[107][3] = 12 - 1;
    walls[108][0] = 15 - 1;
    walls[108][1] = 11 - 1;
    walls[108][2] = 15 - 1;
    walls[108][3] = 12 - 1;
    walls[109][0] = 16 - 1;
    walls[109][1] = 11 - 1;
    walls[109][2] = 16 - 1;
    walls[109][3] = 12 - 1;
    walls[110][0] = 17 - 1;
    walls[110][1] = 11 - 1;
    walls[110][2] = 17 - 1;
    walls[110][3] = 12 - 1;
    walls[111][0] = 18 - 1;
    walls[111][1] = 11 - 1;
    walls[111][2] = 18 - 1;
    walls[111][3] = 12 - 1;
    walls[112][0] = 19 - 1;
    walls[112][1] = 11 - 1;
    walls[112][2] = 19 - 1;
    walls[112][3] = 12 - 1;
    walls[113][0] = 20 - 1;
    walls[113][1] = 11 - 1;
    walls[113][2] = 20 - 1;
    walls[113][3] = 12 - 1;
    walls[114][0] = 21 - 1;
    walls[114][1] = 11 - 1;
    walls[114][2] = 21 - 1;
    walls[114][3] = 12 - 1;
    walls[115][0] = 22 - 1;
    walls[115][1] = 11 - 1;
    walls[115][2] = 22 - 1;
    walls[115][3] = 12 - 1;
    walls[116][0] = 23 - 1;
    walls[116][1] = 11 - 1;
    walls[116][2] = 23 - 1;
    walls[116][3] = 12 - 1;
    walls[117][0] = 24 - 1;
    walls[117][1] = 11 - 1;
    walls[117][2] = 24 - 1;
    walls[117][3] = 12 - 1;
    walls[118][0] = 25 - 1;
    walls[118][1] = 11 - 1;
    walls[118][2] = 25 - 1;
    walls[118][3] = 12 - 1;
    walls[119][0] = 26 - 1;
    walls[119][1] = 11 - 1;
    walls[119][2] = 26 - 1;
    walls[119][3] = 12 - 1;
    walls[120][0] = 1 - 1;
    walls[120][1] = 13 - 1;
    walls[120][2] = 1 - 1;
    walls[120][3] = 14 - 1;
    walls[121][0] = 2 - 1;
    walls[121][1] = 13 - 1;
    walls[121][2] = 2 - 1;
    walls[121][3] = 14 - 1;
    walls[122][0] = 3 - 1;
    walls[122][1] = 13 - 1;
    walls[122][2] = 3 - 1;
    walls[122][3] = 14 - 1;
    walls[123][0] = 4 - 1;
    walls[123][1] = 13 - 1;
    walls[123][2] = 4 - 1;
    walls[123][3] = 14 - 1;
    walls[124][0] = 5 - 1;
    walls[124][1] = 13 - 1;
    walls[124][2] = 5 - 1;
    walls[124][3] = 14 - 1;
    walls[125][0] = 6 - 1;
    walls[125][1] = 13 - 1;
    walls[125][2] = 6 - 1;
    walls[125][3] = 14 - 1;
    walls[126][0] = 7 - 1;
    walls[126][1] = 13 - 1;
    walls[126][2] = 7 - 1;
    walls[126][3] = 14 - 1;
    walls[127][0] = 8 - 1;
    walls[127][1] = 13 - 1;
    walls[127][2] = 8 - 1;
    walls[127][3] = 14 - 1;
    walls[128][0] = 9 - 1;
    walls[128][1] = 13 - 1;
    walls[128][2] = 9 - 1;
    walls[128][3] = 14 - 1;
    walls[129][0] = 10 - 1;
    walls[129][1] = 13 - 1;
    walls[129][2] = 10 - 1;
    walls[129][3] = 14 - 1;
    walls[130][0] = 11 - 1;
    walls[130][1] = 13 - 1;
    walls[130][2] = 11 - 1;
    walls[130][3] = 14 - 1;
    walls[131][0] = 12 - 1;
    walls[131][1] = 13 - 1;
    walls[131][2] = 12 - 1;
    walls[131][3] = 14 - 1;
    walls[132][0] = 13 - 1;
    walls[132][1] = 13 - 1;
    walls[132][2] = 13 - 1;
    walls[132][3] = 14 - 1;
    walls[133][0] = 14 - 1;
    walls[133][1] = 13 - 1;
    walls[133][2] = 14 - 1;
    walls[133][3] = 14 - 1;
    walls[134][0] = 15 - 1;
    walls[134][1] = 13 - 1;
    walls[134][2] = 15 - 1;
    walls[134][3] = 14 - 1;
    walls[135][0] = 16 - 1;
    walls[135][1] = 13 - 1;
    walls[135][2] = 16 - 1;
    walls[135][3] = 14 - 1;
    walls[136][0] = 17 - 1;
    walls[136][1] = 13 - 1;
    walls[136][2] = 17 - 1;
    walls[136][3] = 14 - 1;
    walls[137][0] = 18 - 1;
    walls[137][1] = 13 - 1;
    walls[137][2] = 18 - 1;
    walls[137][3] = 14 - 1;
    walls[138][0] = 19 - 1;
    walls[138][1] = 13 - 1;
    walls[138][2] = 19 - 1;
    walls[138][3] = 14 - 1;
    walls[139][0] = 20 - 1;
    walls[139][1] = 13 - 1;
    walls[139][2] = 20 - 1;
    walls[139][3] = 14 - 1;
    walls[140][0] = 21 - 1;
    walls[140][1] = 13 - 1;
    walls[140][2] = 21 - 1;
    walls[140][3] = 14 - 1;
    walls[141][0] = 22 - 1;
    walls[141][1] = 13 - 1;
    walls[141][2] = 22 - 1;
    walls[141][3] = 14 - 1;
    walls[142][0] = 23 - 1;
    walls[142][1] = 13 - 1;
    walls[142][2] = 23 - 1;
    walls[142][3] = 14 - 1;
    walls[143][0] = 24 - 1;
    walls[143][1] = 13 - 1;
    walls[143][2] = 24 - 1;
    walls[143][3] = 14 - 1;
    walls[144][0] = 25 - 1;
    walls[144][1] = 13 - 1;
    walls[144][2] = 25 - 1;
    walls[144][3] = 14 - 1;
    walls[145][0] = 26 - 1;
    walls[145][1] = 13 - 1;
    walls[145][2] = 26 - 1;
    walls[145][3] = 14 - 1;
    walls[146][0] = 27 - 1;
    walls[146][1] = 13 - 1;
    walls[146][2] = 27 - 1;
    walls[146][3] = 14 - 1;
    walls[147][0] = 28 - 1;
    walls[147][1] = 13 - 1;
    walls[147][2] = 28 - 1;
    walls[147][3] = 14 - 1;
    walls[148][0] = 29 - 1;
    walls[148][1] = 13 - 1;
    walls[148][2] = 29 - 1;
    walls[148][3] = 14 - 1;
    walls[149][0] = 30 - 1;
    walls[149][1] = 13 - 1;
    walls[149][2] = 30 - 1;
    walls[149][3] = 14 - 1;
}

void worldworldArea0()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 3 - 1;
    worldDimension[1] = 3 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
}

void worldworldArea1()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 10 - 1;
    worldDimension[1] = 7 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
}

void worldworldArea3()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 10 - 1;
    worldDimension[1] = 20 - 1;
    karelPosition[0] = 1 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
}

void worldMining3()
{
    hasUlimitedBeeper = false;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 9 - 1;
    worldDimension[1] = 15 - 1;
    karelPosition[0] = 5 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 4 - 1;
    walls[0][1] = 1 - 1;
    walls[0][2] = 5 - 1;
    walls[0][3] = 1 - 1;
    walls[1][0] = 2 - 1;
    walls[1][1] = 1 - 1;
    walls[1][2] = 2 - 1;
    walls[1][3] = 2 - 1;
    walls[2][0] = 1 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 2 - 1;
    walls[2][3] = 2 - 1;
    walls[3][0] = 3 - 1;
    walls[3][1] = 1 - 1;
    walls[3][2] = 3 - 1;
    walls[3][3] = 2 - 1;
    walls[4][0] = 4 - 1;
    walls[4][1] = 1 - 1;
    walls[4][2] = 4 - 1;
    walls[4][3] = 2 - 1;
    walls[5][0] = 1 - 1;
    walls[5][1] = 3 - 1;
    walls[5][2] = 2 - 1;
    walls[5][3] = 3 - 1;
    walls[6][0] = 3 - 1;
    walls[6][1] = 2 - 1;
    walls[6][2] = 3 - 1;
    walls[6][3] = 3 - 1;
    walls[7][0] = 2 - 1;
    walls[7][1] = 3 - 1;
    walls[7][2] = 3 - 1;
    walls[7][3] = 3 - 1;
    walls[8][0] = 4 - 1;
    walls[8][1] = 2 - 1;
    walls[8][2] = 4 - 1;
    walls[8][3] = 3 - 1;
    walls[9][0] = 4 - 1;
    walls[9][1] = 3 - 1;
    walls[9][2] = 5 - 1;
    walls[9][3] = 3 - 1;
    walls[10][0] = 2 - 1;
    walls[10][1] = 3 - 1;
    walls[10][2] = 2 - 1;
    walls[10][3] = 4 - 1;
    walls[11][0] = 4 - 1;
    walls[11][1] = 4 - 1;
    walls[11][2] = 5 - 1;
    walls[11][3] = 4 - 1;
    walls[12][0] = 3 - 1;
    walls[12][1] = 4 - 1;
    walls[12][2] = 3 - 1;
    walls[12][3] = 5 - 1;
    walls[13][0] = 2 - 1;
    walls[13][1] = 5 - 1;
    walls[13][2] = 3 - 1;
    walls[13][3] = 5 - 1;
    walls[14][0] = 3 - 1;
    walls[14][1] = 5 - 1;
    walls[14][2] = 4 - 1;
    walls[14][3] = 5 - 1;
    walls[15][0] = 4 - 1;
    walls[15][1] = 5 - 1;
    walls[15][2] = 5 - 1;
    walls[15][3] = 5 - 1;
    walls[16][0] = 2 - 1;
    walls[16][1] = 6 - 1;
    walls[16][2] = 3 - 1;
    walls[16][3] = 6 - 1;
    walls[17][0] = 4 - 1;
    walls[17][1] = 5 - 1;
    walls[17][2] = 4 - 1;
    walls[17][3] = 6 - 1;
    walls[18][0] = 2 - 1;
    walls[18][1] = 7 - 1;
    walls[18][2] = 3 - 1;
    walls[18][3] = 7 - 1;
    walls[19][0] = 4 - 1;
    walls[19][1] = 6 - 1;
    walls[19][2] = 4 - 1;
    walls[19][3] = 7 - 1;
    walls[20][0] = 3 - 1;
    walls[20][1] = 7 - 1;
    walls[20][2] = 4 - 1;
    walls[20][3] = 7 - 1;
    walls[21][0] = 4 - 1;
    walls[21][1] = 7 - 1;
    walls[21][2] = 5 - 1;
    walls[21][3] = 7 - 1;
    walls[22][0] = 3 - 1;
    walls[22][1] = 7 - 1;
    walls[22][2] = 3 - 1;
    walls[22][3] = 8 - 1;
    walls[23][0] = 4 - 1;
    walls[23][1] = 8 - 1;
    walls[23][2] = 5 - 1;
    walls[23][3] = 8 - 1;
    walls[24][0] = 4 - 1;
    walls[24][1] = 9 - 1;
    walls[24][2] = 5 - 1;
    walls[24][3] = 9 - 1;
    walls[25][0] = 2 - 1;
    walls[25][1] = 9 - 1;
    walls[25][2] = 2 - 1;
    walls[25][3] = 10 - 1;
    walls[26][0] = 1 - 1;
    walls[26][1] = 10 - 1;
    walls[26][2] = 2 - 1;
    walls[26][3] = 10 - 1;
    walls[27][0] = 2 - 1;
    walls[27][1] = 10 - 1;
    walls[27][2] = 3 - 1;
    walls[27][3] = 10 - 1;
    walls[28][0] = 4 - 1;
    walls[28][1] = 10 - 1;
    walls[28][2] = 5 - 1;
    walls[28][3] = 10 - 1;
    walls[29][0] = 1 - 1;
    walls[29][1] = 11 - 1;
    walls[29][2] = 2 - 1;
    walls[29][3] = 11 - 1;
    walls[30][0] = 3 - 1;
    walls[30][1] = 10 - 1;
    walls[30][2] = 3 - 1;
    walls[30][3] = 11 - 1;
    walls[31][0] = 4 - 1;
    walls[31][1] = 10 - 1;
    walls[31][2] = 4 - 1;
    walls[31][3] = 11 - 1;
    walls[32][0] = 1 - 1;
    walls[32][1] = 12 - 1;
    walls[32][2] = 2 - 1;
    walls[32][3] = 12 - 1;
    walls[33][0] = 3 - 1;
    walls[33][1] = 11 - 1;
    walls[33][2] = 3 - 1;
    walls[33][3] = 12 - 1;
    walls[34][0] = 2 - 1;
    walls[34][1] = 12 - 1;
    walls[34][2] = 3 - 1;
    walls[34][3] = 12 - 1;
    walls[35][0] = 4 - 1;
    walls[35][1] = 11 - 1;
    walls[35][2] = 4 - 1;
    walls[35][3] = 12 - 1;
    walls[36][0] = 4 - 1;
    walls[36][1] = 12 - 1;
    walls[36][2] = 5 - 1;
    walls[36][3] = 12 - 1;
    walls[37][0] = 2 - 1;
    walls[37][1] = 12 - 1;
    walls[37][2] = 2 - 1;
    walls[37][3] = 13 - 1;
    walls[38][0] = 4 - 1;
    walls[38][1] = 13 - 1;
    walls[38][2] = 5 - 1;
    walls[38][3] = 13 - 1;
    walls[39][0] = 4 - 1;
    walls[39][1] = 14 - 1;
    walls[39][2] = 5 - 1;
    walls[39][3] = 14 - 1;
    walls[40][0] = 4 - 1;
    walls[40][1] = 15 - 1;
    walls[40][2] = 5 - 1;
    walls[40][3] = 15 - 1;
    beeper[0][0] = 5 - 1;
    beeper[0][1] = 15 - 1;
    beeper[0][2] = 1;
}

void worldMining3Ex()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);
    worldDimension[0] = 9 - 1;
    worldDimension[1] = 15 - 1;
    karelPosition[0] = 5 - 1;
    karelPosition[1] = 1 - 1;
    karelRotation = 1;
    walls[0][0] = 4 - 1;
    walls[0][1] = 1 - 1;
    walls[0][2] = 5 - 1;
    walls[0][3] = 1 - 1;
    walls[1][0] = 2 - 1;
    walls[1][1] = 1 - 1;
    walls[1][2] = 2 - 1;
    walls[1][3] = 2 - 1;
    walls[2][0] = 1 - 1;
    walls[2][1] = 2 - 1;
    walls[2][2] = 2 - 1;
    walls[2][3] = 2 - 1;
    walls[3][0] = 3 - 1;
    walls[3][1] = 1 - 1;
    walls[3][2] = 3 - 1;
    walls[3][3] = 2 - 1;
    walls[4][0] = 4 - 1;
    walls[4][1] = 1 - 1;
    walls[4][2] = 4 - 1;
    walls[4][3] = 2 - 1;
    walls[5][0] = 1 - 1;
    walls[5][1] = 3 - 1;
    walls[5][2] = 2 - 1;
    walls[5][3] = 3 - 1;
    walls[6][0] = 3 - 1;
    walls[6][1] = 2 - 1;
    walls[6][2] = 3 - 1;
    walls[6][3] = 3 - 1;
    walls[7][0] = 2 - 1;
    walls[7][1] = 3 - 1;
    walls[7][2] = 3 - 1;
    walls[7][3] = 3 - 1;
    walls[8][0] = 4 - 1;
    walls[8][1] = 2 - 1;
    walls[8][2] = 4 - 1;
    walls[8][3] = 3 - 1;
    walls[9][0] = 4 - 1;
    walls[9][1] = 3 - 1;
    walls[9][2] = 5 - 1;
    walls[9][3] = 3 - 1;
    walls[10][0] = 2 - 1;
    walls[10][1] = 3 - 1;
    walls[10][2] = 2 - 1;
    walls[10][3] = 4 - 1;
    walls[11][0] = 4 - 1;
    walls[11][1] = 4 - 1;
    walls[11][2] = 5 - 1;
    walls[11][3] = 4 - 1;
    walls[12][0] = 3 - 1;
    walls[12][1] = 4 - 1;
    walls[12][2] = 3 - 1;
    walls[12][3] = 5 - 1;
    walls[13][0] = 2 - 1;
    walls[13][1] = 5 - 1;
    walls[13][2] = 3 - 1;
    walls[13][3] = 5 - 1;
    walls[14][0] = 3 - 1;
    walls[14][1] = 5 - 1;
    walls[14][2] = 4 - 1;
    walls[14][3] = 5 - 1;
    walls[15][0] = 4 - 1;
    walls[15][1] = 5 - 1;
    walls[15][2] = 5 - 1;
    walls[15][3] = 5 - 1;
    walls[16][0] = 2 - 1;
    walls[16][1] = 6 - 1;
    walls[16][2] = 3 - 1;
    walls[16][3] = 6 - 1;
    walls[17][0] = 4 - 1;
    walls[17][1] = 5 - 1;
    walls[17][2] = 4 - 1;
    walls[17][3] = 6 - 1;
    walls[18][0] = 2 - 1;
    walls[18][1] = 7 - 1;
    walls[18][2] = 3 - 1;
    walls[18][3] = 7 - 1;
    walls[19][0] = 4 - 1;
    walls[19][1] = 6 - 1;
    walls[19][2] = 4 - 1;
    walls[19][3] = 7 - 1;
    walls[20][0] = 3 - 1;
    walls[20][1] = 7 - 1;
    walls[20][2] = 4 - 1;
    walls[20][3] = 7 - 1;
    walls[21][0] = 4 - 1;
    walls[21][1] = 7 - 1;
    walls[21][2] = 5 - 1;
    walls[21][3] = 7 - 1;
    walls[22][0] = 3 - 1;
    walls[22][1] = 7 - 1;
    walls[22][2] = 3 - 1;
    walls[22][3] = 8 - 1;
    walls[23][0] = 4 - 1;
    walls[23][1] = 8 - 1;
    walls[23][2] = 5 - 1;
    walls[23][3] = 8 - 1;
    walls[24][0] = 4 - 1;
    walls[24][1] = 9 - 1;
    walls[24][2] = 5 - 1;
    walls[24][3] = 9 - 1;
    walls[25][0] = 2 - 1;
    walls[25][1] = 9 - 1;
    walls[25][2] = 2 - 1;
    walls[25][3] = 10 - 1;
    walls[26][0] = 1 - 1;
    walls[26][1] = 10 - 1;
    walls[26][2] = 2 - 1;
    walls[26][3] = 10 - 1;
    walls[27][0] = 2 - 1;
    walls[27][1] = 10 - 1;
    walls[27][2] = 3 - 1;
    walls[27][3] = 10 - 1;
    walls[28][0] = 4 - 1;
    walls[28][1] = 10 - 1;
    walls[28][2] = 5 - 1;
    walls[28][3] = 10 - 1;
    walls[29][0] = 1 - 1;
    walls[29][1] = 11 - 1;
    walls[29][2] = 2 - 1;
    walls[29][3] = 11 - 1;
    walls[30][0] = 3 - 1;
    walls[30][1] = 10 - 1;
    walls[30][2] = 3 - 1;
    walls[30][3] = 11 - 1;
    walls[31][0] = 4 - 1;
    walls[31][1] = 10 - 1;
    walls[31][2] = 4 - 1;
    walls[31][3] = 11 - 1;
    walls[32][0] = 1 - 1;
    walls[32][1] = 12 - 1;
    walls[32][2] = 2 - 1;
    walls[32][3] = 12 - 1;
    walls[33][0] = 3 - 1;
    walls[33][1] = 11 - 1;
    walls[33][2] = 3 - 1;
    walls[33][3] = 12 - 1;
    walls[34][0] = 2 - 1;
    walls[34][1] = 12 - 1;
    walls[34][2] = 3 - 1;
    walls[34][3] = 12 - 1;
    walls[35][0] = 4 - 1;
    walls[35][1] = 11 - 1;
    walls[35][2] = 4 - 1;
    walls[35][3] = 12 - 1;
    walls[36][0] = 4 - 1;
    walls[36][1] = 12 - 1;
    walls[36][2] = 5 - 1;
    walls[36][3] = 12 - 1;
    walls[37][0] = 2 - 1;
    walls[37][1] = 13 - 1;
    walls[37][2] = 2 - 1;
    walls[37][3] = 14 - 1;
    walls[38][0] = 3 - 1;
    walls[38][1] = 13 - 1;
    walls[38][2] = 3 - 1;
    walls[38][3] = 14 - 1;
    walls[39][0] = 3 - 1;
    walls[39][1] = 12 - 1;
    walls[39][2] = 3 - 1;
    walls[39][3] = 13 - 1;
    walls[40][0] = 3 - 1;
    walls[40][1] = 13 - 1;
    walls[40][2] = 4 - 1;
    walls[40][3] = 13 - 1;
    walls[41][0] = 1 - 1;
    walls[41][1] = 13 - 1;
    walls[41][2] = 2 - 1;
    walls[41][3] = 13 - 1;
    walls[42][0] = 4 - 1;
    walls[42][1] = 13 - 1;
    walls[42][2] = 5 - 1;
    walls[42][3] = 13 - 1;
    walls[43][0] = 4 - 1;
    walls[43][1] = 14 - 1;
    walls[43][2] = 5 - 1;
    walls[43][3] = 14 - 1;
    walls[44][0] = 4 - 1;
    walls[44][1] = 15 - 1;
    walls[44][2] = 5 - 1;
    walls[44][3] = 15 - 1;
    beeper[0][0] = 5 - 1;
    beeper[0][1] = 15 - 1;
    beeper[0][2] = 1;
}

// YOUR OWN CUSTOM WORLDS:
void world3x1()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(500);             // speed in ms
    worldDimension[0] = 1 - 1; // set to value in file
    worldDimension[1] = 3 - 1; // set to value in file
    karelPosition[0] = 1 - 1;  // set to value in file
    karelPosition[1] = 1 - 1;  // set to value in file
    karelRotation = 1;
    beeper[0][0] = 1 - 1;
    beeper[0][1] = 2 - 1;
    beeper[0][2] = 20;
}

#endif