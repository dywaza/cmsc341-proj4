#ifndef MEDIANHEAP_H
#define MEDIANHEAP_H
//#include "Heap.h"
#include <iostream>
using namespace std;

template <typename T> class Heap
{
    public:

        //what methods should a Heap have?

        /*
        List of things a heap can do, assuming minHeap or maxHeap possible:

        Basic stuff:
        - Create the heap structure. (should likely be array generation.)
        (Make it MedianHeap.size/2 +1, to allow for restructuring.)
        - Delete everything in the heap (dtor)
        - Copy constructor??
        - Be able to set a Heap equal to another heap

        Public elements:
        - Add an element
        - Find and remove an element
        - Return the root of the heap
        - Return the current size of the heap
        - Return the capacity of the heap array (MedianHeap.size/2)
        - Return the "extrema" of the heap (biggest element in
        maxheap, smallest in minheap (is in the leaves.)

        Private elements:
        - Find a new extrema in the heap during insertion/removal

        Notice: Find a way to differentiate the real capacity
        (MedianHeap.size/2 + 1) from the "stated" capacity (MedianHeap.size/2).
        Ah, this is confusing.
        */

        Heap();

        Heap(int cap, bool (*cmp) (const T&, const T&));

        ~Heap();

        //copy constructor if we care enough

        const Heap<T>& operator=(const Heap<T>& rhs) ;

        void insert (const T& item);

        bool deleteItem(T& givenItem, bool (*eq) (const T&, const T&)) ;

        void deleteRoot();

        int size ();

        int capacity();

        T getRoot(); //useful for getting MEDIAN

        T getExtrema (); //gets min or max elem depending on bias

        T get (int posIn);

        void dump ();

        void bubbleUp(int where) ;

        void trickleDown(int where) ;

        bool (*f_compare)(const T&, const T& ) ;

        int kapacity;

        int MH_kapacity;

        int currSize;

        T* heapray; //portmanteau between heap and array. Heapray!

        T extrema;
};


template <typename T> class MedianHeap
{
    public:

        MedianHeap();

        MedianHeap(bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&), int cap=100 ) ;

        MedianHeap(const MedianHeap<T>& otherH) ;

        ~MedianHeap() ;

        const MedianHeap<T>& operator=(const MedianHeap<T>& rhs) ;

        int size() ;

        int capacity() ;

        void insert(const T& item) ;

        T getMedian() ;

        T getMin() ;

        T getMax() ;

        int maxHeapSize() ;

        int minHeapSize() ;

        T locateInMaxHeap(int pos) ;

        T locateInMinHeap(int pos) ;

        bool deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&) ) ;

        //void rebalance () ;
        //if one heap has more nodes than the other, by >1, then remove the
        //root of the bigger heap and put it in the smaller.
        //check with an "if" for after every insert or delete.

        void dump() ;


    private:

        //to the best of my knowledge, these methods will be
        //defined by the individual main.cpp files.
        //call them in comparisons, but don't try to dissect them.
        bool (*f_lessThan)(const T&, const T& ) ;

        bool (*f_greaterThan)(const T&, const T&);

        int kapacity;

        Heap<T>* minHeap;
        Heap<T>* maxHeap;
};

//"If you can't stand the heap, stay out of the kitchen."
///-----------------<END OF .H, BEGIN .CPP>------------------------------

//default constructor, do not use
template <typename T>
MedianHeap<T>::MedianHeap()
{
    //please do not use. i beg of you.
}


//the real constructor
template <typename T>
MedianHeap<T>::MedianHeap(bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&), int cap)
{

    f_lessThan = lt;
    f_greaterThan = gt;

    kapacity = cap;

    minHeap = new Heap<T>(kapacity, f_greaterThan);
    maxHeap = new Heap<T>(kapacity, f_lessThan);
}

//copy constructor
template <typename T>
MedianHeap<T>::MedianHeap(const MedianHeap<T>& otherH)
{
    //i don't like this.
    f_lessThan = otherH.f_lessThan;
    f_greaterThan = otherH.f_greaterThan;
    kapacity = otherH.kapacity;
    minHeap = otherH.minHeap;
    maxHeap = otherH.maxHeap;
}

//destructor
template <typename T>
MedianHeap<T>::~MedianHeap()
{
    delete maxHeap;
    delete minHeap;
}

//overwritten assignment operator ["="]
template <typename T>
const MedianHeap<T>& MedianHeap<T>::operator=(const MedianHeap<T>& rhs)
{
    /*
    Allows for two medianheaps to be set equal to each other. I assume that
    this copies all the constituent elements (Heap copying thru their own
    copy methods, variables, w/e in ctor) over to this object.
    */
    f_lessThan = rhs.f_lessThan;
    f_greaterThan = rhs.f_greaterThan;
    kapacity = rhs.kapacity;
    minHeap = rhs.minHeap;
    maxHeap = rhs.maxHeap;
    return *this;

}

