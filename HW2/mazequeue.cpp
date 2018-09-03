#include <iostream>
#include <queue>
#include <string>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    queue<Coord> coordQueue;
    Coord start(sr, sc);
    coordQueue.push(start);
    Coord end(er, ec);
    
    maze[sr][sc] = 'E';
    //E means encountered
    while(!coordQueue.empty())
    {
        Coord current = coordQueue.front();
        coordQueue.pop();
        if(current.r() == er && current.c() == ec)
            return true;
        
        //NORTH
        if(maze[current.r()-1][current.c()] != 'X' && maze[current.r()-1][current.c()] != 'E')
        {
            Coord NORTH(current.r()-1,current.c());
            coordQueue.push(NORTH);
            maze[current.r()-1][current.c()] = 'E';
        }
        
        //EAST
        if(maze[current.r()][current.c()+1] != 'X' && maze[current.r()][current.c()+1] != 'E')
        {
            Coord EAST(current.r(),current.c()+1);
            coordQueue.push(EAST);
            maze[current.r()][current.c()+1] = 'E';
        }
        
        //SOUTH
        if(maze[current.r()+1][current.c()] != 'X' && maze[current.r()+1][current.c()] != 'E')
        {
            Coord SOUTH(current.r()+1,current.c());
            coordQueue.push(SOUTH);
            maze[current.r()+1][current.c()] = 'E';
        }
        
        //WEST
        if(maze[current.r()][current.c()-1] != 'X' && maze[current.r()][current.c()-1] != 'E')
        {
            Coord WEST(current.r(),current.c()-1);
            coordQueue.push(WEST);
            maze[current.r()][current.c()-1] = 'E';
        }
        
    }
    return false;
}