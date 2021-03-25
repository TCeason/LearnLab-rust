//
// Created by taichong on 2021/3/25.
//

#include <iostream>
#include <vector>

using namespace std;

class Array {
public:
    vector<int> data;
    Array(int capacity) {
        this->count = 0;
        this->length = capacity;
        this->data = vector<int>(capacity);
    }

    int find(int index){
        if (index < 0 || index > count) return -1;
        return 0;
    }

    bool insert(int index, int value) {
        if (index == count && count == 0){
            data[index] = value;
            ++count;
            return true;
        }
        if (length == count) {
            cout << "The array if full." << endl;
            return false;
        }
        if (index < 0 || index > count){
            cout << "The index is unsafe." << endl;
            return false;
        }
        for (int i = count; i > index ; --i) {
            data[i] = data[i-1];
        }
        data[index] = value;
        ++count;
        return true;
    };

    bool deleteArr(int index) {
        if (index < 0 || index > count) return false;
        for (int i = index+1; i < count; ++i) {
            data[i-1] = data[i];
        }

        vector<int> newarr = vector<int>(this->count-1);
        for (int i = 0; i < count-1; i++) {
            newarr[i] = data[i];
        }
        this->data = newarr;

        --count;
        return true;
    }

    void printAll() {
        for (int i = 0; i < count; ++i) {
            cout << data[i] << " " ;
        }
        cout << endl;
    }
private:
    int count;
    int length;
};

int main() {
    int capacity = 4;
    Array arr = Array(capacity);
    arr.insert(10, 5);
    for (int i = 0; i < capacity; ++i) {
        arr.insert(i, i*2);
    }
    arr.insert(10, 5);
    arr.printAll();
    arr.find(6);
    arr.deleteArr(1);
    arr.printAll();
    arr.insert(3, 5);
    arr.printAll();
}
