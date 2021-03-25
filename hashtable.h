//
// Created by taichong on 2021/3/18.
//

#ifndef HASHTABLE_HASHTABLE_H
#define HASHTABLE_HASHTABLE_H

template<typename Key, typename HashTableEntity, typename Hasher, typename Grower>
struct HashTable {
    int size;
    Grower grower;
    HashTableEntity& entities;
    int entities_raw;


};

class hashtable {
public:
    template<typename Key, typename HashTableEntity, typename Hasher, typename Grower>
    hashtable(Key, HashTableEntity, Hasher, Grower) {

    };
    static int hash_table(const int &num);

    ~hashtable() {}
private:

};


#endif //HASHTABLE_HASHTABLE_H
