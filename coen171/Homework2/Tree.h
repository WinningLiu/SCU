# ifndef TREE_H
# define TREE_H

template<class T>
class Node {
    T data;
    Node<T> *left;
    Node<T> *right;

public:
    Node(const T &);
    void insert(const T &);
    bool member(const T &) const;
};

template<class T>
class Tree {
    Node<T> *root;

public:
    Tree();
    void insert(const T &);
    bool member(const T &) const;
};


/* Your functions go here */
bool Node<T>::member(const T & anObject) const
{
    if (anObject < data)
    {
        if (left == NULL)
            return false;
        else 
            return left->member(anObject);
    }
    if (anObject > data)
    {
        if (right == NULL)
            return false;
        else 
            return right->member(anObject);
    }
    return true;

}
void Node<T>::insert(const T& anObject)
{
   if (anObject < data)
   {
        if (left == NULL)
            left = new Node(anObject)
        else 
            left->insert(anObject);
   }
   if (anObject > value)
   {
        if (right == NULL)
            left = new Node(anObject);
        else 
            right->insert(anObject);
   }
}
bool Tree<T>::member(const T& anObject) const
{
    if (root == NULL)
        return false;
    else 
        return root->member(anObject);
}
void Tree::insert(const T& anObject)
{
    if (root == NULL)
        root = new Node(anObject);
    else 
        root->insert(anObject)
}
# endif /* TREE_H */