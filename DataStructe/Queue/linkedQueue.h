//
// Created by taichong on 2021/7/9.
//

#ifndef linkedQueue_
#define linkedQueue_

#include "queue.h"
#include "../List/chainNode.h"
#include "../List/myExceptions.h"
#include <sstream>

using namespace std;

template<class T>
class linkedQueue : public queue<T>
{
public:
    linkedQueue(int initialCapacity = 10)
    {
        queueFront = NULL;
        queueSize = 0;
    }
    ~linkedQueue();
    bool empty() const
    {
        return queueSize == 0;
    }

    int size() const
    {
        return queueSize;
    }

    T& front()
    {
        if (queueSize == 0)
            throw queueEmpty();
        return queueFront->element;
    }

    T& back()
    {
        if (queueSize == 0)
            throw queueEmpty();

        return queueBack->element;
    }

    void pop();

    void push(const T & theElement);
private:
    chainNode<T>* queueFront;
    chainNode<T>* queueBack;
    int queueSize;
};

template<class T>
linkedQueue<T>::~linkedQueue<T>()
{
    while (queueFront != NULL)
    {
        chainNode<T>* nextNode = queueFront->next;
        delete queueFront;
        queueFront = nextNode;
    }
}

template<class T>
void linkedQueue<T>::pop()
{
    if (queueFront == NULL)
        throw queueEmpty();

    chainNode<T>* nextNode = queueFront->next;
    delete queueFront;
    queueFront = nextNode;
    queueSize--;
}

template<class T>
void linkedQueue<T>::push(const T &theElement)
{
    chainNode<T>* newNode = new chainNode<T>(theElement, NULL);

    if (queueSize == 0)
        queueFront = newNode;
    else
        queueBack->next = newNode;
    queueBack = newNode;
    queueSize++;
}
#endif //linkedQueue_
