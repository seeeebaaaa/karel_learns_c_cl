#include "../../karel.h"

/**
 * ====
 * TASK
 * ====
 *
 * Karel needs to walk through all shafts in the mine and stop at the beeper. Use recursive functions and no while/do-while/for loops.
 */

// recursive

void setup()
{
    loadWorld("Mining3");
    // ^^ also works for Mining3Ex
    setSpeed(100);
}

void turnRight()
{
    turnLeft();
    turnLeft();
    turnLeft();
}

void checkMines()
{
    if (noBeepersPresent())
    {
        if (rightIsBlocked() && frontIsClear())
        {
            move();
            checkMines();
        }
        else if (rightIsBlocked() && frontIsBlocked())
        {
            turnLeft();
            checkMines();
        }
        else if (rightIsClear())
        {
            turnRight();
            move();
            checkMines();
        }
    }
}

void run()
{
    checkMines();
}