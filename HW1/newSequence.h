#ifndef NEWSEQUENCE_H
#define NEWSEQUENCE_H

#include <string>

typedef unsigned long ItemType;
const int DEFAULT_MAX_ITEMS = 200;

class Sequence
{
public:
    Sequence(int max = DEFAULT_MAX_ITEMS);
    
    Sequence(const Sequence& other);
    
    Sequence& operator=(const Sequence& other);
    
    ~Sequence();
    
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
    int m_size;
    int m_max;
    ItemType* m_arr;
};

#endif /* NEWSEQUENCE_H */
