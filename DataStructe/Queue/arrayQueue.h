//
// Created by taichong on 2021/7/9.
//

#ifndef arrayQueue_
#define arrayQueue_

#include "queue.h"
#include "../List/myExceptions.h"
#include <sstream>

using namespace std;

template<class T>
class arrayQueue : public queue<T>
{
public:
    arrayQueue(int initialCapacity);
    ~arrayQueue()
    {
        delete [] queue;
    }

    bool empty() const
    {
        return theFront == theBack;
    }

    int size() const
    {
        return (theBack - theFront + arrayLength) % arrayLength;
    }

    T& front()
    {
        if (theFront == theBack)
            throw queueEmpty();
        return queue[(theFront + 1) % arrayLength];
    }

    T& back()
    {
        if (theFront == theBack)
            throw queueEmpty();
        return queue[theBack];
    }

    void pop()
    {
        if (theFront == theBack)
            throw queueEmpty();
        theFront = (theFront + 1) % arrayLength;
        queue[theFront].~T();
    }

    void push(const T & theElement);
private:
    int theFront;
    int theBack;
    int arrayLength;
    T* queue;
};

template<class T>
arrayQueue<T>::arrayQueue(int initialCapacity)
{
    if (initialCapacity < 1)
    {
        ostringstream s;
        s << "Initial capacity = " << initialCapacity << " Must be > 0";
        throw illegalParameterValue(s.str());
    }

    arrayLength = initialCapacity;
    queue = new T[arrayLength];
    theFront = 0;
    theBack = 0;
}

template<class T>
void arrayQueue<T>::push(const T & theElement)
{
    if ((theBack + 1) % arrayLength == theFront)
    {
        T* newQueue = new T[2 * arrayLength];

        // copy elements into new array
        int start = (theFront + 1) % arrayLength;
        if (start < 2)
            copy(queue + start, queue + start + arrayLength - 1, newQueue);
        else
        {
            copy(queue + start, queue + arrayLength, newQueue);
            copy(queue, queue + theBack + 1, newQueue + arrayLength - 1);
        }

        theFront = 2 * arrayLength - 1;
        theBack = arrayLength - 2;
        arrayLength *= 2;
        queue = newQueue;
    }

    theBack = (theBack + 1) % arrayLength;
    queue[theBack] = theElement;
}

#endif //arrayQueue_
