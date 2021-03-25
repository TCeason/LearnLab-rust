//
// Created by taichong on 2021/3/18.
//

#include "hashtable.h"
#include <iostream>
#include <map>

using namespace std;

/*int hashtable::hash_table(int &num) {
non-const lvalue reference to type 'int' cannot bind to a temporary of type 'int'*/
int hashtable::hash_table(const int &num) {
    cout<< &num << endl;
    clock_t startTime, endTime;
    startTime = clock();

    for (int i = 0; i < num ; i++) {
        map<int, int> hashtable;
        hashtable.insert(make_pair(i,i));
    }

    endTime = clock();

    cout << "C++ RBtree Hash Map insert " << num << " elements elapsed Time: " << static_cast<double>(endTime-startTime) / CLOCKS_PER_SEC << " Seconds" << endl;
    return 0;
}