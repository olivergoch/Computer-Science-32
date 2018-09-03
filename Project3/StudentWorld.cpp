#include "StudentWorld.h"
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

bool StudentWorld::loadTheField()
{
    Compiler *compilerForEntrant0;
    Compiler *compilerForEntrant1;
    Compiler *compilerForEntrant2;
    Compiler *compilerForEntrant3;
    string error;
    fileNames = getFilenamesOfAntPrograms();
    if(fileNames.size() >= 1)
    {
        compilerForEntrant0 = new Compiler;
        m_compilers.push_back(compilerForEntrant0);
        if(!compilerForEntrant0->compile(fileNames[0], error))
        {
            setError(fileNames[0] + " " + error);
            return GWSTATUS_LEVEL_ERROR;
        }
        fileNames[0] = m_compilers[0]->getColonyName();
    }
    if(fileNames.size() >= 2)
    {
        compilerForEntrant1 = new Compiler;
        m_compilers.push_back(compilerForEntrant1);
        if(!compilerForEntrant1->compile(fileNames[1], error))
        {
            setError(fileNames[1] + " " + error);
            return GWSTATUS_LEVEL_ERROR;
        }
        fileNames[1] = m_compilers[1]->getColonyName();
    }
    
    if(fileNames.size() >= 3)
    {
        compilerForEntrant2 = new Compiler;
        m_compilers.push_back(compilerForEntrant2);
        if(!compilerForEntrant2->compile(fileNames[2], error))
        {
            setError(fileNames[2] + " " + error);
            return GWSTATUS_LEVEL_ERROR;
        }
        fileNames[2] = m_compilers[2]->getColonyName();
    }
    
    if(fileNames.size() == 4)
    {
        compilerForEntrant3 = new Compiler;
        m_compilers.push_back(compilerForEntrant3);
        if(!compilerForEntrant3->compile(fileNames[3], error))
        {
            setError(fileNames[3] + " " + error);
            return GWSTATUS_LEVEL_ERROR;
        }
        fileNames[3] = m_compilers[3]->getColonyName();
    }
    
    Field f;
    string fieldFile = getFieldFilename();
    
    if (f.loadField(fieldFile) != Field::LoadResult::load_success)
        return false;
    int y = 0;
    //add all the actors
    for(; y < VIEW_HEIGHT; y++)
    {
        for(int x = 0; x < VIEW_WIDTH; x++)
        {
            Field::FieldItem item = f.getContentsOf(x,y); // note it’s x,y and not y,x!!!
            if (item == Field::FieldItem::rock)
                m_actors[y][x].push_back(new Pebble(this, x, y));
            
            else if(item == Field::FieldItem::grasshopper)
                m_actors[y][x].push_back(new BabyGrasshopper(this, x, y));
            
            else if(item == Field::FieldItem::food)
                m_actors[y][x].push_back(new Food(this, x, y));
            
            else if(item == Field::FieldItem::water)
                m_actors[y][x].push_back(new Water(this, x, y));
            
            else if(item == Field::FieldItem::poison)
                m_actors[y][x].push_back(new Poison(this, x, y));
            
            else if(fileNames.size() >= 1 && item == Field::FieldItem::anthill0)
                m_actors[y][x].push_back(new Anthill(this, x, y, 0, compilerForEntrant0));
            
            else if(fileNames.size() >= 2 && item == Field::FieldItem::anthill1)
                m_actors[y][x].push_back(new Anthill(this, x, y, 1, compilerForEntrant1));
            
            else if(fileNames.size() >= 3 && item == Field::FieldItem::anthill2)
                m_actors[y][x].push_back(new Anthill(this, x, y, 2, compilerForEntrant2));
            
            else if(fileNames.size() == 4 && item == Field::FieldItem::anthill3)
                m_actors[y][x].push_back(new Anthill(this, x, y, 3, compilerForEntrant3));
             }
    }
    return true;
}

bool StudentWorld::canMoveTo(int x, int y) const
{
    //return false if there is a pebble there
    list<Actor*>::const_iterator iter = m_actors[y][x].begin();
    for(;iter != m_actors[y][x].end(); iter++)
    {
        if((*iter)->isPebble())
            return false;
    }
    return true;
}

void StudentWorld::addFood(int x, int y, int amt)
{
    //if find the food on the square and add to it
    list<Actor*>::iterator iter = m_actors[y][x].begin();
    for(;iter != m_actors[y][x].end(); iter++)
    {
        if((*iter)->isFood())
        {
            Food* f;
            f = static_cast<Food*>(*iter);
            f->addHP(amt);
        }
    }
}

