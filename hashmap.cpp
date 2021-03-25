//
// Created by taichong on 2021/3/18.
//

#include "hashmap.h"
#include <iostream>
#include <map>
#include <unordered_map>

using namespace std;

void hashmap::hash_map(int i) {
    clock_t t, e;
    t = clock();
    unordered_map<int, int> hash_map;
    for (int nums = 0; nums < i; nums++){
        hash_map.insert(make_pair(nums, nums));
    }
    e = clock();
    cout << "C++ Hash Map insert " << i << " elements elapsed Time: " << static_cast<double>(e-t) / CLOCKS_PER_SEC << " Seconds" << endl;
}
