#include "ScoreList.h"

ScoreList::ScoreList()
{
    
}// Create an empty score list.
    
bool ScoreList::add(unsigned long score)
{
    if(score > 100)
        return false;
    
    return s.insert(score) != -1;
}
    // If the score is valid (a value from 0 to 100) and the score list
    // has room for it, add it to the score list and return true.
    // Otherwise, leave the score list unchanged and return false.
    
bool ScoreList::remove(unsigned long score)
{
    unsigned long x = s.find(score);
    return s.erase(x);
}
    // Remove one instance of the specified score from the score list.
    // Return true if a score was removed; otherwise false.
    
int ScoreList::size() const
{
    return s.size();
}// Return the number of scores in the list.
    
unsigned long ScoreList::minimum() const
{
    if(s.empty())
        return NO_SCORE;
    
    unsigned long x;
    s.get(0, x);
    
    return x;
}
    // Return the lowest score in the score list.  If the list is
    // empty, return NO_SCORE.
    
unsigned long ScoreList::maximum() const
{
    if(s.empty())
        return NO_SCORE;
    
    unsigned long x;
    s.get(s.size()-1, x);
    
    return x;
}
    // Return the highest score in the score list.  If the list is
    // empty, return NO_SCORE.