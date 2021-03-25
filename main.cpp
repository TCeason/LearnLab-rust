//
// Created by taichong on 2021/3/18.
//

#include "hashmap.h"
#include "hashbtree.h"
#include "hashtable.h"



#define N 10000000

//using namespace std;

int main() {
    hashmap::hash_map(N);

    hashbtree rbtree(N);
    rbtree.hash_rbtree();

    //hashtable::hash_table(N);
    const int n = N;
    hashtable::hash_table(n);

    return 0;
}

