//
// Created by taichong on 2021/7/7.
//

#ifndef chainNode_
#define chainNode_

template <class T>
struct chainNode
{
    // data numbers
    T element;
    chainNode<T> *next;

    // methods
    chainNode() {}
    chainNode(const T & element)
    {
        this->element = element;
    }
    chainNode(const T & element, chainNode<T>* next)
    {
        this->element = element;
        this->next = next;
    }

};


#endif //chainNode_
