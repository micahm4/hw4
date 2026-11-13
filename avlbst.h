#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateL(AVLNode<Key, Value>* node);
    void rotateR(AVLNode<Key, Value>* node);
    void fixInsert(AVLNode<Key, Value>* node, int8_t diff);
    void fixRemove(AVLNode<Key, Value>* node, int8_t diff);


};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateL(AVLNode<Key, Value>* node) {
  AVLNode<Key, Value>* rightChild = node->getRight();

  if (rightChild == nullptr) { //none
    return;
  }

  AVLNode<Key, Value>* parent = node->getParent();

  //move left to right subtree
  node->setRight(rightChild->getLeft());

  if (rightChild->getLeft() != nullptr) {
    rightChild->getLeft()->setParent(node); //the parent the right node
  }

  rightChild->setParent(parent);

  if (parent == nullptr) {
    this->root_ = rightChild;
  }
  else if (node == parent->getLeft()) {
    parent->setLeft(rightChild);
  }
  else {
    parent->setRight(rightChild);
  }

  rightChild->setLeft(node);
  node->setParent(rightChild);

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateR(AVLNode<Key, Value>* node) {
  AVLNode<Key, Value>* leftChild = node->getLeft();

  if (leftChild == nullptr) { //none
    return;
  }

  AVLNode<Key, Value>* parent = node->getParent();

  //move right to left subtree
  node->setLeft(leftChild->getRight());

  if (leftChild->getRight() != nullptr) {
    leftChild->getRight()->setParent(node); //the parent the right node
  }

  leftChild->setParent(parent);

  if (parent == nullptr) {
    this->root_ = leftChild;
  }
  else if (node == parent->getLeft()) {
    parent->setLeft(leftChild);
  }
  else {
    parent->setRight(leftChild);
  }

  leftChild->setRight(node);
  node->setParent(leftChild);

}

template<class Key, class Value>
void AVLTree<Key, Value>::fixInsert(AVLNode<Key, Value>* node, int8_t diff) {
  
  node->updateBalance(diff);
  int8_t balance = node->getBalance();

  if (balance == 0){ //already balanced
    return;
  }

  if (balance == -1 || balance == 1) {
    AVLNode<Key, Value>* parent = node->getParent();

    if (parent == nullptr) {
      return;
    }

    if (node == parent->getLeft()) { //use helper function
      fixInsert(parent, -1);
    }
    else {
      fixInsert(parent,1);
    }

    return;

  }

  if (balance == -2) { //case 1
    AVLNode<Key, Value>* left = node->getLeft();

    if (left->getBalance() <= 0) {
      rotateR(node);

      node->setBalance(0);
      left->setBalance(0);
    }
    else {
      AVLNode<Key, Value>* middle = left->getRight();
      int8_t midBalance = middle->getBalance();

      rotateL(left);
      rotateR(node);

      if (midBalance == -1) {
        node->setBalance(1);
        left->setBalance(0);
      }
      else if (midBalance == 0) {
        node->setBalance(0);
        left->setBalance(0);
      }
      else {
        node->setBalance(0);
        left->setBalance(-1);
      }

      middle->setBalance(0);

    }
  }


  if (balance == 2) { //case 2
    
    AVLNode<Key, Value>* right = node->getRight();

    if (right->getBalance() >= 0) { //right right
      rotateL(node);

      node->setBalance(0);
      right->setBalance(0);
    }

    else { //right left
      AVLNode<Key, Value>* middle = right->getLeft();
      int8_t midBalance = middle->getBalance();

      rotateR(right);
      rotateL(node);

      if (midBalance == 1) {
        node->setBalance(-1);
        right->setBalance(0);
      }
      else if (midBalance == 0) {
        node->setBalance(0);
        right->setBalance(0);
      }
      else {
        node->setBalance(0);
        right->setBalance(1);
      }

      middle->setBalance(0);
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::fixRemove(AVLNode<Key, Value>* node, int8_t diff) {
  
  node->updateBalance(diff);

  int8_t balance = node->getBalance();

  //no change
  if (balance == -1 || balance == 1) {
    return;
  }

  if (balance == 0) {
    AVLNode<Key, Value>* parent = node->getParent();

    if (parent == nullptr) {
      return;
    }

    if (node == parent->getLeft()) {
      fixRemove(parent, 1);
    }
    else {
      fixRemove(parent, -1);
    }

    return;

  }

  //left heavy
  if (balance == -2) {
    AVLNode<Key, Value>* left = node->getLeft();

    if (left->getBalance() <= 0) {
      int8_t leftBalance = left->getBalance();

      rotateR(node);

      if(leftBalance == 0) {
        node->setBalance(-1);
        left->setBalance(1);
        return;
      }

      node->setBalance(0); //restart
      left->setBalance(0);
    }
    else {
      AVLNode<Key, Value>* middle = left->getRight();

      int8_t midBalance = middle->getBalance();

      rotateL(left);
      rotateR(node);

      if (midBalance == -1) {
        node->setBalance(1);
        left->setBalance(0);
      }
      else if (midBalance == 0) {
        node->setBalance(0);
        left->setBalance(0);
      }
      else {
        node->setBalance(0);
        left->setBalance(-1);
      }

      middle->setBalance(0);
    }
  }

  //right heavy 

  else if (balance == 2) {
    AVLNode<Key, Value>* right = node->getRight();

    if (right->getBalance() >= 0) {
      int8_t rightBalance = right->getBalance();

      rotateL(node);

      if (rightBalance == 0) {
        node->setBalance(1);
        right->setBalance(-1);
        return;
      }

      node->setBalance(0);
      right->setBalance(0);

    }
    else {
      AVLNode<Key, Value>* middle = right->getLeft();
      int8_t midBalance = middle->getBalance();

      rotateR(right);
      rotateL(node);

      if (midBalance == 1) {
        node->setBalance(-1);
        right->setBalance(0);
      }
      else if (midBalance == 0) {
        node->setBalance(0);
        right->setBalance(0);
      }
      else {
        node->setBalance(0);
        right->setBalance(1);
      }

      middle->setBalance(0);
      
    }
  }

  //check if new parent needs update

  AVLNode<Key, Value>* newR = nullptr;

  if (balance == -2) {
    newR = node->getParent();
  }
  else if (balance == 2) {
    newR = node->getParent();
  }

  if (newR != nullptr) {
    AVLNode<Key, Value>* parent = newR->getParent();

    if (parent != nullptr) {
      if (newR == parent->getLeft()) {
        fixRemove(parent, 1);
      }
      else {
        fixRemove(parent, -1);
      }
    }
  }
}





/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);

    if (this->root_ == nullptr) { //none
      this->root_ = newNode;
      return;
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);

    AVLNode<Key, Value>* parent = nullptr;

    while (curr != nullptr) {
      parent = curr;

      if (new_item.first == curr->getKey()) {
        curr->setValue(new_item.second);
        delete newNode;
        return;
      }
      else if (new_item.first < curr->getKey()) {
        curr = curr->getLeft();
      }
      else {
        curr = curr->getRight();
      }
    }

    newNode->setParent(parent);

    if (new_item.first < parent->getKey()) {
      parent->setLeft(newNode);
      fixInsert(parent, -1);
    }
    else {
      parent->setRight(newNode);
      fixInsert(parent, 1);
    }

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);

    //find
    while (curr != nullptr && curr->getKey() != key) { //go through until key found
      if (key < curr->getKey()) {
        curr = curr->getLeft();
      }
      else{
        curr = curr->getRight();
      }
    }

    //not found

    if (curr == nullptr) {
      return;
    }

    if (curr->getLeft() != nullptr && curr->getRight() != nullptr) {
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(curr));

      nodeSwap(curr, pred);
    }

    AVLNode<Key, Value>* child = nullptr;

    if (curr->getLeft() == nullptr) {
      child = curr->getRight();
    }
    else {
      child = curr->getLeft();
    }

    AVLNode<Key, Value>* parent = curr->getParent();

    int8_t diff = 0;

    if (parent != nullptr) {
      if (parent->getLeft() == curr) {
        diff = 1;
      }
      else {
        diff = -1;
      }
    }

    if (parent == nullptr) {
      this->root_ = child;
    }
    else if (parent->getLeft() == curr) {
      parent->setLeft(child);
    }
    else {
      parent->setRight(child);
    }

    if (child != nullptr) {
      child->setParent(parent);
    }

    delete curr;

    if (parent != nullptr) {
      fixRemove(parent, diff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
