#include "ScoreList.h"
#include <iostream>
#include <cassert>
using namespace std;

void testScore()
{
    ScoreList s;
    assert(s.add(5));
    assert(!s.add(-1));
    assert(s.add(0));
    assert(!s.add(101));
    assert(s.size() == 2);
    for(int i = 0; i < 10; i++)
        s.add(83);
    assert(s.size() == 12);
    assert(s.maximum() == 83);
    assert(s.remove(83));
    assert(s.size() == 11);
    assert(s.maximum() == 83);
    assert(s.remove(5));
    assert(s.remove(0));
    for(int i = 0; i < 12; i++)
        s.add(10);
    assert(s.size() == 21);
    assert(s.remove(10));
    assert(s.size() == 20);
    assert(s.minimum() == 10);
    assert(!s.remove(11));
    assert(s.remove(10));
    assert(s.size() == 19 && s.minimum() == 10);
}

int main()
{
    testScore();
    cout << "Passed all testz" << endl;
}