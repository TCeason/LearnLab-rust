//
// Created by taichong on 2021/7/8.
//

#ifndef stack_
#define stack_

template<class T>
class stack
{
public:
    virtual ~stack()
    {

    };

    virtual bool empty() const = 0;

    virtual int size() const = 0;

    virtual int top() = 0;

    // remove the top element;
    virtual void pop() = 0;

    virtual void push(const T & theElement) = 0;
};
#endif //stack_
