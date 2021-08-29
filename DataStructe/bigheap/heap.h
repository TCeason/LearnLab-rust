#ifndef UNTITLED_HEAP_H
#define UNTITLED_HEAP_H

#include <vector>
#include <iostream>

using namespace std;

template<class T>
class heap {
public:
    heap()
    {
        _heap.push_back(0);
    }

    int size()
    {
        return _heap.size() - 1;
    }

    bool empty()
    {
        return size() == 0;
    }

    void push(T val)
    {
        _heap.push_back(val);
        auto index = size();

        while (index / 2 > 0 && _heap[index] > _heap[index / 2])
        {
            swap(_heap[index], _heap[index / 2]);
            index /= 2;
        }
    }

    void pop()
    {
        if (empty())
            return;

        int heap_size = size();
        swap(_heap[1], _heap[heap_size]);
        _heap.pop_back();
        heap_size--;

        int index = 1;
        while (true)
        {
            int maxI = index;

            if (2 * index <= heap_size && _heap[index] < _heap[2 * index])
                maxI = 2 * index;
            else if (2 * index + 1 < heap_size && _heap[maxI] < _heap[2 * index])
                maxI = 2 * index + 1;

            if (maxI == index)
                break;
            swap(_heap[index], _heap[maxI]);
            index = maxI;
        }
    }
    void print()
    {
        for (int i = 1; i < _heap.size(); i++)
        {
            cout << _heap[i] << " ";
        }
        cout << endl;
    }

private:
    vector<T> _heap;
};

#endif //UNTITLED_HEAP_H