//this represents how many items are in all heaps
template <typename T>
int MedianHeap<T>::size ()
{
    return (minHeap->size() + maxHeap->size());
}

//
template <typename T>
int MedianHeap<T>::capacity ()
{
    /*
    Returns the capacity of the total MedianHeap.
    This is ultimately just the array size of both heaps
    added together.

    NOTE: Because of [0] index, kapacity should be 2 less
    than this previously described value. It is still
    both Heap::kapacity added, however.
    */

    return kapacity;
}

//
template <typename T>
void MedianHeap<T>::insert(const T& item)
{
    /*
    This one's a little harder.
    Because we're keeping track of the medians
    of our data set through the two heaps,
    we just need to determine whether the T& item
    is bigger than the median or smaller.
    (Doesn't matter which heap the median is based
     on.) If bigger, throw on MinHeap. If smaller,
     throw on the MaxHeap. If it's the SAME as the
     median, then, well... huh. That won't happen.

     Also, how do we detect imbalances? Hmm... well,
     After every insert, compare the sizes of the two
     heaps. If one of them is bigger than the other, take
     the root of the bigger heap and call remove(root) on it.
     Then, add the root T back to the smaller heap. Now
     they're the same size, and the median is hopefully
     updated.
     */

     if (size() > capacity())
     {
        throw;
     }

     if (size() == 0)
     {
        maxHeap->insert(item);
     }
     else if (f_lessThan(item,getMedian()))
     {
        maxHeap->insert(item);
     }
     else {
        minHeap->insert(item);
     }

     if (minHeap->size()-maxHeap->size() >= 2 || maxHeap->size()-minHeap->size() >= 2)
     {
         if (minHeap->size() > maxHeap->size())
         {
             T carryOver = minHeap->getRoot();
             minHeap->deleteRoot(); //PASS THE EQ FUNCITON DAVE
             maxHeap->insert(carryOver);
         }
         else { //minHeap->size() < maxHeap->size()
            T carryOver = maxHeap->getRoot();
            maxHeap->deleteRoot(); //PASS THE EQ DUDE
            minHeap->insert(carryOver);
         }
     }
}

//
template <typename T>
T MedianHeap<T>::getMedian()
{
    //i presume we keep track of median on this level.
    //thus, return the root of the heap we want to take
    //the median off of. doesn't matter which...
    /*
    Note: if for some reason the default heap to get from
    is empty, get the median from the other heap.
    Default working heap is maxHeap.
    */

    if (maxHeap->size() == 0)
    {
        if (minHeap->size() == 0)
        {
            cerr << "ERR: Cannot get median when size == 0";
            throw;
        }
        return minHeap->getRoot();
    }
    //return maxHeap->getRoot();

    if (maxHeap->size() > minHeap->size())
        return maxHeap->getRoot();
    else if (maxHeap->size() < minHeap->size())
        return minHeap->getRoot();
    else
        return maxHeap->getRoot();
}

//
template <typename T>
T MedianHeap<T>::getMin()
{
    /*
    I feel like we should keep track to min and max
    with separate variables somehow?
    Here's an idea: in each heap, keep a data value called
    "extreme". This is the biggest item in the minHeap, but also the
    smallest item in the maxHeap.
    If the size of the heap is greater than one, return the
    extreme value of maxHeap. If size is one, return the extreme
    value of the heap that has the element. (Because it's the only
    element, it will be both the min and max regardless.)
    If size is zero, I guess return void? idk.
    */
    if (maxHeap->size() == 0)
    {
        if (minHeap->size() == 0)
        {
            cerr << "ERR: Cannot retrieve min if size == 0";
            throw;
        }
        return minHeap->getExtrema();
    }
    return maxHeap->getExtrema();
}

//
template <typename T>
T MedianHeap<T>::getMax()
{
    //See above for details.
    if (minHeap->size() == 0)
    {
        if (maxHeap->size() == 0)
        {
            cerr << "ERR: Cannot retrieve max if size == 0";
            throw;
        }
        return maxHeap->getExtrema();
    }
    return minHeap->getExtrema();
}

//
template <typename T>
int MedianHeap<T>::maxHeapSize()
{
    return maxHeap->currSize;
}

//
template <typename T>
int MedianHeap<T>::minHeapSize()
{
    return minHeap->currSize;
}

