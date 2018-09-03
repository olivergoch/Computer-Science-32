#include "Sequence.h"
#include <iostream>

// Create an empty sequence (i.e., one whose size() is 0).
Sequence::Sequence()
{
    m_size = 0;
    m_head = nullptr;
    m_tail = nullptr;
}

//destructor
Sequence::~Sequence()
{
    int num = this->m_size;
    for(int i = 0; i < num; i++)
    {
        this->erase(0);
    }
}

//copy constructor
Sequence::Sequence(const Sequence& other)
{
    Node* p = other.m_head;
    //size = 0;
    this->m_size = 0;
    this->m_head = nullptr;
    this->m_tail = nullptr;
    //insert the data into the new Sequence
    for(int i = 0; i < other.m_size; i++, p = p->m_next)
        this->insert(i, p->m_data);
    
}

//assignment operator
Sequence& Sequence::operator=(const Sequence& other)
{
    //copy and swap other with the new one using a temp
    Sequence temp(other);
    this->swap(temp);
    return *this;
}

// Return true if the sequence is empty, otherwise false.
bool Sequence::empty() const
{
    return m_size == 0;
}

// Return the number of items in the sequence.
int Sequence::size() const
{
    return m_size;
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
bool Sequence::insert(int pos, const ItemType& value)
{
    //if out of the sequence return false
    if(pos < 0 || pos > m_size)
        return false;
    
    //if the position is 0 and size is 0 insert it at beginning
    if(pos == 0 && m_size == 0)
    {
        Node* newGuy = new Node;
        newGuy->m_next = nullptr;
        newGuy->m_prev = nullptr;
        newGuy->m_data = value;
        m_head = newGuy;
        m_tail = newGuy;
        m_size++;
        return true;
    }
    //if the position is 0 but size is greater than 1
    else if(pos == 0)
    {
        Node* newGuy = new Node;
        newGuy->m_next = m_head;
        newGuy->m_prev = nullptr;
        newGuy->m_data = value;
        m_head->m_prev = newGuy;
        m_head = newGuy;
        m_size++;
        return true;
    }
    //if the position is the end
    else if(pos == m_size)
    {
        Node* p = m_tail;
        Node* newGuy = new Node;
        newGuy->m_next = nullptr;
        newGuy->m_prev = p;
        p->m_next = newGuy;
        m_tail = newGuy;
        newGuy->m_data = value;
        m_size++;
        return true;
    }
    //if it's in the middle
    else
    {
        Node* p = m_head;
        for(int i = 0; p != nullptr && i < pos; i++, p = p->m_next)
        ;
        Node* newGuy = new Node;
        newGuy->m_next = p;
        newGuy->m_prev = p->m_prev;
        p->m_prev->m_next = newGuy;
        p->m_prev = newGuy;
        newGuy->m_data = value;
        m_size++;
        return true;
    }
    return false;
}

// Let p be the smallest integer such that value <= the item at
// position p in the sequence; if no such item exists (i.e.,
// value > all items in the sequence), let p be size().  Insert
// value into the sequence so that it becomes the item at position
// p.  The original item at position p and those that follow it end
// up at positions one higher than before.  Return p if the value
// was actually inserted.  Return -1 if the value was not inserted
// (perhaps because the sequence has a fixed capacity and is full).
int Sequence::insert(const ItemType& value)
{
    Node* pos = m_head;
    int p = 0;
    
    //look for a position that is greater than given value
    for(; pos!= nullptr && value > pos->m_data; p++, pos = pos->m_next)
        ;
    
    //if it's the only element
    if(p == 0 && m_size == 0)
    {
        Node* newGuy = new Node;
        newGuy->m_next = nullptr;
        newGuy->m_prev = nullptr;
        newGuy->m_data = value;
        m_head = newGuy;
        m_tail = newGuy;
        m_size++;
        return p;
    }
    
    //if it's the first element
    else if(p == 0)
    {
        Node* newGuy = new Node;
        newGuy->m_next = m_head;
        newGuy->m_prev = nullptr;
        newGuy->m_data = value;
        m_head->m_prev = newGuy;
        m_head = newGuy;
        m_size++;
        return p;
    }
    
    //if it is less than no values, put it at the end
    else if(pos == nullptr)
    {
        Node* pall = m_tail;
        Node* newGuy = new Node;
        newGuy->m_next = nullptr;
        newGuy->m_prev = pall;
        pall->m_next = newGuy;
        m_tail = newGuy;
        newGuy->m_data = value;
        m_size++;
        p = m_size;
        p--;
        return p;
    }
    
    //if it's somewhere in the middle
    else
    {
        Node* newGuy = new Node;
        newGuy->m_next = pos;
        newGuy->m_prev = pos->m_prev;
        pos->m_prev->m_next = newGuy;
        pos->m_prev = newGuy;
        newGuy->m_data = value;
        m_size++;
        return p;
    }
    return -1;
}

// If 0 <= pos < size(), remove the item at position pos from
// the sequence (so that all items that followed this item end up at
// positions one lower than they were at before), and return true.
// Otherwise, leave the sequence unchanged and return false.
bool Sequence::erase(int pos)
{
    //if it's outside the bounds
    if(pos < 0 || pos >= m_size)
        return false;
    
    //if it's the only element
    if(m_size == 1)
    {
        Node* p = m_head;
        m_head = nullptr;
        m_tail = nullptr;
        delete p;
        m_size--;
        return true;
    }
    
    //if it's the first element
    else if(pos == 0)
    {
        Node* p = m_head;
        p->m_next->m_prev = nullptr;
        m_head = p->m_next;
        delete p;
        m_size--;
        return true;
    }
    
    //if it's last element
    else if(pos == m_size-1)
    {
        Node* p = m_tail;
        p->m_prev->m_next = nullptr;
        m_tail = p->m_prev;
        delete p;
        m_size--;
        return true;
    }
    //elsewise it's in the middle
    else
    {
        Node* p = m_head;
        for(int i = 0; p != nullptr && i < pos; i++, p = p->m_next)
            ;
        p->m_prev->m_next = p->m_next;
        p->m_next->m_prev = p ->m_prev;
        delete p;
        m_size--;
        return true;
    }
    return false;
}

// Erase all items from the sequence that == value.  Return the
// number of items removed (which will be 0 if no item == value).
int Sequence::remove(const ItemType& value)
{
    Node* p = m_head;
    int pos = 0;
    int num = 0;
    //go through list
    while(p!= nullptr)
    {
        //if you find the value
        if(p->m_data == value)
        {
            p = p->m_next;
            erase(pos);
            num++;
            pos--;
        }
        else
        {
            p = p->m_next;
        }
        pos++;
    }
    
    return num;
}

// If 0 <= pos < size(), copy into value the item at position pos
// in the sequence and return true.  Otherwise, leave value unchanged
// and return false.
bool Sequence::get(int pos, ItemType& value) const
{
    if(pos < 0 || pos >= m_size)
        return false;
    
    Node* p = m_head;
    //cycle through list until you find the position
    for(int i = 0; p != nullptr && i < pos; i++, p = p->m_next)
        ;
    //set the value to the data
    value = p->m_data;
    return true;
}

// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.
bool Sequence::set(int pos, const ItemType& value)
{
    if(pos < 0 || pos >= m_size)
        return false;
    
    Node* p = m_head;
    //cycle through list until you find the position
    for(int i = 0; p != nullptr && i < pos; i++, p = p->m_next)
        ;
    //set the data to the value
    p->m_data = value;
    return true;
}

// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.
int Sequence::find(const ItemType& value) const
{
    Node* pos = m_head;
    int p;
    //cycle through list until you find value
    for(p = 0; pos != nullptr && value != pos->m_data; p++, pos = pos->m_next)
        ;
    //if it reached the end without finding the value
    if(pos == nullptr)
    {
        //return -1
        p = -1;
        return p;
    }
    else
        //return the position
        return p;
}

// Exchange the contents of this sequence with the other one.
void Sequence::swap(Sequence& other)
{
    //if the this is longer than the other
    if(this->m_size > other.m_size)
    {
        //set them equal to each other in terms of length
        int start = other.m_size;
        int count = 0;
        Node* p = this->m_head;
        for(int i = other.m_size; i < m_size; i++, p = p->m_next)
        {
            other.insert(i, p->m_data);
            count++;
        }
        //exchange the data from this to the other
        Node* po = this->m_head;
        for(int i = 0; i < m_size; i++, po = po->m_next)
        {
            ItemType copy = po->m_data;
            other.get(i, po->m_data);
            other.set(i, copy);
        }
        //erase the extra data in this
        for(int i = 0; i < count; i++)
            this->erase(start);
    }
    //if other is longer than this
    else if(this->m_size < other.m_size)
    {
        //set them equal to each other in terms of length
        int start = m_size;
        int count = 0;
        Node* p = other.m_head;
        for(int i = m_size; i < other.m_size; i++, p = p->m_next)
        {
            this->insert(i, p->m_data);
            count++;
        }
        //exchange the data from this to the other
        Node* po = other.m_head;
        for(int i = 0; i < m_size; i++, po = po->m_next)
        {
            ItemType copy = po->m_data;
            this->get(i, po->m_data);
            this->set(i, copy);
        }
        //erase the extra data in other
        for(int i = 0; i < count; i++)
            other.erase(start);
    }
    else if(this->m_size == other.m_size)
    {
        //if they have the same length then exchange the data
        Node* po = other.m_head;
        for(int i = 0; i < m_size; i++, po = po->m_next)
        {
            ItemType copy = po->m_data;
            this->get(i, po->m_data);
            this->set(i, copy);
        }
    }
}

//Non member function implementations

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
    //get the sizes of both sequences
    int smax = seq2.size();
    int mmax = seq1.size();
    
    if(smax > mmax)
        return -1;
    
    //if the second sequence is empty, return -1
    if(seq2.empty())
        return -1;
    
    //set up the system
    int i = 0;
    ItemType x;
    ItemType y;
    //start at beginning of first sequence
    for(; i < mmax;i++)
    {
        seq1.get(i,x);
        seq2.get(0,y);
        //if the beginning of the subsequence is equal to the bigger sequence
        if(x == y)
        {
            int j = 0;
            int k = i;
            //go through the sequences
            for(; j < smax; j++, k++)
            {
                seq1.get(k, x);
                seq2.get(j, y);
                //if one value is not the same, break out of the loop
                if(x != y)
                    break;
            }
            //if it exits subsequence, then all values are the same, return starting point
            if(j == smax)
                return k-j;
        }
    }
    //if it went through bigger sequence with no complete matches, return -1
    if(i == mmax)
        return -1;
    
    return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
    bool same1 = false;
    bool same2 = false;
    
    //check if the first sequence has the same size has the result sequence
    if(seq1.size() == result.size())
    {
        //check if they are the same sequence through subsequence
        if(subsequence(seq1, result) == -1)
            same1 = false;
        if(subsequence(seq1, result) == 0)
            same1 = true;
    }
    
    //check if the second sequence has the same size has the result sequence
    if(seq2.size() == result.size())
    {
        //check if they are the same sequence through subsequence
        if(subsequence(seq2, result) == -1)
            same2 = false;
        if(subsequence(seq2, result) == 0)
            same2 = true;
    }
    
    //if seq1 and result are the same, insert seq2 into result
    if(same1)
    {
        for(int i = 0; i < seq2.size(); i++)
        {
            ItemType x;
            seq2.get(i, x);
            //intersperse seq2 in result
            if(result.insert(i*2+1, x))
            {}
            else
            {
                //if seq2 is longer than seq1, insert onto end of result
                result.insert(result.size(),x);
            }
        }
        return;
    }
    
    //if seq2 and result are the same, insert seq1 into result
    if(same2)
    {
        for(int i = 0; i < seq1.size(); i++)
        {
            ItemType x;
            seq1.get(i, x);
            //intersperse seq1 in result
            if(result.insert(i*2+1, x))
            {}
            else
            {
                //if seq1 is longer than seq2, insert onto end of result
                result.insert(result.size(),x);
            }
        }
        return;
    }
    
    //empty result string
    if(!result.empty() && !same1 && !same2)
    {
        int x = result.size();
        for(int i = 0; i < x; i++)
            result.erase(0);
    }
    
    //if seq1 is empty, then result is seq2
    if(seq1.empty())
    {
        result = seq2;
        return;
    }
    
    //if seq2 is empty, then result is seq1
    if(seq2.empty())
    {
        result = seq1;
        return;
    }
    
    //set result to seq1
    result = seq1;
    for(int i = 0; i < seq2.size(); i++)
    {
        ItemType x;
        seq2.get(i, x);
        //intersperse seq2 in result
        if(result.insert(i*2+1, x))
        {}
        else
        {
            //if seq2 is longer than seq1, then insert onto the end of result
            result.insert(result.size(),x);
        }
    }
    
}

void Sequence::dump() const
{
    std::cerr << "---BEGIN DUMPING---" << std::endl;
    Node* p = m_head;
    int count = 0;
    for(; p != nullptr; p = p->m_next, count++)
        std::cerr << "Position " << count << " = " << p->m_data << std::endl;
    std::cerr << "Size is " << m_size << std::endl;
    std::cerr << "---END DUMPING---" << std::endl;
}

