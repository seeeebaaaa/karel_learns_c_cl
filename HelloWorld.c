#include "karel.h"

void setup()
{
    setSpeed(500); // Speed between actions in milliseconds
    loadWorld("LivingRoom");
}

void turnRight()
{
    turnLeft();
    turnLeft();
    turnLeft();
}

void run()
{
    while (frontIsClear())
    {

        move();
    }
    // move();
    turnLeft();
    while (rightIsBlocked())
    {
        move();
    }
    turnRight();

    while (frontIsClear())
    {
        while (noBeepersPresent())
        {
            move();
        }
        pickBeeper();
    }
    // karelPosition[0] = 3;
    // karelPosition[1] = 8;
    // move();
    // move();
}

/** TODO!!
 *
 * LOAD WOLRDS FROM FILE
 *
 */