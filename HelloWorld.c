#include "karel.h"

void setup()
{
    setSpeed(500); // Optional: Speed between actions/frames in milliseconds
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
    while (noBeepersPresent())
    {
        move();
    }
    pickBeeper();

    while (frontIsClear())
    {
        move();
    }
}