//
// Created by taichong on 2021/7/8.
//

#ifndef derivedArrayStack_
#define derivedArrayStack_

#include <iostream>
#include "stack.h"
#include "../List/arraylist.h"
#include "../List/myExceptions.h"

//using namespace std;

template<class T>
class derivedArrayStack : public stack<T>, private arraylist<T>
{
public:
    derivedArrayStack(int initialCapacity = 10)
    : arraylist<T>(initialCapacity)
    {

    }

    bool empty() const
    {
        return arraylist<T>::empty();
    }

    int size() const
    {
        return arraylist<T>::size();
    }

    T top()
    {
        if (arraylist<T>::empty())
            throw stackEmpty();
        return this->get(arraylist<T>::size() - 1);
    }

    void pop()
    {
        if (arraylist<T>::empty())
            throw stackEmpty();
        this->erase(arraylist<T>::size() - 1);
    }

    void push(const T & theElement)
    {
        this->insert(arraylist<T>::size(), theElement);
    }
};


#endif //derivedArrayStack_
