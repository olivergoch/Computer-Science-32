#include "History.h"
#include "Arena.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    //initalize the grid
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            m_grid[r-1][c-1] = ('.');
}

bool History::record(int r, int c)
{
    if(r < 1 || r > m_rows)
        return false;
    
    if(c < 1 || c > m_cols)
        return false;
    
    //if in it's at 0
    if(m_grid[r-1][c-1] == '.')
    {
        m_grid[r-1][c-1] = 'A';
        return true;
    }
    //if it maxes out
    else if(m_grid[r-1][c-1] == 'Z')
    {
        m_grid[r-1][c-1] = 'Z';
        return true;
    }
    //if it's in between
    else
    {
        m_grid[r-1][c-1]++;
        return true;
    }
}

void History::display() const
{
    //draw the screen
    clearScreen();
    for (int r = 1; r <= m_rows; r++)
    {
        for (int c = 1; c <= m_cols; c++)
            cout << m_grid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
}