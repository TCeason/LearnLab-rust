//
// Created by taichong on 2021/8/29.
//

#include "heap.h"

int main() {
    heap<int> bigheap;
    bigheap.push(4);
    bigheap.push(3);
    bigheap.push(2);
    bigheap.push(1);
    bigheap.push(5);
    bigheap.print();
    bigheap.pop();
    bigheap.print();
    bigheap.pop();
    bigheap.print();

    return 0;
}
