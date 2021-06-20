//
// Created by taichong on 2021/6/20.
//

#ifndef LEARNLAB_RUST_RUN_H
#define LEARNLAB_RUST_RUN_H
#include <stdio.h>
#include <cstdint>

namespace lsm {
    template <class K, class v>
    class Run {
    public:
        virtual void insert_key(const K key, const V val) = 0;
        virtual void delete_key(const K key, const V val) = 0;
        virtual V lookup(key Key) = 0;
        virtual uint32_t nums_elements() = 0;
        virtual void set_size(const size_t size) = 0;
    };
}

#endif //LEARNLAB_RUST_RUN_H
