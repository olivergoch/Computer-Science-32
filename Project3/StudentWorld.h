#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Compiler.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include "GraphObject.h"
#include <string>
#include <ctime>
#include <vector>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
        ticks = 0;
        for(int i = 0; i < 4; i++)
        {
            m_numAnts[i] = 0;
        }
        
	}

    bool loadTheField();
    
    //Bug handling & helper functions
    int EatFood(int x, int y, int maxAmt);
    void dealWithNewAnt(int x, int y, int coloyNum, Compiler* ptr); 
    void dealWithDropFood(int x, int y, int amt);
    int dealWithPickupFood(int x, int y, int amt);
    void dealWithAntAttack(int x, int y, int colonyNum);
    void dealWithPheromone(int x, int y, int colonyNum);
    bool isEnemyHere(int x, int y, int colonyNum);
    bool isFoodHere(int x, int y);
    bool isAnthillHere(int x, int y, int colonyNum);
    bool isPheromoneHere(int x, int y, int colonyNum);
    bool isDangerHere(int x, int y, int colonyNum);
    //Able to move there
    bool canMoveTo(int x, int y) const;
    //Add Food Helper
    void addFood(int x, int y, int amt);
    //Helper with Poison or Water
    void dealWithHazard(int x, int y);
    //Add actor
    void addActor(Actor* a, int x, int y);
    //AdultG Helper Functions
    bool dealWithGAttack(Actor* me);
    bool dealWithGJump(Actor* me);
    void setDisplayText();
    int getLeader();
    
    
    
	virtual int init()
	{
        //load the field
        if(!loadTheField())
           return GWSTATUS_LEVEL_ERROR;
        for(int i = 0; i < fileNames.size(); i++)
        {
            for(int j = 0; j < fileNames[i].size(); j++)
                fileNames[i][j] = tolower(fileNames[i][j]);
        }
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
        
        ticks++;
        for(int y = 0; y < VIEW_WIDTH; y++)
        {
            for(int x = 0; x < VIEW_HEIGHT; x++)
            {
                std::list<Actor*>::iterator iter = m_actors[y][x].begin();
                for(;iter != m_actors[y][x].end(); iter++)
                {
                    dealWithHazard(x, y);
                    (*iter)->doSomething();
                    int movedX = (*iter)->getX();
                    int movedY = (*iter)->getY();
                    m_actors[movedY][movedX].push_back((*iter));
                    iter = m_actors[y][x].erase(iter);
                    
                }
                std::list<Actor*>::iterator mopUP = m_actors[y][x].begin();
                for(;mopUP != m_actors[y][x].end(); mopUP++)
                {
                    if((*mopUP)->isDead())
                    {
                        if((*mopUP)->isFood() || (*mopUP)->isAntHill())
                        {
                            delete * mopUP;
                            mopUP = m_actors[y][x].erase(mopUP);
                        }
                        else
                        {
                            delete * mopUP;
                            mopUP = m_actors[y][x].erase(mopUP);
                            if(isFoodHere(x, y))
                                addFood(x, y, 100);
                            else
                                m_actors[y][x].push_back(new Food(this, x, y, 100));
                        }
                    }
                }
            }
        }
        setDisplayText();
        if(ticks == 2000)
        {
            if(getLeader() == -1)
                return GWSTATUS_NO_WINNER;
            else
            {
                setWinner(fileNames[getLeader()]);
                return GWSTATUS_PLAYER_WON;
            }
        }
        else
            return GWSTATUS_CONTINUE_GAME;
	}
    
	virtual void cleanUp()
	{
        for(int i = 0; i < VIEW_HEIGHT; i++)
        {
            for(int j = 0; j < VIEW_WIDTH; j++)
            {
                std::list<Actor*>::iterator iter = m_actors[i][j].begin();
                for(;iter != m_actors[i][j].end(); iter++)
                {
                    delete * iter;
                    iter = m_actors[i][j].erase(iter);
                }
            }
        }
        for(int k = 0; k < m_compilers.size(); k++)
        {
            delete m_compilers[k];
            m_compilers[k] = nullptr;
        }
        m_compilers.clear();
    }
    
    ~StudentWorld()
    {
        cleanUp();
        cleanUp();
    }

private:
    int ticks;
    std::list<Actor*> m_actors[VIEW_WIDTH][VIEW_HEIGHT];
    std::vector<Compiler*> m_compilers;
    int m_numAnts[4];
    std::vector<std::string> fileNames;
};

#endif // STUDENTWORLD_H_
