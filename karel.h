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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

// extern int meineVariable;

// extern int meineFunktion1(int);
// extern int meineFunktion2(char);

// extern all def
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
                                        * 4 x..
                                        * 3 x x..
                                        * 2 x x x..
                                        * 1 x x x x..
                                        * 0 x x x x x..
                                        J>^0 1 2 3 4
                                        * I
                                        */

int karelPosition[2] = {0, 0}; // entspricht {Y, X}
int karelRotation = 0;         // 0=north, 1=east, 2=south, 3=west
int walls[maxWalls][4];        // array of walls ; field1|field2 ==> [[field1Y,field1X,field1Y,field1X]
int beeper[maxBeeps][3];       // stores Y,X,NUMBER
int numberOfBeeper = 0;
bool hasUlimitedBeeper = false;

// ✨ Beauty ✨

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
    printf("Karel is now on: [%d;%d]\n", karelPosition[0], karelPosition[1]);
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
            printf("|");
        }
        else
        {
            printf(" ");
        }
    }
    else
    {
        printf("|");
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
            printf("- ");
        }
        else
        {
            printf("-");
        }
    }
    else
    {
        if (j != worldDimension[1])
        {
            printf("  ");
        }
        else
        {
            printf(" ");
        }
    }
}

bool DEBUG_Draw = true;

void draw()
{
    printf("\e[2;1H\e[2J");
    printf("   ");
    for (int i = 0; i <= worldDimension[1]; i++)
    {
        printf("--");
    }
    printf("\n");
    for (int i = worldDimension[0]; i >= 0; i--)
    {
        // printf("%d |", i + 1);

        printf("> |");

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
                    printf("%c", beeperSymbol);
                }
                else
                {
                    printf("%d", beeper[beepPos][2]);
                }
                placeWallVertical(i, j);
                continue;
            }
            // place karel
            if (karelPosition[0] == i && karelPosition[1] == j && isBeeperInBeepDex(beeper, atmBeep) == 0)
            {
                printf("%c", karelFacesWithoutBeeper[karelRotation]);
                placeWallVertical(i, j);
                continue;
            }
            if (karelPosition[0] == i && karelPosition[1] == j && isBeeperInBeepDex(beeper, atmBeep) == 1)
            {
                printf("%c", karelFacesWithBeeper[karelRotation]);
                placeWallVertical(i, j);
                continue;
            }
            printf("%c", clearSymbol);
            placeWallVertical(i, j);
        }
        printf("\n");
        if (i != 0)
        {
            printf("  |");
            for (int j = 0; j <= worldDimension[1]; j++)
            {
                placeWallHorizontal(i - 1, j);
            }
            printf("|\n");
        }
    }
    printf("   ");
    for (int i = 0; i <= worldDimension[1]; i++)
    {
        printf("--");
    }
    printf("\n   ");
    for (int i = 0; i <= worldDimension[1]; i++)
    {
        // printf("%d ", i + 1);
        printf("^ ");
    }
    printf("\n");
    if (hasUlimitedBeeper)
    {
        printf("Beepers in Bag: infinite\n");
    }
    else
    {
        printf("Beepers in Bag: %d\n", numberOfBeeper);
    }
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
    printf("loedade teh wolrd %s \n", worldName);
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
            beeper[i][2]++;
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
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0] - 1, karelPosition[1]};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            return false;
        }
        return true;
    }
    case 3:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0], karelPosition[1] - 1};
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
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0], karelPosition[1] - 1};
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
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0] - 1, karelPosition[1] - 1};
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
    switch (karelRotation)
    {
    case 0:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0], karelPosition[1] + 1};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            return false;
        }
        return true;
    }
    case 1:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0] - 1, karelPosition[1] + 1};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            return false;
        }
        return true;
    }
    case 2:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0], karelPosition[1] - 1};
        if (isWallInWallDex(walls, theWallInQuestion))
        {
            return false;
        }
        return true;
    }
    case 3:
    {
        int theWallInQuestion[4] = {karelPosition[0], karelPosition[1], karelPosition[0] + 1, karelPosition[1]};
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
    printf("finsihed");
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

void worldLivingRoom()
{
    hasUlimitedBeeper = true;
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

void worldFlagDistance1()
{
    hasUlimitedBeeper = true;
    numberOfBeeper = 0;
    setSpeed(100); // speed in ms

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
#endif
