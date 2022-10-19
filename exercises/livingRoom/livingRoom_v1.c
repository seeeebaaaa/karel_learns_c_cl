#include "../../karel.h"

// this code will be executed at the start, preparing the world etc.
void setup()
{
    loadWorld("LivingRoom");
}

// Karel turns right
void turnRight()
{
    turnLeft();
    turnLeft();
    turnLeft();
}
/**
 * Pre: Karel stands somewhere
 * Post: Karel stands infront of a wall (straight line to start))
 */
void moveToWall()
{
    while (frontIsClear())
    {
        move();
    }
}

/**
 * Pre: Karel is standing somewhere; there is a beeper in a clear line in teh direction hes facing
 * Post: Karel is standing on top of said beeper
 */
void goToBeeper()
{
    while (noBeepersPresent())
    {
        move();
    }
}

/**
 * Pre: Karel is standing at a wall facing north
 * Post: Karel is standing ontop of said wall, facing north
 */
void climbWall()
{
    while (rightIsBlocked())
    {
        move();
    }
}

// all the cod ein this run() will be executed after the setup()
void run()
{
    moveToWall();
    turnLeft();
    climbWall();
    turnRight();
    goToBeeper();
    pickBeeper();
    moveToWall();
}