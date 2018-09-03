#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//////////
//ACTOR//
////////
Actor::Actor(StudentWorld* world, int startX, int startY, int imageID, int depth, Direction startDir)
:GraphObject(imageID, startX, startY, startDir, depth, 0.25)
{
    m_isDead = false;
    m_isPebble = false;
    m_isFood = false;
    m_isPheromone = false;
    m_isAntHill = false;
    m_isAdultG = false;
    m_isAnt = false;
    m_isBabyG = false;
    m_colonyNum = -1;
    m_sw = world;
    m_isFirstTime = true;
    m_isPoison = false;
    m_isWater = false;
}

void Actor::getStunned()
{
}

void Actor::getPoisoned()
{
}

Actor::~Actor()
{
}

Pebble::Pebble(StudentWorld* sw, int startX, int startY)
:Actor(sw, startX, startY, IID_ROCK, 1)
{
    setPebble();
}

void Pebble::doSomething()
{
}

/////////////////
//POOLOFWATER///
///////////////
Water::Water(StudentWorld* sw, int startX, int startY)
:Actor(sw, startX, startY, IID_WATER_POOL, 2)
{
    setWater();
}

void Water::doSomething()
{
}

///////////
//POISON//
/////////
Poison::Poison(StudentWorld* sw, int startX, int startY)
:Actor(sw, startX, startY, IID_POISON, 2)
{
    setPoison();
}

void Poison::doSomething()
{
}

/////////////////
//ENERGYHOLDER//
////////////////
EnergyHolder::EnergyHolder(StudentWorld* sw, int startX, int startY, int imageID, int depth)
:Actor(sw, startX, startY, imageID, depth)
{
    m_hp = 0;
}

void EnergyHolder::doSomething()
{
}

/////////
//FOOD//
///////
Food::Food(StudentWorld* sw, int startX, int startY, int startHP)
:EnergyHolder(sw, startX, startY, IID_FOOD, 2)
{
    addHP(startHP);
    setFood();
}


void Food::doSomething()
{
    if(getHP() <= 0)
    {
        setDead();
        return;
    }
}

///////////////
//PHEROMONE///
/////////////
Pheromone::Pheromone(StudentWorld* sw, int startX, int startY, int imageID, int colonyNum)
:EnergyHolder(sw, startX, startY, IID_FOOD, 2)
{
    addHP(256);
    setPheromone();
    setColonyNum(colonyNum);
}

void Pheromone::doSomething()
{
    reduceHP(1);
    if(getHP() <= 0)
    {
        setDead();
        return;
    }
    return;
}

////////////
//ANTHILL//
//////////
Anthill::Anthill(StudentWorld* sw,  int startX, int startY, int colonyNum, Compiler* cmplrptr)
:EnergyHolder(sw, startX, startY, IID_ANT_HILL, 2)
{
    setAntHill();
    setColonyNum(colonyNum);
    setCompiler(cmplrptr);
    addHP(8999);
    
}

void Anthill::doSomething()
{
    reduceHP(1);
    if(getHP() <= 0)
    {
        setDead();
        return;
    }
    if(getWorld()->isFoodHere(getX(), getY()))
    {
        addHP(getWorld()->EatFood(getX(), getY(), 10000));
        return;
    }
    if(getHP() >= 2000)
    {
        while(getHP() >= 2000)
        {
            getWorld()->dealWithNewAnt(getX(), getY(), getColonyNum(), getCompiler());
            reduceHP(1500);
        }
    }
}

///////////
//INSECT//
/////////
Insect::Insect(StudentWorld* sw, int startX, int startY, int imageID)
:EnergyHolder(sw, startX, startY, imageID, 1)
{
    m_break = 0;
    m_isBlocked = false;
}

void Insect::doSomething()
{
}

void Insect::getXYInFrontOfMe(int &x, int &y) const
{
    Direction d = getDirection();
    switch (d)
    {
        case up:
            x = getX();
            y = getY()+1;
            break;
        case down:
            x = getX();
            y = getY()-1;
            break;
        case right:
            x = getX()+1;
            y = getY();
            break;
        case left:
            x = getX()-1;
            y = getY();
            break;
        case none:
            break;
    }
}