//
template <typename T>
T MedianHeap<T>::locateInMaxHeap(int pos)
{
    return maxHeap->get(pos);
}

//
template <typename T>
T MedianHeap<T>::locateInMinHeap(int pos)
{
    return minHeap->get(pos);
}

//
template <typename T>
bool MedianHeap<T>::deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&) )
{
    /*
    This may be by far the hardest method to implement.
    Note the "equalTo" function.

    Idea:
    If givenItem is larger than the median, do delete on the minHeap.
    If smaller, do delete on the maxHeap. (Since this is the heap that
    the element would be on.) If it IS the median, then go to whatever
    heap the median is currently on. (Probably maxHeap, but watch out.)

    Delete should find the element in the heap array, remove it, and
    reorder the tree by replacing its position with the extreme of the set.
    Then, run a "findExtreme" method to set the new extreme position, if
    the removed item was the extreme.
    */

    if (f_lessThan(givenItem, getMedian()))
    {
        return minHeap->deleteItem(givenItem, equalTo);
    }
    else {
        return maxHeap->deleteItem(givenItem, equalTo);
    }

}

//
template <typename T>
void MedianHeap<T>::dump()
{
    //DUMP
    cout << "---- MedianHeap.dump() BEGIN HERE ----     " << endl;
    cout << "Reported capacity: " << kapacity << endl;
    cout << endl;
    cout << "---------- MAXHEAP heap ----------" << endl;
    maxHeap->dump();
    cout << "---------- MINHEAP heap ----------" << endl;
    minHeap->dump();

    cout << "min:   = " << getMin() << endl;
    cout << "max:   = " << getMax() << endl;
    cout << "median = " << getMedian() << endl;
    cout << endl;
    cout << "==== END MEDIANHEAP.DUMP() ====             " << endl;
}

//
template <typename T>
Heap<T>::Heap()
{
    cout << "This should never appear." << endl;
    cout << "Except for in the source code. Duh." << endl;
}

//
template <typename T>
Heap<T>::Heap(int cap, bool (*cmp) (const T&, const T&))
{
    //MH_kapacity = cap;
    kapacity = cap/2 + 2; //edit if needed
    f_compare = cmp;
    //bias = h;

    currSize = 0;
    heapray = new T [kapacity+1];

    /*
    for (int c = 0; c < kapacity+1 ; c++)
        heapray[c] = NULL;
    */
}

//
template <typename T>
const Heap<T>& Heap<T>::operator=(const Heap<T>& rhs)
{
    kapacity = rhs.kapacity;
    f_compare = rhs.f_compare;
    currSize = rhs.currSize;
    heapray = rhs.heapray;
}

//
template <typename T>
Heap<T>::~Heap()
{
    //delete &heapray;
    delete [] heapray;
    //delete heapray;
}

//
template <typename T>
void Heap<T>::insert (const T& item)
{
    //do an insert
    /*
    PROCESS OF AN INSERT:
    (ie: "what should this do?")

    - Because the type of heap is predetermined, methods may have to
    work differently depending on which side of the median we are on.
    - If <some value less than capacity >= size>: do nothing. (full)
    - Insert the value at the currently pointed "last" element+1 (which is equal
    to the size+1, I believe. No search required?)
    - If this element is (greater than/less than) the extrema, or is the first
    element added, then set it as the new extrema. (Note that this will always
    stay in the leaves of the heap.)
    - Now, we have to check to see if the new node will "trickle up".
    - Run a method "trickle up", which recursively does the following:
        - if (nodeIndex/2 # nodeIndex) && nodeIndex != 1 //# is < for maxHeap, > for min.
            swap the values of the two items
            call "trickle up" on nodeIndex/2
    - I think we've sufficiently inserted by this point? Hm.
    */
    if (currSize >= kapacity)
    {
        cout << "Error: Cannot insert T, as HEAP is full!" << endl;
    }
    else {
        //ADD THE ELEMENT
        //JEGUS WHAT IS THIS LEVEL OF INTENSITY
        currSize++;
        heapray[currSize] = item;

        //if (bias == HTYPE::MAXHEAP && (f_lessThan(item,extrema) || currSize == 1))
        if (currSize == 1)
        {
            extrema = item; //is overloaded = automatically here? check later.
        }
        else if (f_compare(item,extrema))
        {
            extrema = item;
        }
        //if (bias == HTYPE::MINHEAP && (f_greaterThan(item,extrema) || currSize == 1))
        //{
        //    extrema = item;
        //}

        bubbleUp(currSize);
    }
}

