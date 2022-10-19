#include "../../karel.h"

/**
 * ====
 * TASK
 * ====
 *
 * Repair all Pillars and stop at x,1 at the right side. The pillars will always have a distance by 3 from each other
 */

void setup()
{
    loadWorld("DamagedPillar2");
    // ^^ also works for DamagedPillar2
    setSpeed(100);
}

// Karel turns around
void turnAround()
{
    turnLeft();
    turnLeft();
}

/**
 * Pre: Karel stands at foot of pillar facing north
 * Post: Karel stands at top of pillar facing north; pillar is repaired
 */
void moveAndRepair()
{
    while (frontIsClear())
    {
        if (noBeepersPresent())
        {
            putBeeper();
        }
        move();
    }
    if (noBeepersPresent())
    {
        putBeeper();
    }
}

/**
 * Pre: Karel stands at top of pillar facing south
 * Post: Karel is standing at foot of pillar facing south
 */
void moveDown()
{
    while (frontIsClear())
    {
        move();
    }
}

/**
 * Pre: Karel is standing at foot of pillar facing east
 * Post: Karel is standing at foot of next pillar facing east
 */
void moveToNextPillar()
{
    for (int i = 0; i < 4; i++)
    {
        move();
    }
}

/**
 * Pre: Karel stands at foot of pillar facing east
 * Post: Karel stands at foot of repaired pillar facing east
 */
void repairPillar()
{
    turnLeft();
    moveAndRepair();
    turnAround();
    moveDown();
    turnLeft();
}

/**
 * Pre: Karel stands at 1,1 facing east; all pillars still ahve to be repaired
 * Post: Karel stands at the right side of the world facing east; all pillars are repaired
 */
void repairAllPillars()
{
    while (frontIsClear())
    {
        repairPillar();
        moveToNextPillar();
    }
    repairPillar(); // the last pillar
}

void run()
{
    repairAllPillars();
}