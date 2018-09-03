#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <iostream>
#include <string>

typedef unsigned long ItemType;
const int DEFAULT_MAX_ITEMS = 200;

class Sequence
{
public:
    Sequence();
    
    bool empty() const;
    
    int size() const;
    
    bool insert(int pos, const ItemType& value);
    
    int insert(const ItemType& value);
    
    bool erase(int pos);
    
    int remove(const ItemType& value);
    
    bool get(int pos, ItemType& value) const;
    
    bool set(int pos, const ItemType& value);
    
    int find(const ItemType& value) const;
    
    void swap(Sequence& other);
    
private:
    ItemType m_arr[DEFAULT_MAX_ITEMS];
    int m_size;
};

#endif /* SEQUENCE_H */