//
template <typename T>
void Heap<T>::bubbleUp(int where)
{
    //only call this from Heap.insert()

    //if (bias == HTYPE::MAXHEAP)
    //{
        //if(f_lessThan(heapray[where/2],heapray[where]) && where != 1 )
        if (where != 1)
        {
            if (f_compare(heapray[where/2], heapray[where]))
            {
                T temp = heapray[where/2];
                heapray[where/2] = heapray[where];
                heapray[where] = temp;
                bubbleUp(where/2);
            }
        }
    //}
    /*if (bias == HTYPE::MINHEAP)
    {
        if (f_greaterThan(heapray[where/2],heapray[where]) && where != 1)
        {
            T temp = heapray[where/2];
            heapray[where/2] = heapray[where];
            heapray[where] = temp;
            bubbleUp(where/2);
        }
    }
    */
}

//
template <typename T>
void Heap<T>::trickleDown(int where)
{
    /*
        Pseudocode:

        if (children are NOT out-of-bounds (2n > currSize && 2n+1 > currSize))
        {
            if (current node is SMALLER(maxheap)/LARGER(minheap) than leftChild OR rightChild)
            {
                Switch the value of current node with its SMALLEST/LARGEST child;
                    if left child, call bubbleUp(where*2)
                    if right child, call bubbleUp(where*2+1)
            }
        }
    */
        if (!(where*2 > currSize) && !(where*2+1 > currSize))
        {
            if (f_compare(heapray[where], heapray[where*2]) || f_compare(heapray[where], heapray[where*2+1]))
            {
                T temp = heapray[where];
                if(f_compare(heapray[where*2+1], heapray[where*2]))
                {
                    heapray[where] = heapray[where*2];
                    heapray[where*2] = temp;
                    trickleDown(where*2);
                }
                else {
                    heapray[where] = heapray[where*2+1];
                    heapray[where*2+1] = temp;
                    trickleDown(where*2+1);
                }
            }
        }
}

//since the root is always in the same place, no point in searching for it.
template <typename T>
void Heap<T>::deleteRoot()
{
    heapray[1] = heapray[currSize]; //last node in heap
    //heapray[currSize] = NULL;

    currSize--;
    trickleDown(1);

}

//
template <typename T>
bool Heap<T>::deleteItem(T& givenItem, bool (*eq) (const T&, const T&))
{
    /*
    So then how are we expected to do delete.
    I guess we'll just fill in from here.

    Yippie, now we're actually guaranteed an equals function.
    Alright, so this is how this is going to work:

    - Take the eq, set to some dumb function pointer (or not, don't care.)
    - Look through the ENTIRE heapray, comparing each element with the givenItem
    using the function.
    - If found, break out of the loop and save the element position in array.
    - Replace this position with the last element (currSize), and decrement currSize.
    - Call trickleDown on the saved position
    - Return true!
    - If you don't find something by the end, then breakout = false, so return false.
    */

    if (currSize == 0)
        return false;

    int checking = 1;
    bool breakout = false;
    bool findNewExtrema = false;
    while (checking <= currSize && breakout == false)
    {
        breakout = eq(givenItem, heapray[checking]);
        checking++;
    }

    if (breakout == false)
    {
        return false;
    }
    else {
        checking--;

        if (eq(heapray[checking],extrema))
        {
            findNewExtrema = true;
        }

        heapray[checking] = heapray[currSize];
        currSize--;

        trickleDown(checking);

        if (findNewExtrema == true)
        {
            extrema = heapray[1];
            for (int count = 1; count <= currSize; count++ )
            {
                if (f_compare(heapray[count], extrema))
                {
                    extrema = heapray[count];
                }
            }
        }

        return true;
    }


}

//
template <typename T>
int Heap<T>::size()
{
    return currSize;
}

//
template <typename T>
int Heap<T>::capacity()
{
    return kapacity;
}

//
template <typename T>
T Heap<T>::getRoot()
{
    if (currSize <= 0)
    {
        cerr << "ERR: Cannot get root: size == 0";
        throw;
    }
    else{
        return heapray[1];
    }
}

//
template <typename T>
T Heap<T>::get(int posIn)
{
    if ((posIn > currSize) || (posIn < 1))
    {
        throw;
    }
    else {
        return heapray[posIn];
    }
}

//
template <typename T>
T Heap<T>::getExtrema()
{
    //I expect this to be ridiculous.
    return extrema;
}

//
template <typename T>
void Heap<T>::dump()
{
    //Does each T have its own specified cout function? huh.

    cout << "size = " << currSize << ", capacity = " << kapacity << endl;

    for (int count = 1; count <= currSize ; count++)
        cout << "Heap[" << count << "] = " << heapray[count] << endl;

    cout << endl;

}


#endif // HEAP_H

