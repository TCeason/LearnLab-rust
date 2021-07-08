//
// Created by taichong on 2021/7/8.
//

#ifndef LEARNLAB_RUST_ARRAYSTACK_H
#define LEARNLAB_RUST_ARRAYSTACK_H

#include "stack.h"
#include "../List/myExceptions.h"
#include "../List/changeLength1D.h"
#include <sstream>

using namespace std;

template<class T>
class arrayStack : public stack<T>
{
public:
    arrayStack(int initialCapacity = 10);
    ~arrayStack()
    {
        delete [] stack;
    }

    // ADT method
    bool empty() const
    {
        return stackTop == -1;
    }

    int size() const
    {
        return stackTop + 1;
    }

    T& top()
    {
        if (stackTop == -1)
            throw stackEmpty();

        return stack[stackTop];
    }

    void pop()
    {
        if (stackTop == -1)
            throw stackEmpty();
        stack[stackTop--].~T();
    }

    void push(const T & theElement);

private:
    int stackTop;
    int arrayLength;
    T *stack;
};

template<class T>
arrayStack<T>::arrayStack(int initialCapacity)
{
    if (initialCapacity < 1)
    {
        ostringstream s;
        s << "Initial capacity = " << initialCapacity << " Must be > 0";
        throw illegalParameterValue(s.str());
    }

    arrayLength = initialCapacity;
    stack = new T[arrayLength];
    stackTop = -1;
}

template<class T>
void arrayStack<T>::push(const T &theElement)
{
    if (stackTop == arrayLength -1)
    {
        changeLength1D(stack, arrayLength, 2 * arrayLength);
        arrayLength *= 2;
    }
    stack[++stackTop] = theElement;
}
#endif //LEARNLAB_RUST_ARRAYSTACK_H
