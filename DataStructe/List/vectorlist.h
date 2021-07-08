//
// Created by taichong on 2021/7/8.
//

#ifndef vectorList_
#define vectorList_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include "linearlist.h"
#include "myExceptions.h"

using namespace std;

template<class T>
class vectorlist : public linearList<T>
{
public:
    vectorlist(int initialCapacity = 10);
    vectorlist(const vectorlist<T>&);
    ~vectorlist()
    {
        delete element;
    }

    // ADT method
    bool empty() const
    {
        return element->empty();
    }

    int size() const
    {
        return (int) element->size();
    }

    T& get(int theIndex) const;

    int indexOf(const T & theElement) const;

    void erase(int theIndex);

    void insert(int theIndex, const T & theElement);

    void output(ostream & out) const;

    int capacity() const
    {
        return (int) element->capacity();
    }

    // iterators to start and end of list
    typedef typename vector<T>::iterator iterator;

    iterator begin()
    {
        return element->begin();
    }

    iterator end()
    {
        return element->end();
    }
protected:
    void checkIndex(int theIndex) const;
    vector<T>* element;
};

template<class T>
vectorlist<T>::vectorlist(int initialCapacity)
{
    if (initialCapacity < 1)
    {
        ostringstream s;
        s << "Initial capacity = " << initialCapacity << " Must be > 0";
        throw illegalParameterValue(s.str());
    }

    // create an empty vector with capacity 0
    element = new vector<T>;
    // increase vector capacity from 0 to initialCapacity
    element->reserve(initialCapacity);
}

template<class T>
vectorlist<T>::vectorlist(const vectorlist<T> & theList)
{
    element = new vector<T>(*theList.element);
}

template<class T>
void vectorlist<T>::checkIndex(int theIndex) const
{
    if (theIndex < 0 || theIndex >= size())
    {
        ostringstream s;
        s << "index = " << theIndex << " size = " << size();
        throw illegalIndex(s.str());
    }
}

template<class T>
T& vectorlist<T>::get(int theIndex) const
{
    checkIndex(theIndex);
    return (*element)[theIndex];
}

template<class T>
int vectorlist<T>::indexOf(const T &theElement) const
{
    int theIndex = (int) (find(element->begin(), element->end(), theElement) - element->begin());
    if (theIndex == size())
        return -1;
    else
        return theIndex;
}

template<class T>
void vectorlist<T>::erase(int theIndex)
{
    checkIndex(theIndex);
    element->erase(begin() + theIndex);
}

template<class T>
void vectorlist<T>::insert(int theIndex, const T & theElement)
{
    if (theIndex < 0 || theIndex > size())
    {
        ostringstream s;
        s << "index = " << theIndex << " size = " << size();
        throw illegalIndex(s.str());
    }

    element->insert(element->begin() + theIndex, theElement);
}

template<class T>
void vectorlist<T>::output(ostream & out) const
{
    copy(element->begin(), element->end(), ostream_iterator<T>(out, " "));
}

template<class T>
ostream & operator<<(ostream & out, const vectorlist<T> & x)
{
    x.output(out);
    return out;
}
#endif //vectorList_
