#include "../../karel.h"

/**
 * ====
 * TASK
 * ====
 *
 * Collect all the beepers in the shafts and stop at the same y on the right side.
 */

// non recursive

void setup()
{
    loadWorld("DiamondMining2");
    // ^^ also works for DiamondMining1
    setSpeed(50);
}

/**
 * Pre: karel stands ontop of mine facing east
 * Post: karel stands over shaft entrance facing east
 */
void moveToNextMine()
{
    move();
}

// karel turns right
void turnRight()
{
    turnLeft();
    turnLeft();
    turnLeft();
}

/**
 * Pre: Karel is standing somewhere
 * Post: Karel moves to a wall in a straight line
 */
void moveToWall()
{
    while (frontIsClear())
    {
        move();
    }
}

// Karel turns around
void turnAround()
{
    turnLeft();
    turnLeft();
}

/**
 * Pre: Karel is in mine
 * Post: Karel is standing in front of a wall; picked up all beeper in his path
 */
void moveAndPickToWall()
{
    while (frontIsClear())
    {
        if (beepersPresent())
        {
            pickBeeper();
        }
        move();
    }
    if (beepersPresent())
    {
        pickBeeper();
    }
}

/**
 * Pre: Karel stands in shaft, left side facing east
 * Post: Karel stands at shaft entrance facing east
 */
void moveToExit()
{
    while (leftIsBlocked())
    {
        move();
    }
    turnLeft();
    move();
    while (rightIsBlocked())
    {
        move();
    }
    turnRight();
    if (frontIsClear())
    {
        move();
    }
}

/**
 * Pre: Karel stands at shaft entrance
 * Psot: Karel stands at shaft exit; shaft's clear
 */
void clearShaft()
{
    turnRight();
    moveToWall();
    turnLeft();
    moveToWall();
    turnAround();
    moveAndPickToWall();
    turnAround();
    moveToExit();
}

/**
 * Pre: Karel stands at the left side facing east
 * Post: Karel stands at the right side facing east
 */
void clearMine()
{
    while (frontIsClear())
    {
        moveToNextMine();
        if (rightIsClear())
        {
            clearShaft();
        }
    }
}

void run()
{
    clearMine();
}