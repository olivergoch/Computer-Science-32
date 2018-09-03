#include <iostream>
#include <string>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if(sr == er && sc == ec)
        return true;
    
    if(maze[sr][sc] == 'X' || maze[sr][sc] == 'E')
        return false;
    
    maze[sr][sc] = 'E';
    
    //NORTH
    if(pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
        return true;
    
    //EAST
    if(pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
        return true;
    
    //SOUTH
    if(pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
        return true;
    
    //WEST
    if(pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
        return true;
    
     return false;
}
