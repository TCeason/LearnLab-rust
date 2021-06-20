//
// Created by taichong on 2021/6/20.
//

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Node {
public:
    int key;

    vector<Node *> forward;

    Node(int key, int level)
    {
        this->key = key;
        this->forward = vector<Node *>(level + 1);
    }
};

class SkipList {
public:
    Node * head;
    int MAXLEVEL = 10;
    float p = 0.1;
    int level = 0;

    SkipList()
    {
        this->head = new Node(-1, MAXLEVEL + 1);
    }

    int randomLevel() {
        float r = (float) rand() / RAND_MAX;
        int lvl = 0;
        while (r < p && lvl < MAXLEVEL)
        {
            lvl++;
            r = (float ) rand() / RAND_MAX;
        }
        return lvl;
    }

    void insert(int key) {
        auto updated = vector<Node *>(MAXLEVEL + 1);
        auto cur = this->head;
        for (int i = this->level; i >= 0; i--)
        {
            while (cur->forward[i] != nullptr && cur->forward[i]->key < key)
            {
                cur = cur->forward[i];
            }
            updated[i] = cur;
        }
        cur = cur->forward[0];

        if (cur == nullptr || cur->key != key) {
            auto lvl = this->randomLevel();
            if (lvl > this->level) {
                for (int i = this->level + 1; i <= lvl; i++)
                {
                    updated[i] = this->head;
                }
                this->level = lvl;
            }
            Node *newNode = new Node(key, lvl);

            for (int i = 0; i <= lvl; i++)
            {
                newNode->forward[i] = updated[i]->forward[i];
                updated[i]->forward[i] = newNode;
            }
        }
    }

    bool search (int key)
    {
        auto cur = this->head;
        for (int i = this->level; i >= 0; i--)
        {
            while (cur->forward[i] != nullptr && cur->forward[i]->key < key)
            {
                cur = cur->forward[i];
            }
        }
        cur = cur->forward[0];
        return cur != nullptr && cur->key == key;
    }

    void deleteByKey(int key) {
        if (!search(key)) {
            return;
        }
        auto updated = vector<Node *>(MAXLEVEL + 1);
        auto cur = this->head;
        for (int i = this->level; i >= 0; i--) {
            while (cur->forward[i] != nullptr && cur->forward[i]->key < key)
            {
                cur = cur->forward[i];
            }
            updated[i] = cur;
        }
        cur = cur->forward[0];
        if (cur->key != key) {
            return;
        }

        for (int i = cur->forward.size() - 1; i >= 0; i--)
        {
            updated[i]->forward[i] = updated[i]->forward[i]->forward[i];
        }
        delete(cur);
        while (this->level > 0 && this->head->forward[this->level] == nullptr)
        {
            this->level--;
        }
    }

    void showList() {
        cout << "=== Skip List ===" << endl;
        for (int i = 0; i <= level; i++)
        {
            auto cur = this->head->forward[i];
            while (cur != nullptr)
            {
                cout << cur->key;
                if (cur->forward[i] != nullptr)
                {
                    cout << "->";
                }
                cur = cur->forward[i];
            }
            cout << "\n";
        }
    }
};

int main() {
    auto list = new SkipList();
    list->insert(1);

    list->showList();
    list->deleteByKey(5);
    list->showList();

    cout << list->search(4) << endl;
    cout << list->search(11) << endl;

    return 0;
}