bool Insect::moveForwardIfPossible()
{
    int newX = 0;
    int newY = 0;
    //get the new x and y
    getXYInFrontOfMe(newX, newY);
    //if that is a rock
    if(!getWorld()->canMoveTo(newX, newY))
    {
        m_isBlocked = true;
        return false;
    }
    moveTo(newX, newY);
    resetFirstTime();
    return true;
}

void Insect::rndDirection()
{
    Direction direct;
    int rnd = randInt(0, 3);
    switch(rnd)
    {
        case 0:
            direct = up;
            break;
        case 1:
            direct = down;
            break;
        case 2:
            direct = left;
            break;
        case 3:
            direct = right;
            break;
            
    }
    setDirection(direct);
}

void Insect::getStunned()
{
    m_break += 2;
    setFirstTime();
}

void Insect::getPoisoned()
{
    reduceHP(150);
    setFirstTime();
}


////////
//ANT//
//////
Ant::Ant(StudentWorld* sw, int startX, int startY, int imageID, int colonyNum, Compiler* ptr)
:Insect(sw, startX, startY, imageID)
{
    setAnt();
    addHP(1500);
    rndDirection();
    m_food = 0;
    m_wasBit = false;
    m_lastRandomValue = 0;
    m_ic = 0;
    setCompiler(ptr);
    setColonyNum(colonyNum);
}

void Ant::doSomething()
{
    //reduce HP
    reduceHP(1);
    if(getHP() <= 0)
    {
        setDead();
        return;
    }
    //take a break if stunned
    if(getBreak() > 0)
    {
        takeBreak();
        return;
    }
    
    //unBlock
    if(isBlocked())
        unBlock();
    Compiler* c = getCompiler();
    Compiler::Command cmd;
    int cmdCount = 0;
    //interpret commands
    for(;cmdCount <= 10;)
    {
        if(!c->getCommand(m_ic, cmd))
        {
            setDead();
            return;
        }
       switch (cmd.opcode) {
            case Compiler::moveForward:
                if(!isFirstTime())
                    resetFirstTime();
                ++m_ic;
                if(!isBlocked())
                {
                    if(moveForwardIfPossible())
                        m_wasBit = false;
                }
                cmdCount++;
                return;
                break;
            case Compiler::eatFood:
                ++m_ic;
                if(m_food >= 100)
                {
                    addHP(100);
                    m_food -= 100;
                }
                else
                {
                    addHP(m_food);
                    m_food = 0;
                }
                cmdCount++;
                return;
                break;
            case Compiler::dropFood:
                ++m_ic;
                getWorld()->dealWithDropFood(getX(), getY(), m_food);
                m_food = 0;
                cmdCount++;
                return;
                break;
            case Compiler::bite:
                ++m_ic;
                getWorld()->dealWithAntAttack(getX(), getY(), getColonyNum());
                cmdCount++;
                return;
                break;
            case Compiler::pickupFood:
                ++m_ic;
                m_food += getWorld()->dealWithPickupFood(getX(), getY(), m_food);
                cmdCount++;
                return;
                break;
            case Compiler::emitPheromone:
                ++m_ic;
                getWorld()->dealWithPheromone(getX(), getY(), getColonyNum());
                cmdCount++;
                return;
                break;
            case Compiler::faceRandomDirection:
                ++m_ic;
                rndDirection();
                cmdCount++;
                return;
            case Compiler::rotateClockwise:
                ++m_ic;
                rotateClockwise();
                cmdCount++;
                return;
                break;
            case Compiler::rotateCounterClockwise:
                ++m_ic;
                rotateCounterClockwise();
                cmdCount++;
                return;
            case Compiler::generateRandomNumber:
                ++m_ic;
                m_lastRandomValue = randInt(0, stoi(cmd.operand1)-1);
                cmdCount++;
            case Compiler::goto_command:
                m_ic = stoi(cmd.operand1);
                cmdCount++;
                break;
            case Compiler::if_command:
                if(ifCommands(cmd.operand1))
                    m_ic = stoi(cmd.operand2);
                else
                    ++m_ic;
                cmdCount++;
                break;
            default:
                break;
        }
    }
}

void Ant::rotateClockwise()
{
    Direction d = getDirection();
    switch(d)
    {
        case up:
            setDirection(right);
            break;
        case right:
            setDirection(down);
            break;
        case down:
            setDirection(left);
            break;
        case left:
            setDirection(up);
            break;
        case none:
        default:
            break;
    }
}

