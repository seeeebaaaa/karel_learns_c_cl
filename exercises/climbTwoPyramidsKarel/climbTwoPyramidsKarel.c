#include "../../karel.h"

/**
 * Task
 * Go to the other side and pick up all beepers
 */

void setup()
{
    loadWorld("ClimbTwoPyramidsKarel");
    setSpeed(100);
}

// karel turns right
void turnRight()
{
    turnLeft();
    turnLeft();
    turnLeft();
}

// karel climbs up one side of the pyramid and picks up beepers
void climbSideUp()
{
    while (frontIsBlocked())
    {
        if (beepersPresent())
        {
            pickBeeper();
        }
        turnLeft();
        move();
        turnRight();
        move();
    }
}

// akrel climbs down one sid eof the pyramid and picks up beepers
void climbSideDown()
{
    while (frontIsClear())
    {

        move();
        if (beepersPresent())
        {
            pickBeeper();
        }
        turnLeft();
        move();
        turnRight();
        // manualMode();
    }
}

// moves till wall
void moveToWall()
{
    while (frontIsClear())
    {
        move();
    }
}

// does the 1. pyramid
void pyramid1()
{
    climbSideUp();
    if (beepersPresent())
    {
        pickBeeper();
    }
    move();
    turnRight();
    climbSideDown();
    turnLeft();
}

// does the 2. pyramid
void pyramid2()
{
    climbSideUp();
    if (beepersPresent())
    {
        pickBeeper();
    }
    move();
    turnRight();
    climbSideDown();
}

void run()
{
    moveToWall();
    pyramid1();
    moveToWall();
    pyramid2();
    turnLeft();
    moveToWall();
}