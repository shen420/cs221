#include "chain.h"
#include "chain_given.cpp"

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain(){
  /* your code here */
  clear();
  delete head_;
}

/**
 * Inserts a new node at the end of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block & ndata){
  /* your code here */
  Node * n = new Node(ndata);
  Node * last = head_->prev;
  last->next = n;
  n->prev = last;
  n->next = head_;
  head_->prev = n;
  length_++;
}

/**
 * Modifies the Chain by moving the subchain of len Nodes,
 * starting at position startPos, dist positions toward the
 * end of the chain.  If startPos + len - 1 + dist > length
 * then dist = length - startPos - len + 1 (to prevent gaps
 * in the new chain).
 * The subchain occupies positions (startPos + dist) through
 * (startPos + dist + len - 1) of the resulting chain.
 * The order of subchain nodes is not changed in the move.
 * You may assume that: 1 <= startPos <= length - len + 1,
 * 0 <= dist <= length, and 0 <= len <= length.
 */
void Chain::moveBack(int startPos, int len, int dist){
  /* your code here */
  // Done
  if(startPos + len - 1 + dist > length_){
    dist = length_ - startPos - len - 1;
  }
  if(dist>0 && len>0){
    Node * firstSC = walk(head_, startPos);
    Node * lastSC = walk(firstSC, len-1);
    firstSC->prev->next = lastSC->next;
    lastSC->next->prev = firstSC->prev;
    Node * a = walk(lastSC, dist);
    lastSC->next = a->next;
    a->next->prev = lastSC;
    a->next = firstSC;
    firstSC->prev = a;
  }
}

/**
 * Rolls the current Chain by k nodes: reorders the current list
 * so that the first k nodes are the (n-k+1)th, (n-k+2)th, ... , nth
 * nodes of the original list followed by the 1st, 2nd, ..., (n-k)th
 * nodes of the original list where n is the length.
 */
void Chain::roll(int k){
  /* your code here */
  // Done
  if(k % length_==0) return;
  k = k % length_;
  Node * a = head_->next;
  Node * b = head_->prev;
  Node * last = walk(head_, length_ - k);
  Node * first = last->next;
  first->prev = head_;
  head_->next = first;
  last->next = head_;
  head_->prev = last;
  a->prev = b;
  b->next = a;
}

/**
 * Modifies the current chain by reversing the sequence
 * of nodes from pos1 up to and including pos2. You may
 * assume that pos1 and pos2 exist in the given chain,
 * and pos1 <= pos2.
 * The positions are 1-based.
 */
void Chain::reverseSub(int pos1, int pos2){
  /* your code here */
  Node * p1 = walk(head_, pos1);
  Node * p2 = walk(head_, pos2);
  Node * bef = p1->prev;
  Node * aft = p2->next;

  Node * curr = p1;
  while(curr!=p2){
    Node * aux = curr->next;
    curr->prev = curr->next;
    curr = aux;
  }
  // curr == p2 at this point
  curr = p1;
  while(curr!=p2){
    curr->prev->next = curr;
    curr = curr->prev;
  }
  bef->next = p2;
  p2->prev = bef;
  aft->prev = p1;
  p1->next = aft;
}

/*
* Modifies both the current chain and the "other" chain by removing
* nodes from the other chain and adding them between the nodes
* of the current chain -- one "other" node between two current nodes --
* until one of the two chains is exhausted.  Then the rest of the
* nodes in the non-empty chain follow.
* The length of the resulting chain should be the sum of the lengths
* of current and other. The other chain should have only
* the head_ sentinel at the end of the operation.
* The weave fails and the original
* chains should be unchanged if block sizes are different.
* In that case, the result of the function should be:
* cout << "Block sizes differ." << endl;
*/
void Chain::weave(Chain & other) { // leaves other empty.
  /* your code here */
  if(other.height_!=height_ || other.width_!=width_){
    cout << "Block sizes differ." << endl;
    return;
  }
  Node * curr = head_->next;
  Node * h2 = other.head_;
  while(other.length_>0){ // still nodes toInsert
    length_++;
    other.length_ -= 1;
    Node * aux = curr->next;
    Node * toInsert = h2->next;
    h2->next = toInsert->next;
    curr->next = toInsert;
    toInsert->next = aux;
    aux->prev = toInsert;
    toInsert->prev = curr;
    curr = curr->next;
    if(aux!=head_) curr = curr->next;
  }
}


/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class except for the sentinel head_. Sets length_
 * to zero.  After clear() the chain represents an empty chain.
 */
void Chain::clear() {
  /* your code here */
  Node * curr = head_->next;
  while(curr!=head_){
    Node * aux = curr;
    curr = curr->next;
    delete aux;
  }
  length_ = 0;
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const& other) {
  /* your code here */
  clear();
  height_ = other.height_;
  width_ = other.width_;
  Node * curr = other.head_->next;
  while(curr!=other.head_){
    insertBack(curr->data);
    curr = curr->next;
  }
}