bool StudentWorld::isFoodHere(int x, int y)
{
    //return true if food is on this square
    list<Actor*>::iterator iter = m_actors[y][x].begin();
    for(; iter!= m_actors[y][x].end(); iter++)
    {
        if((*iter)->isFood())
            return true;
    }
    return false;
}

void StudentWorld::dealWithHazard(int x, int y)
{
    //poison or stun all valid actors
    list<Actor*>::iterator iter = m_actors[y][x].begin();
    for(;iter != m_actors[y][x].end(); iter++)
    {
        if(((*iter)->isAnt() || (*iter)->isBabyG()) && (*iter)->isFirstTime())
        {
            list<Actor*>::iterator rator = m_actors[y][x].begin();
            for(;rator != m_actors[y][x].end(); rator++)
            {
                if((*rator)->isWater())
                    (*iter)->getStunned();
                
                else if((*rator)->isPoison())
                    (*iter)->getPoisoned();
            }
        }
    }
}

int StudentWorld::EatFood(int x, int y, int maxAmt)
{
    //Decrease food on that square by given amt
    list<Actor*>::iterator iter = m_actors[y][x].begin();
    for(;iter != m_actors[y][x].end(); iter++)
    {
        if((*iter)->isFood())
        {
            Food* f;
            f = static_cast<Food*>(*iter);
            if(f->getHP() > maxAmt)
            {
                f->reduceHP(maxAmt);
                return maxAmt;
            }
            else if(f->getHP() <= maxAmt)
            {
                int rtrn = f->getHP();
                f->reduceHP(rtrn);
                return rtrn;
            }
        }
    }
    return 0;
}

void StudentWorld::addActor(Actor *a, int x, int y)
{
    //add a new actor
    if(x < 0 || x > VIEW_WIDTH-1 || y < 0 || y > VIEW_HEIGHT-1)
        return;
    m_actors[y][x].push_back(a);
}

bool StudentWorld::dealWithGAttack(Actor *me)
{
    //get the current position
    int aX = me->getX();
    int aY = me->getY();
    string enemies;
    int num = 0;
    int posit = 0;
    //find the enemies and add them to a string
    list<Actor*>::iterator iter = m_actors[aY][aX].begin();
    for(;iter != m_actors[aY][aX].end(); iter++, posit++)
    {
        if((*iter) != me && ((*iter)->isAdultG() || (*iter)->isAnt() || (*iter)->isBabyG()))
        {
            enemies += to_string(posit);
            num++;
        }
    }
    //if there are enemies on that square
    if(num > 0)
    {
        //choose a random one to deal damage to
        int which = enemies[randInt(0, num-1) - '0'] - '0';
        list<Actor*>::iterator other = m_actors[aY][aX].begin();
        for(int i = 0; i < which; other++, i++)
            ;
        if((*other)->isBabyG())
        {
            BabyGrasshopper* bg;
            bg = static_cast<BabyGrasshopper*>(*other);
            bg->reduceHP(50);
            return true;
        }
        else if((*other)->isAdultG())
        {
            AdultGrasshopper* ag;
            ag = static_cast<AdultGrasshopper*>(*other);
            ag->reduceHP(50);
            //50% random chance of Adult Grasshopper biting back
            if(ag->getHP() > 0)
                if(randInt(0, 1) == 0)
                    dealWithGAttack(ag);
            return true;
        }
        else if((*other)->isAnt())
        {
            Ant* an;
            an = static_cast<Ant*>(*other);
            an->reduceHP(50);
            an->getBit();
            return true;
        }
    }
    return false;
}

bool StudentWorld::dealWithGJump(Actor *me)
{
    //find a random value wihtin a 10 square radius
    int jX = me->getX() + randInt(1, 10)*cos((randInt(0, 360)*acos(-1))/180);;
    int jY = me->getY() + randInt(1, 10)*sin((randInt(0, 360)*acos(-1))/180);
    bool noPebble = true;
    for(int tries = 0; tries < 10;)
    {
        //find one that is within the arena
        for(;jX <= 0 || jX > VIEW_WIDTH-1 || jY <= 0 || jY > VIEW_HEIGHT-1;)
        {
            jX = me->getX() + randInt(1, 10)*cos((randInt(0, 360)*acos(-1))/180);
            jY = me->getY() + randInt(1, 10)*sin((randInt(0, 360)*acos(-1))/180);
        }
        //check if there is a pebble there
        list<Actor*>::iterator iter = m_actors[jY][jX].begin();
        for(;iter != m_actors[jY][jX].end(); iter++)
        {
            if((*iter)->isPebble())
            {
                noPebble = false;
                tries++;
            }
        }
        if(noPebble)
        {
            me->moveTo(jX, jY);
            return true;
        }
    }
    return false;
}

