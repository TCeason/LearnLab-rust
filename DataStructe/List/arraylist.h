//
// Created by taichong on 2021/7/7.
//

#ifndef arrayList_
#define arrayList_

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include "linerlist.h"
#include "changeLength1D.h"
#include "myExceptions.h"
#include <iterator>

using namespace std;

template<class T>
class arraylist : public linerList<T>
{
public:
    arraylist(int initialCapacity = 10);
    arraylist(const arraylist<T> &);
    ~arraylist()
    {
        delete [] element;
    }

    // ADT method
    bool empty() const
    {
        return listSize == 0;
    }

    int size() const
    {
        return listSize;
    }

    T& get(int theIndex) const;

    int indexOf(const T & theElement) const;
    void erase(int theIndex);
    void insert(int theIndex, const T & theElement);
    void output(ostream & out) const;

    // additional method
    int capacity() const
    {
        return arrayLength;
    }

protected:
    // throw illegalIndex if theIndex invalid
    void checkIndex(int theIndex) const;

    // 1D array to hold list elements
    T* element;

    // capacity of the 1D array
    int arrayLength;

    // number of elements in list
    int listSize;
};

// Constructor
template<class T>
arraylist<T>::arraylist(int initialCapacity)
{
    if (initialCapacity < 1)
    {
        ostringstream s;
        s << "Inital capacity = " << initialCapacity << " Must be > 0";
        throw illegalParameterValue(s.str());
    }
    arrayLength = initialCapacity;
    element = new T[arrayLength];
    listSize = 0;
}

// copy Constructor
template<class T>
arraylist<T>::arraylist(const arraylist<T> & theList)
{
    arrayLength = theList.arrayLength;
    listSize = theList.listSize;
    element = new T[arrayLength];
    copy(theList.element, theList.element + listSize, element);
}

// Verify that theIndex if between 0 and listSize - 1.
template<class T>
void arraylist<T>::checkIndex(int theIndex) const
{
    if (theIndex < 0 || theIndex >= listSize)
    {
        ostringstream s;
        s << "index = " << theIndex << " size = " << listSize;
        throw illegalParameterValue(s.str());
    }
}

template<class T>
T& arraylist<T>::get(int theIndex) const
{
    checkIndex(theIndex);
    return element[theIndex];
}

template<class T>
int arraylist<T>::indexOf(const T &theElement) const
{
    int theIndex = (int) (find(element, element + listSize, theElement) - element);

    if (theIndex == listSize)
        return -1;
    else
        return theIndex;
}

template<class T>
void arraylist<T>::erase(int theIndex)
{
    checkIndex(theIndex);

    copy(element + theIndex + 1, element + listSize, element + theIndex);
    element[--listSize].~T();
}

// Insert theElement so that its index is theIndex
template<class T>
void arraylist<T>::insert(int theIndex, const T & theElement)
{
    if (theIndex < 0 || theIndex > listSize)
    {
        ostringstream s;
        s << "index = " << theIndex << " size = " << listSize;
        throw illegalIndex(s.str());
    }

    if (listSize == arrayLength)
    {
        changeLength1D(element, arrayLength, 2 * arrayLength);
        arrayLength *= 2;
    }

    copy_backward(element + theIndex, element + listSize, element + listSize + 1);

    element[theIndex] = theElement;

    listSize++;
}

template<class T>
void arraylist<T>::output(ostream & out) const
{
    copy(element, element + listSize, ostream_iterator<T>(cout, " "));
}

template<class T>
ostream & operator<<(ostream & out, const arraylist<T> & x)
{
    x.output(out);
    return out;
}
#endif //arrayList_
