#include "newSequence.h"

Sequence::Sequence(int max)
{
    m_size = 0;
    m_max = max;
    m_arr = new ItemType[m_max];
}    // Create an empty sequence (i.e., one whose size() is 0).

Sequence::Sequence(const Sequence& other)
{
    m_size = other.m_size;
    m_max = other.m_max;
    m_arr = new ItemType[other.m_max];
    for(int i = 0; i < other.m_size; i++)
    {
        m_arr[i] = other.m_arr[i];
    }
}

Sequence& Sequence::operator=(const Sequence& other)
{
    if(this != &other)
    {
        m_size = other.m_size;
        m_max = other.m_max;
        m_arr = new ItemType[other.m_max];
        for(int i = 0; i < other.m_size; i++)
        {
            m_arr[i] = other.m_arr[i];
        }
    }
    return *this;
}

Sequence::~Sequence()
{
    delete [] m_arr;
}

bool Sequence::empty() const // Return true if the sequence is empty, otherwise false.
{
    return m_size == 0;
}
int Sequence::size() const    // Return the number of items in the sequence.
{
    return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
    if(pos < 0 || pos > size())
        return false;
    
    if(size() == m_max)
        return false;
    
    if(pos == size())
    {
        m_arr[size()] = value;
        m_size++;
        return true;
    }
    else
    {
        ItemType copy = m_arr[pos];
        m_arr[pos] = value;
        for(int i = size(); i > pos; i--)
        {
            m_arr[i] = m_arr[i-1];
        }
        m_arr[pos+1] = copy;
        m_size++;
        return true;
    }
    
    return false;
}
// Insert value into the sequence so that it becomes the item at
// position pos.  The original item at position pos and those that
// follow it end up at positions one higher than they were at before.
// Return true if 0 <= pos <= size() and the value could be
// inserted.  (It might not be, if the sequence has a fixed capacity,
// (e.g., because it's implemented using a fixed-size m_array) and is
// full.)  Otherwise, leave the sequence unchanged and return false.
// Notice that if pos is equal to size(), the value is inserted at the
// end.

int Sequence::insert(const ItemType& value)
{
    int p = -1;
    
    if(size() == m_max)
        return p;
    
    for(int i = 0; i < size(); i++)
    {
        if(value <= m_arr[i])
        {
            p = i;
            ItemType copy = m_arr[p];
            m_arr[p] = value;
            for(int j = size(); j > p; j--)
            {
                m_arr[j] = m_arr[j-1];
            }
            m_arr[p+1] = copy;
            m_size++;
            return p;
        }
    }
    if(p == -1)
    {
        p = size();
        m_arr[size()] = value;
        m_size++;
        return p;
    }
    return p;
}
// Let p be the smallest integer such that value <= the item at
// position p in the sequence; if no such item exists (i.e.,
// value > all items in the sequence), let p be size().  Insert
// value into the sequence so that it becomes the item at position
// p.  The original item at position p and those that follow it end
// up at positions one higher than before.  Return p if the value
// was actually inserted.  Return -1 if the value was not inserted
// (perhaps because the sequence has a fixed capacity and is full).

bool Sequence::erase(int pos)
{
    if(pos < 0 || pos >= size())
        return false;
    
    for(int i = pos; i < size()-1; i++)
        m_arr[i] = m_arr[i+1];
    
    m_size--;
    return true;
}
// If 0 <= pos < size(), remove the item at position pos from
// the sequence (so that all items that followed this item end up at
// positions one lower than they were at before), and return true.
// Otherwise, leave the sequence unchanged and return false.

int Sequence::remove(const ItemType& value)
{
    int count = 0;
    
    for(int i = 0; i < size(); i++)
    {
        if(m_arr[i] == value)
        {
            for(int j = i; j < size()-1; j++)
                m_arr[j] = m_arr[j+1];
            m_size--;
            count++;
            i--;
        }
    }
    return count;
}
// Erase all items from the sequence that == value.  Return the
// number of items removed (which will be 0 if no item == value).

bool Sequence::get(int pos, ItemType& value) const
{
    if(pos < 0 || pos >= size())
        return false;
    
    value = m_arr[pos];
    return true;
}
// If 0 <= pos < size(), copy into value the item at position pos
// in the sequence and return true.  Otherwise, leave value unchanged
// and return false.

bool Sequence::set(int pos, const ItemType& value)
{
    if(pos < 0 || pos >= size())
        return false;
    
    m_arr[pos] = value;
    return true;
}
// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.

int Sequence::find(const ItemType& value) const
{
    int p = -1;
    for(int i = 0; i < size(); i++)
    {
        if(m_arr[i] == value)
        {
            p = i;
            return p;
        }
    }
    return p;
}
// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.

void Sequence::swap(Sequence& other)
{
    ItemType *temp = m_arr;
    int temp1 = m_size;
    int temp2 = m_max;
    m_arr = other.m_arr;
    other.m_arr = temp;
    m_size = other.m_size;
    m_max = other.m_max;
    other.m_size = temp1;
    other.m_max = temp2;
}
// Exchange the contents of this sequence with the other one.