void StudentWorld::dealWithNewAnt(int x, int y, int coloyNum, Compiler* ptr)
{
    //add a new ant to the square
    if(coloyNum == 0)
        addActor(new Ant(this, x, y, IID_ANT_TYPE0, coloyNum, ptr), x, y);
    else if(coloyNum == 1)
        addActor(new Ant(this, x, y, IID_ANT_TYPE1, coloyNum, ptr), x, y);
    else if(coloyNum == 2)
        addActor(new Ant(this, x, y, IID_ANT_TYPE2, coloyNum, ptr), x, y);
    else if(coloyNum == 3)
        addActor(new Ant(this, x, y, IID_ANT_TYPE3, coloyNum, ptr), x, y);
    else
        return;
    m_numAnts[coloyNum]++;
}

void StudentWorld::dealWithDropFood(int x, int y, int amt)
{
    //either add to food that is already there
    list<Actor*>::iterator iter = m_actors[y][x].begin();
    for(;iter != m_actors[y][x].end(); iter++)
    {
        if((*iter)->isFood())
        {
            addFood(x, y, amt);
            return;
        }
    }
    //or allocate new Food item
    if(amt > 0)
        addActor(new Food(this, x, y, amt), x, y);
}

int StudentWorld::dealWithPickupFood(int x, int y, int amt)
{
    //pickup food off the current square
    list<Actor*>::iterator iter = m_actors[y][x].begin();
    for(;iter != m_actors[y][x].end(); iter++)
    {
        if((*iter)->isFood())
        {
            Food* f;
            f = static_cast<Food*>(*iter);
            if(f->getHP() <= 400 && amt <= 1400)
            {
                int x = f->getHP();
                f->reduceHP(x);
                return x;
            }
            else if(f->getHP() > 400 && amt <= 1400)
            {
                f->reduceHP(400);
                return 400;
            }
            else if(amt > 1400)
            {
                int x = 1800 - amt;
                f->reduceHP(x);
                return x;
            }
        }
    }
    return 0;
}

void StudentWorld::dealWithAntAttack(int x, int y, int colonyNum)
{
    string enemies;
    int num = 0;
    int posit = 0;
    //find the enemies and add them to a string
    list<Actor*>::iterator iter = m_actors[y][x].begin();
    for(;iter != m_actors[y][x].end(); iter++, posit++)
    {
        if(((*iter)->isBabyG() || (*iter)->isAdultG()) && !(*iter)->isAnt())
        {
            enemies += to_string(posit);
            num++;
        }
        else if((*iter)->isAnt() && (*iter)->getColonyNum() != colonyNum)
        {
            enemies += to_string(posit);
            num++;
        }
    }
    //if there are enemies on that square
    if(num > 0)
    {
        //choose a random one to deal damage to
        int which = enemies[randInt(0, num) - '0'] - '0';
        list<Actor*>::iterator other = m_actors[y][x].begin();
        for(int i = 0; i < which; other++, i++)
            ;
        if((*other)->isBabyG())
        {
            BabyGrasshopper* bg;
            bg = static_cast<BabyGrasshopper*>(*other);
            bg->reduceHP(15);
            return;
        }
        else if((*other)->isAdultG())
        {
            AdultGrasshopper* ag;
            ag = static_cast<AdultGrasshopper*>(*other);
            ag->reduceHP(15);
            //deal with biteback
            if(ag->getHP() > 0)
                if(randInt(0, 1) == 0)
                    dealWithGAttack(ag);
            return;
        }
        else if((*other)->isAnt())
        {
            Ant* an;
            an = static_cast<Ant*>(*other);
            an->reduceHP(15);
            an->getBit();
            return;
        }
    }
}

