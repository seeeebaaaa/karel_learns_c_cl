#include "../../karel.h"

/**
 * ====
 * TASK
 * ====
 *
 * Measure the distance to the flag by placing the according amount of beepers at 1,1 and move afterwards to show the amount.
 */

// non recursive solution

void setup()
{
    loadWorld("flagDistance1");
    // ^^also works for flagDistance0 and flagDistance2
    setSpeed(20);
}

// karel turns right
void turnRight()
{
    turnLeft();
    turnLeft();
    turnLeft();
}

/**
 * Pre: Karel stands at 1,1 facing east on top of the beeper pile
 * Post: Karel stands at 1,2 facing east
 */
void showBeeper()
{
    turnLeft();
    move();
    turnRight();
}

// karel turns around
void turnAround()
{
    turnLeft();
    turnLeft();
}

/**
 * Pre: Karel stands at 1,1 facing east
 * Post: Karels tands infront of the flagpole, behind him aline of beeper, facing east
 */
void placeBeeperMeasure()
{
    while (frontIsClear())
    {
        putBeeper();
        turnAround();
        while (frontIsClear())
        {
            move();
        }
        putBeeper();
        turnAround();
        while (beepersPresent())
        {
            move();
        }
    }
}

/**
 * Pre: Karel stands in front of teh falgpole facing west
 * Post: Karel stands at 1,1 on a pile of beepers
 */
void clearBeeper()
{
    while (frontIsClear())
    {
        pickBeeper();
        move();
    }
    pickBeeper();
}

/**
 * Pre: karel is at 1,1 facing east
 * Post: karel is at 1,1 facing east; underneath him a pile of beepers
 */
void measure()
{
    placeBeeperMeasure();
    turnAround();
    clearBeeper();
    turnRight();
    turnRight();
}

void run()
{
    measure();
    showBeeper();
}