void Ant::rotateCounterClockwise()
{
    Direction d = getDirection();
    switch(d)
    {
        case up:
            setDirection(left);
            break;
        case left:
            setDirection(down);
            break;
        case down:
            setDirection(right);
            break;
        case right:
            setDirection(up);
            break;
        case none:
        default:
            break;
    }
}

bool Ant::ifCommands(std::string cond)
{
    //I smell danger
    if(cond == "0")
    {
        int checkX;
        int checkY;
        getXYInFrontOfMe(checkX, checkY);
        return getWorld()->isEnemyHere(checkX, checkY, getColonyNum());
    }
    //i smell pheromone
    else if(cond == "1")
    {
        int checkX;
        int checkY;
        getXYInFrontOfMe(checkX, checkY);
        return getWorld()->isPheromoneHere(checkX, checkY, getColonyNum());
    }
    //i was bit
    else if(cond == "2")
        return m_wasBit;
    //i have food
    else if(cond == "3")
        return m_food > 0;
    //i am hungry
    else if(cond == "4")
        return getHP() <= 25;
    //i am on my anthill
    else if(cond == "5")
        return getWorld()->isAnthillHere(getX(), getY(), getColonyNum());
    //i am stading on food
    else if(cond == "6")
        return getWorld()->isFoodHere(getX(), getY());
    //i am standing on an enemy
    else if(cond == "7")
        return getWorld()->isEnemyHere(getX(), getY(), getColonyNum());
    //i am blocked
    else if (cond == "8")
        return isBlocked();
    //the last random value was 0
    if(cond == "9")
        return m_lastRandomValue == 0;
    return false;
    
}

////////////////
//GRASSHOPPER//
//////////////
Grasshopper::Grasshopper(StudentWorld* sw, int startX, int startY, int imageID, int startHP)
:Insect(sw, startX, startY, imageID)
{
    addHP(startHP);
    rndDirection();
    m_walk = randInt(2, 10);
}

void Grasshopper::doSomething()
{
    //reduce hp
    reduceHP(1);
    //check if dead
    if(getHP() <= 0)
    {
        setDead();
        return;
    }
    //take or set break
    if(getBreak() == 0)
    {
        setBreak(2);
    }
    else
    {
        takeBreak();
        return;
    }
    
    //change to adult grashopper if it has enough HP
    if(isBabyG() && getHP() >= 1600)
    {
        setDead();
        getWorld()->addActor(new AdultGrasshopper(getWorld(), getX(), getY()), getX(), getY());
        return;
    }
    
    if(isAdultG())
    {
        //attack if there are enemies
        if(randInt(1, 3) == 1)
        {
            if(getWorld()->dealWithGAttack(this))
            {
                setBreak(2);
                return;
            }
        }
        //10% chance of jumping
        if(randInt(1, 10) == 1)
        {
            if(getWorld()->dealWithGJump(this))
            {
                setBreak(2);
                return;
            }
        }
    }
    
    //Eat if there is food on the square
    if(getWorld()->isFoodHere(getX(), getY()))
    {
        addHP(getWorld()->EatFood(getX(), getY(), 200));
        if(randInt(0, 1) == 0)
        {
            setBreak(2);;
            return;
        }
    }
    
    //if its done walking or blocked, reset walk
    if(isBlocked()|| m_walk == 0)
    {
        rndDirection();
        m_walk = randInt(2, 10);
        unBlock();
        setBreak(2);
        return;
    }
    //otherwise, move forward and decrement the amount it has to walk
    m_walk--;
    moveForwardIfPossible();
    return;
}

////////////////////
//BABYGRASSHOPPER//
//////////////////
BabyGrasshopper::BabyGrasshopper(StudentWorld* sw, int startX, int startY)
:Grasshopper(sw, startX, startY, IID_BABY_GRASSHOPPER, 500)
{
    setBabyG();
}

/////////////////////
//ADULTGRASSHOPPER//
///////////////////
AdultGrasshopper::AdultGrasshopper(StudentWorld* sw, int startX, int startY)
:Grasshopper(sw, startX, startY, IID_ADULT_GRASSHOPPER, 1600)
{
    setAdultG();
}