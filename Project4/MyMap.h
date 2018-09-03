// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node* m_leftChild;
        Node* m_rightChild;
    };
    Node* m_root;
    int m_size;
    void deleteNode(Node* n);
};

template<typename KeyType, typename ValueType>
inline
int MyMap<KeyType, ValueType>::size() const
{
    return m_size;
}


template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap()
{
    m_root = nullptr;
    m_size = 0;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType &key, const ValueType &value)
{
    //if there is no tree yet
    if(m_root == nullptr)
    {
        Node* newGuy = new Node;
        newGuy->m_key = key;
        newGuy->m_value = value;
        newGuy->m_leftChild = nullptr;
        newGuy->m_rightChild = nullptr;
        m_root = newGuy;
        m_size++;
        return;
    }
    //if the value is already there
    else if(find(key) != nullptr)
    {
        ValueType* newValue = find(key);
        *newValue = value;
        return;
    }
    //otherwise a new leaf node is created
    else
    {
        //go until a leaf node is reached
        Node* p = m_root;
        for(;;)
        {
            if(key < p->m_key && p->m_leftChild == nullptr)
                break;
            else if(key > p->m_key && p->m_rightChild == nullptr)
                break;
            else if(key < p->m_key)
                p = p->m_leftChild;
            else if(key > p->m_key)
                p = p->m_rightChild;
        }
        //add it to left of leaf node
        if(key < p->m_key)
        {
            Node* newGuy = new Node;
            newGuy->m_key = key;
            newGuy->m_value = value;
            newGuy->m_leftChild = nullptr;
            newGuy->m_rightChild = nullptr;
            p->m_leftChild = newGuy;
            m_size++;
        }
        //add it to right of leaf node
        else if(key > p->m_key)
        {
            Node* newGuy = new Node;
            newGuy->m_key = key;
            newGuy->m_value = value;
            newGuy->m_leftChild = nullptr;
            newGuy->m_rightChild = nullptr;
            p->m_rightChild = newGuy;
            m_size++;
        }
    }
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const
{
    Node* p = m_root;
    //go until you reach a leaf node or find the matching key
    while(p != nullptr && p->m_key != key)
    {
        if(key < p->m_key)
            p = p->m_leftChild;
        else if(key > p->m_key)
            p = p->m_rightChild;
    }
    if(p == nullptr)
        return nullptr;
    else
        return &(p->m_value);
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear()
{
    //call helper function
    if(m_size != 0)
        deleteNode(m_root);
    m_root = nullptr;
    m_size = 0;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::deleteNode(Node* n)
{
    //recursively delete each node
    if(n->m_leftChild != nullptr)
        deleteNode(n->m_leftChild);
    if(n->m_rightChild != nullptr)
        deleteNode(n->m_rightChild);
    
    delete n;
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap()
{
    clear();
}