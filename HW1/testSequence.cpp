#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void testSequence()
{
    Sequence s;
    assert(s.empty());
    assert(s.find(42) == -1);
    s.insert(42);
    assert(s.size() == 1  &&  s.find(42) == 0);
    assert(!s.empty());
    s.insert(-1, 5);
    assert(s.size() == 1  &&  s.find(42) == 0);
    s.insert(3, 5);
    assert(s.size() == 1  &&  s.find(42) == 0);
    for(int i = 1; i < 200; i++)
        s.insert(50);
    assert(s.insert(3, 5) == false);
    assert(s.insert(15) == -1);
    assert(s.remove(50) == 199);
    assert(s.insert(1, 20));
    assert(s.size() == 2  &&  s.find(42) == 0 && s.find(20) == 1);
    assert(s.insert(2, 15));
    assert(s.insert(3, 17));
    assert(s.remove(16) == 0);
    assert(s.size() == 4  &&  s.find(42) == 0 && s.find(20) == 1 && s.find(15) == 2 && s.find(17) == 3);
    assert(!s.erase(20));
    assert(!s.erase(-1));
    assert(s.erase(1));
    assert(s.size() == 3  &&  s.find(42) == 0 && s.find(15) == 1 && s.find(17) == 2);
    unsigned long x = 5;
    assert(s.set(1, x));
    assert(s.size() == 3  &&  s.find(42) == 0 && s.find(5) == 1 && s.find(17) == 2);
    assert(s.insert(16) == 0);
    assert(s.size() == 4 && s.get(0,x) && x == 16);
    assert(s.set(0, 1));
    assert(!s.empty());
    assert(!s.set(-1, 10));
    assert(!s.set(4, 5));
    assert(!s.get(5, x) && x == 16);
    assert(!s.get(-1, x) && x == 16);
    assert(s.insert(1) == 0);
    Sequence s1;
    assert(s1.empty());
    assert(s1.size() == 0);
    assert(s1.insert(10) == 0);
    assert(s1.insert(9) == 0);
    assert(s1.insert(8) == 0);
    assert(s1.remove(8) == 1);
    assert(s.set(1, 20));
    s.swap(s1);
    assert(s1.size() == 5 && s1.find(1) == 0 && s1.find(20) == 1 && s1.find(42) == 2 && s1.find(5) == 3 && s1.find(17) == 4
           && s.size() == 2 && s.find(9) == 0 && s.find(10) == 1);
}

int main()
{
    testSequence();
    cout << "Passed all tests" << endl;
}