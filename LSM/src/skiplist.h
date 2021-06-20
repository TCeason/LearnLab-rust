//
// Created by taichong on 2021/6/20.
//

#ifndef LEARNLAB_RUST_SKIPLIST_H
#define LEARNLAB_RUST_SKIPLIST_H
#include <cstdint>
#include <cstdlib>
#include <random>
#include <string>
#include "run.h"

std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(0.0, 1.0);

const double NODE_PROBABILITY = 0.5;

namespace lsm {
    template<class K, class V, unsigned MAXLEVEL>
    class SkipListNode {
    public:
        const K key;
        V val;
        SkipListNode<K, V, MAXLEVEL> * _forward[MAXLEVEL + 1];

        SkipListNode() {
            for (int i = 1; i <= MAXLEVEL; i++) {
                _forward[i] = NULL;
            }
        }

        SkipListNode(K searchedKey):key(searchedKey) {
            for (int i = 1; i <= MAXLEVEL; i++) {
                _forward[i] = NULL;
            }
        }

        SkipListNode(K searchedKey, V val):key(searchedKey), val(val) {
            for (int i = 1; i <= MAXLEVEL; i++) {
                _forward[i] = NULL;
            }
        }

        virtual ~SkipListNode(){}
    };

    template<class K, class V, int MAXLEVEL = 16>
    class SkipList : public Run<K, V>
    {
    public:
        typedef SkipListNode<K, V, MAXLEVEL> Node;
        const int max_level;

        SkipList(K _MinK, k _MaxK): p_listHead(NULL), p_listTail(NULL), cur_max_level(1), max_level(MAXLEVEL),
                                    minKey(_MinK), maxKey(_MaxK), n(0)
        {
            p_listHead = new Node(minKey);
            p_listTail = new Node(maxKey);
            for (int i = 1; i <= MAXLEVEL; i++) {
                p_listHead->_forward[i] = p_listTail;
            }
        }

        virtual ~SkipList()
        {
            Node * currNode = p_listHead->_forward[1];
            while (currNode != p_listTail)
            {
                Node * tempNode = currNode;
                currNode = currNode->_forward[1];
                delete tempNode;
            }
            delete p_listHead, p_listTail;
        }

        void insert_key (const K key, V val)
        {}

        void delete_key(const K searchedKey)
        {}

        V lookup(const K searchedKey)
        {}

        uint32_t num_elements()
        {}

        void set_size(size_t size)
        {
            maxSize = size;
        }

        void get_size()
        {
            return n * (sizeof(K) + sizeof(V));
        }

    private:
        K minKey;
        K maxKey;
        uint32_t n;
        size_t maxSize
        int cur_max_level;
        Node * p_listHead;
        Node * p_listTail;
        uint32_t keyPerLevel[MAXLEVEL];
    };
}
#endif //LEARNLAB_RUST_SKIPLIST_H
