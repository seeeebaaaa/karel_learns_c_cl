#include "../../karel.h"

/**
 * ====
 * TASK
 * ====
 *
 * Measure the distance to the flag by placing the according amount of beepers at 1,1 and move afterwards to show the amount.
 */

// recursive way

void setup()
{
    loadWorld("flagDistance1");
    // ^^ also works for flagDistance0 and flagDistance2
    setSpeed(100);
}

// Karel truns around
void turnAround()
{
    turnLeft();
    turnLeft();
}

/** Recursive function
 * Pre: Karel stands at 1,1 facing east
 * Post: Karel stands at 1,1 facing east, standing ontop of a pile of beepers corresponding to the distance to the flagpole
 */
void measure()
{
    if (frontIsClear())
    {
        move();
        measure();
    }
    turnAround();
    while (frontIsClear())
    {
        move();
    }
    putBeeper();
    turnAround();
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

void run()
{
    measure();
    pickBeeper();
    showBeeper();
}