#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <string>

typedef std::string ItemType;
class Sequence
{
public:
    Sequence();
    ~Sequence();
    Sequence(const Sequence& other);
    Sequence& operator=(const Sequence& other);
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
    void dump() const;
private:
    struct Node
    {
        Node* m_next;
        Node* m_prev;
        ItemType m_data;
    };
    int m_size;
    Node* m_head;
    Node* m_tail;
};

//Non member function declarations
//Subsquence
int subsequence(const Sequence& seq1, const Sequence& seq2);
//interleave
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif /* SEQUENCE_H */
