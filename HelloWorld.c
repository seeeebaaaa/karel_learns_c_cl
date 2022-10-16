#include "karel.h"

void setup()
{
    loadWorld("LivingRoom");
    setSpeed(500); // Optional: Speed between actions/frames in milliseconds
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