void StudentWorld::dealWithPheromone(int x, int y, int colonyNum)
{
    //add to current pheromone
    list<Actor*>::iterator iter = m_actors[y][x].begin();
    for(; iter!= m_actors[y][x].end(); iter++)
    {
        if((*iter)->isPheromone())
        {
            if((*iter)->getColonyNum() == colonyNum)
            {
                Pheromone* ph;
                ph = static_cast<Pheromone*>(*iter);
                if(ph->getHP() >= 512)
                {
                    ph->addHP(758-ph->getHP());
                    return;
                }
                else
                {
                    ph->addHP(256);
                    return;
                }
            }
        }
    }
    //or allocate new pheromone
    if(colonyNum == 0)
        addActor(new Pheromone(this, x, y, IID_PHEROMONE_TYPE0, colonyNum), x, y);
    else if(colonyNum == 1)
        addActor(new Pheromone(this, x, y, IID_PHEROMONE_TYPE1, colonyNum), x, y);
    else if(colonyNum == 2)
        addActor(new Pheromone(this, x, y, IID_PHEROMONE_TYPE2, colonyNum), x, y);
    else if(colonyNum == 3)
        addActor(new Pheromone(this, x, y, IID_PHEROMONE_TYPE3, colonyNum), x, y);
}

bool StudentWorld::isEnemyHere(int x, int y, int colonyNum)
{
    //return true if there is an enemy(grasshopper or opposing ant)
    list<Actor*>::iterator iter = m_actors[y][x].begin();
    for(; iter!= m_actors[y][x].end(); iter++)
    {
        if((*iter)->isAdultG() || (*iter)->isBabyG())
            return true;
        else if((*iter)->isAnt() && (*iter)->getColonyNum() != colonyNum)
            return true;
    }
    return false;
}

bool StudentWorld::isAnthillHere(int x, int y, int colonyNum)
{
    //return true if Anthill is here
    list<Actor*>::iterator iter = m_actors[y][x].begin();
    for(; iter!= m_actors[y][x].end(); iter++)
    {
        if((*iter)->isAntHill() && (*iter)->getColonyNum() == colonyNum)
            return true;
    }
    return false;
}

bool StudentWorld::isPheromoneHere(int x, int y, int colonyNum)
{
    //return true if Pheromone is here
    list<Actor*>::iterator iter = m_actors[y][x].begin();
    for(; iter!= m_actors[y][x].end(); iter++)
    {
        if((*iter)->isPheromone() && (*iter)->getColonyNum() == colonyNum)
            return true;
    }
    return false;
}

bool StudentWorld::isDangerHere(int x, int y, int colonyNum)
{
    //if there is an enemy here or water or poison
    if(isEnemyHere(x, y, colonyNum))
        return true;
    list<Actor*>::iterator iter = m_actors[y][x].begin();
    for(; iter!= m_actors[y][x].end(); iter++)
    {
        if((*iter)->isPoison() || (*iter)->isWater())
            return true;
    }
    return false;
}

int StudentWorld::getLeader()
{
    //find colony with most ants
    int maxNum = 5;
    int pos = -1;
    int i = 0;
    for(; i < 4; i++)
    {
        if(m_numAnts[i] > maxNum)
        {
            maxNum = m_numAnts[i];
            pos = i;
        }
    }
    if(pos != -1)
        return pos;
    
    return -1;
}

