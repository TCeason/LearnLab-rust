//
// Created by taichong on 2021/3/18.
//

#include "hashbtree.h"
#include <iostream>
#include <map>

using namespace std;


void hashbtree::hash_rbtree()
{
    clock_t startTime, endTime;
    startTime = clock();

    map<int, int> rbtree_map;
    for (int i = 0; i < this->num; i++){
        rbtree_map.insert(make_pair(i,i));
    }
    endTime = clock();

    cout << "C++ RBtree Hash Map insert " << num << " elements elapsed Time: " << static_cast<double>(endTime-startTime) / CLOCKS_PER_SEC << " Seconds" << endl;
}