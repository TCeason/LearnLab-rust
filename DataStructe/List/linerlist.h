//
// Created by taichong on 2021/7/7.
//

#ifndef linerlist_
#define linerlist_
#include <iostream>

using namespace std;

template<class T>
class linerList
{
    public:
        virtual ~linerList() {};
        // return true if list is empty
        virtual bool empty() const = 0;

        // return nubmer of elements in list
        virtual int size() const = 0;

        // return element whose index is theIndex
        virtual T & get(int theIndex) const = 0;

        // return index of first occurence of theElement
        virtual int indexOf(const T & theElement) const = 0;

        // remove the element whose index is theIndex
        virtual void erase(int theIndex) = 0;

        // insert theElement so that its index is theIndex
        virtual void insert(int theIndex, const T & theElement) = 0;

        // insert list into stream out
        virtual void output(ostream & out) const = 0;
};
#endif //linerlist_
