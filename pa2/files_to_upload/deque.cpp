/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

/* Constructor for the Deque class */
template <class T>
Deque<T>::Deque(){

/* your code here */
  k1 = 0;
  k2 = 0;
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T const& newItem)
{
    /**
     * @todo Your code here!
     */
     data.push_back(newItem);
     k2++;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes.
 *
 * @return The leftmost item of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    /**
     * @todo Your code here!
     */
     T res = data.at(k1);
     k1++;

     // check if empty block exists on the left which can fit the data on the right
     // if so, resize
    //  if(k1 >= k2 - k1){
    if (data.size()/2 >= (k2-k1)) {
       vector<T> nData;
       for(int i = k1; i<k2; i++){
         nData.push_back(data.at(i));
       }
       data = nData;
       k2 = k2 - k1;
       k1 = 0;
     }
     return res;
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The rightmost item of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    /**
     * @todo Your code here!
     */
     T res = data.at(k2-1);
     k2--;
     data.pop_back();

     if (data.size()/2 >= (k2-k1)) {
       vector<T> nData;
       for(int i = k1; i<k2; i++){
         nData.push_back(data.at(i));
       }
       data = nData;
       k2 = k2 - k1;
       k1 = 0;
     }
     
     return res;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the left of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    /**
     * @todo Your code here!
     */
     return data.at(k1);
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    /**
     * @todo Your code here!
     */
     return data.at(k2-1);
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    /**
     * @todo Your code here!
     */
     // cout << __LINE__ << endl;
     // cout << k2 << " " << k1 << endl;
     if(k2-k1==0) return true;
     return false;
}