void StudentWorld::setDisplayText()
{
    ostringstream oss;
    if(fileNames.size() == 4 && getLeader() == -1)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << ": " << setfill('0') << setw(2) << m_numAnts[0] << " ants"
        << setfill(' ') << setw(8) << fileNames[1] << ": "<< setfill('0') << setw(2) << m_numAnts[1] << " ants"
        << setfill(' ') << setw(8) << fileNames[2] << ": "<< setfill('0') << setw(2) << m_numAnts[2] << " ants"
        << setfill(' ') << setw(8) << fileNames[3] << ": "<< setfill('0') << setw(2) << m_numAnts[3] << " ants";
    }
    else if(fileNames.size() == 4 && getLeader() == 0)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << "*: " << setfill('0') << setw(2) << m_numAnts[0] << " ants"
        << setfill(' ') << setw(8) << fileNames[1] << ": "<< setfill('0') << setw(2) << m_numAnts[1] << " ants"
        << setfill(' ') << setw(8) << fileNames[2] << ": "<< setfill('0') << setw(2) << m_numAnts[2] << " ants"
        << setfill(' ') << setw(8) << fileNames[3] << ": "<< setfill('0') << setw(2) << m_numAnts[3] << " ants";
    }
    else if(fileNames.size() == 4 && getLeader() == 1)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << ": " << setfill('0') << setw(2) << m_numAnts[0] << " ants"
        << setfill(' ') << setw(8) << fileNames[1] << "*: "<< setfill('0') << setw(2) << m_numAnts[1] << " ants"
        << setfill(' ') << setw(8) << fileNames[2] << ": "<< setfill('0') << setw(2) << m_numAnts[2] << " ants"
        << setfill(' ') << setw(8) << fileNames[3] << ": "<< setfill('0') << setw(2) << m_numAnts[3] << " ants";
    }
    else if(fileNames.size() == 4 && getLeader() == 2)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << ": " << setfill('0') << setw(2) << m_numAnts[0] << " ants"
        << setfill(' ') << setw(8) << fileNames[1] << ": "<< setfill('0') << setw(2) << m_numAnts[1] << " ants"
        << setfill(' ') << setw(8) << fileNames[2] << "*: "<< setfill('0') << setw(2) << m_numAnts[2] << " ants"
        << setfill(' ') << setw(8) << fileNames[3] << ": "<< setfill('0') << setw(2) << m_numAnts[3] << " ants";
    }
    else if(fileNames.size() == 4 && getLeader() == 3)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << ": " << setfill('0') << setw(2) << m_numAnts[0] << " ants"
        << setfill(' ') << setw(8) << fileNames[1] << ": "<< setfill('0') << setw(2) << m_numAnts[1] << " ants"
        << setfill(' ') << setw(8) << fileNames[2] << ": "<< setfill('0') << setw(2) << m_numAnts[2] << " ants"
        << setfill(' ') << setw(8) << fileNames[3] << "*: "<< setfill('0') << setw(2) << m_numAnts[3] << " ants";
    }
    else if(fileNames.size() == 3 && getLeader() == -1)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << ": " << setfill('0') << setw(2) << m_numAnts[0] << " ants"
        << setfill(' ') << setw(8) << fileNames[1] << ": "<< setfill('0') << setw(2) << m_numAnts[1] << " ants"
        << setfill(' ') << setw(8) << fileNames[2] << ": "<< setfill('0') << setw(2) << m_numAnts[2] << " ants";
    }
    else if(fileNames.size() == 3 && getLeader() == 0)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << "*: " << setfill('0') << setw(2) << m_numAnts[0] << " ants"
        << setfill(' ') << setw(8) << fileNames[1] << ": "<< setfill('0') << setw(2) << m_numAnts[1] << " ants"
        << setfill(' ') << setw(8) << fileNames[2] << ": "<< setfill('0') << setw(2) << m_numAnts[2] << " ants";
    }
    else if(fileNames.size() == 3 && getLeader() == 1)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << ": " << setfill('0') << setw(2) << m_numAnts[0] << " ants"
        << setfill(' ') << setw(8) << fileNames[1] << "*: "<< setfill('0') << setw(2) << m_numAnts[1] << " ants"
        << setfill(' ') << setw(8) << fileNames[2] << ": "<< setfill('0') << setw(2) << m_numAnts[2] << " ants";
    }
    else if(fileNames.size() == 3 && getLeader() == 2)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << ": " << setfill('0') << setw(2) << m_numAnts[0] << " ants"
        << setfill(' ') << setw(8) << fileNames[1] << ": "<< setfill('0') << setw(2) << m_numAnts[1] << " ants"
        << setfill(' ') << setw(8) << fileNames[2] << "*: "<< setfill('0') << setw(2) << m_numAnts[2] << " ants";
    }
    else if(fileNames.size() == 2 && getLeader() == -1)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << ": " << setfill('0') << setw(2) << m_numAnts[0] << " ants"
        << setfill(' ') << setw(8) << fileNames[1] << ": "<< setfill('0') << setw(2) << m_numAnts[1] << " ants";
    }
    else if(fileNames.size() == 2 && getLeader() == 0)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << "*: " << setfill('0') << setw(2) << m_numAnts[0] << " ants"
        << setfill(' ') << setw(8) << fileNames[1] << ": "<< setfill('0') << setw(2) << m_numAnts[1] << " ants";
    }
    else if(fileNames.size() == 2 && getLeader() == 1)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << ": " << setfill('0') << setw(2) << m_numAnts[0] << " ants"
        << setfill(' ') << setw(8) << fileNames[1] << "*: "<< setfill('0') << setw(2) << m_numAnts[1] << " ants";
    }
    else if(fileNames.size() == 1 && getLeader() == -1)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << ": " << setfill('0') << setw(2) << m_numAnts[0] << " ants";
    }
    else if(fileNames.size() == 1 && getLeader() == 0)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks) <<  " -"
        << setw(8) << fileNames[0] << "*: " << setfill('0') << setw(2) << m_numAnts[0] << " ants";
    }
    else if(fileNames.size() == 0)
    {
        oss <<  "Ticks:" << setw(5) << std::to_string(2000-ticks);
    }
    string s = oss.str();
    setGameStatText(s);
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
