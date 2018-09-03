#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Compiler;
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int startX, int startY, int imageID, int depth, Direction startDir = right);
    virtual void doSomething() = 0;
    virtual bool isDead() const;
    virtual void setDead();
    virtual bool isPebble() const;
    virtual void setPebble();
    virtual void getPoisoned();
    virtual void getStunned();
    virtual bool isFood() const;
    virtual void setFood();
    virtual bool isPheromone() const;
    virtual void setPheromone();
    virtual bool isAntHill() const;
    virtual void setAntHill();
    virtual bool isAdultG() const;
    virtual void setAdultG();
    virtual bool isAnt() const;
    virtual void setAnt();
    virtual bool isBabyG() const;
    virtual void setBabyG();
    virtual int getColonyNum() const;
    virtual void setColonyNum(int cn);
    StudentWorld* getWorld() const;
    virtual bool isFirstTime() const;
    virtual void resetFirstTime();
    virtual void setFirstTime();
    virtual bool isWater() const;
    virtual void setWater();
    virtual bool isPoison() const;
    virtual void setPoison();
    virtual ~Actor();
private:
    bool m_isDead;
    bool m_isPebble;
    bool m_isFood;
    bool m_isPheromone;
    bool m_isAntHill;
    bool m_isAdultG;
    bool m_isAnt;
    bool m_isBabyG;
    int m_colonyNum;
    bool m_isFirstTime;
    bool m_isWater;
    bool m_isPoison;
private:
    StudentWorld* m_sw;
};

/////////////////
//INLINE ACTOR//
///////////////
inline
bool Actor::isDead() const { return m_isDead; }

inline
bool Actor::isPebble() const { return m_isPebble; }

inline
bool Actor::isFood() const { return m_isFood; }

inline
bool Actor::isPheromone() const { return m_isPheromone; }

inline
bool Actor::isAntHill() const { return m_isAntHill; }

inline
bool Actor::isAdultG() const { return m_isAdultG; }

inline
bool Actor::isAnt() const { return m_isAnt; }

inline
bool Actor::isBabyG() const { return m_isBabyG; }

inline
int Actor::getColonyNum() const { return m_colonyNum; }

inline
StudentWorld* Actor::getWorld() const { return m_sw; }

inline
bool Actor::isFirstTime() const { return m_isFirstTime; }

inline
bool Actor::isWater() const { return m_isWater; }

inline
bool Actor::isPoison() const { return m_isPoison; }

inline
void Actor::setDead() { m_isDead = true; }

inline
void Actor::setPebble() { m_isPebble = true; }

inline
void Actor::setFood() { m_isFood = true; }

inline
void Actor::setPheromone() { m_isPheromone = true; }

inline
void Actor::setAntHill() { m_isAntHill = true; }

inline
void Actor::setAdultG() { m_isAdultG = true; }

inline
void Actor::setAnt(){ m_isAnt = true; }

inline
void Actor::setBabyG(){ m_isBabyG = true; }

inline
void Actor::setColonyNum(int cn) { m_colonyNum = cn; }
    
inline
void Actor::resetFirstTime(){ m_isFirstTime = true; }

inline
void Actor::setFirstTime(){ m_isFirstTime = false; }

inline
void Actor::setWater() { m_isWater = true; }

inline
void Actor::setPoison() { m_isPoison = true; }
    
///////////
//PEBBLE//
/////////
class Pebble : public Actor
{
public:
    Pebble(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
};

/////////////////
//POOLOFWATER///
///////////////
class Water : public Actor
{
public:
    Water(StudentWorld * sw, int startX, int startY);
    virtual void doSomething();
};

///////////
//POISON//
/////////
class Poison : public Actor
{
public:
    Poison(StudentWorld * sw, int startX, int startY);
    virtual void doSomething();
};

/////////////////
//ENERGYHOLDER//
////////////////
class EnergyHolder : public Actor
{
public:
    EnergyHolder(StudentWorld* sw, int startX, int startY, int imageID, int depth);
    virtual void doSomething();
    int getHP();
    void addHP(int addition);
    void reduceHP(int reduction);
    void setCompiler(Compiler* cmplr);
    Compiler* getCompiler();
private:
    int m_hp;
    Compiler* m_compiler;
};

////////////////////////
//INLINE ENERGYHOLDER//
//////////////////////
inline
int EnergyHolder::getHP(){ return m_hp; }

inline
void EnergyHolder::addHP(int addition){ m_hp += addition; }

inline
void EnergyHolder::reduceHP(int reduction){ m_hp -= reduction; }

inline
void EnergyHolder::setCompiler(Compiler *cmplr) { m_compiler = cmplr; }

inline
Compiler* EnergyHolder::getCompiler(){ return m_compiler; }

/////////
//FOOD//
///////
class Food : public EnergyHolder
{
public:
    Food(StudentWorld* sw, int startX, int startY, int startHP = 6000);
    virtual void doSomething();
};

///////////////
//PHEROMONE///
/////////////
class Pheromone : public EnergyHolder
{
public:
    Pheromone(StudentWorld* sw, int startX, int startY, int imageID, int colonyNum);
    virtual void doSomething();
};

////////////
//ANTHILL//
//////////
class Anthill : public EnergyHolder
{
public:
    Anthill(StudentWorld* sw,  int startX, int startY, int colonyNum, Compiler* cmplrptr);
    virtual void doSomething();
};


///////////
//INSECT//
/////////
class Insect : public EnergyHolder
{
public:
    Insect(StudentWorld* sw, int startX, int startY, int imageID);
    virtual void doSomething();
    void rndDirection();
    void getXYInFrontOfMe(int& x, int& y) const;
    virtual bool moveForwardIfPossible();
    virtual void getPoisoned();
    virtual void getStunned();
    void setBreak(int brk);
    int getBreak() const;
    void takeBreak();
    void setBlocked();
    bool isBlocked() const;
    void unBlock();
private:
    int m_break;
    bool m_isBlocked;
};

//////////////////
//INLINE INSECT//
////////////////
inline
void Insect::setBreak(int brk) { m_break = brk; }

inline
int Insect::getBreak() const { return m_break; }

inline
void Insect::takeBreak() { m_break--; }

inline
void Insect::setBlocked() { m_isBlocked = true; }

inline
bool Insect::isBlocked() const { return m_isBlocked; }

inline
void Insect::unBlock() { m_isBlocked = false; }

////////
//ANT//
//////
class Ant : public Insect
{
public:
    Ant(StudentWorld* sw, int startX, int startY, int imageID, int colonyNum, Compiler* ptr);
    virtual void doSomething();
    void getBit();
private:
    void rotateClockwise();
    void rotateCounterClockwise();
    bool ifCommands(std::string cond);
    int m_ic;
    int m_food;
    int m_lastRandomValue;
    bool m_wasBit;
};

inline
void Ant::getBit() { m_wasBit = true; }

////////////////
//GRASSHOPPER//
//////////////
class Grasshopper : public Insect
{
public:
    Grasshopper(StudentWorld* sw, int startX, int startY, int imageID, int startHP);
    virtual void doSomething();
private:
    int m_walk;
};

////////////////////
//BABYGRASSHOPPER//
//////////////////
class BabyGrasshopper : public Grasshopper
{
public:
    BabyGrasshopper(StudentWorld* sw, int startX, int startY);
};

/////////////////////
//ADULTGRASSHOPPER//
///////////////////
class AdultGrasshopper : public Grasshopper
{
public:
    AdultGrasshopper(StudentWorld* sw, int startX, int startY);
};
#endif // ACTOR_H_
