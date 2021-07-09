//
// Created by taichong on 2021/7/9.
//

#ifndef dictionary_
#define dictionary_

using namespace std;

template<class K, class E>
class dictionary
{
public:
    virtual ~dictionary()
    {

    }

    virtual bool empty() const = 0;

    // return number of pairs in dictionary
    virtual int size() const = 0;

    // return pointer to matching pair
    virtual pair<const K, E>* find(const K &) const = 0;

    // remove matching pair
    virtual void erase(const K &) = 0;

    // insert a (key, value)  pair into the dictionary
    virtual void insert(const pair<const K, E> &) = 0;
};
#endif //dictionary_
