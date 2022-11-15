// Written by Evan Scales
#include <iostream>
#include <pthread.h>
#include <mutex>
#include <string>
#include <vector>
#include <list>

using namespace std;

class EvansMap {
public:
    EvansMap(int size) {
        table.resize(size);
        // init the lists
        for (int i = 0; i < table.size(); i++) {
            List *list = &table[i];
            initList(list);
        }
        pthread_mutex_init(&mutex, NULL);
    }
    ~EvansMap() {
        table.clear();
        pthread_mutex_destroy(&mutex);
    }
    bool insert(int key, string value) {
        int index = hash(key);
        List *list = &table[index];
        return insert(key, value, list);
    }
    string get(int key) {
        int index = hash(key);
        List *list = &table[index];
        if (list->head == NULL) {
            return "No " + to_string(key);
        }
        return get(key, list);

    }
    bool remove(int key) {
        int index = hash(key);
        List *list = &table[index];
        if (list->head == NULL) {
            return false;
        }
        return remove(key, list);
    }
    void print() {
        // Lock the mutex
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < table.size(); i++) {
            cout << i << ": ";
            List *list = &table[i];
            print(list);
            cout << endl;
        }
        // Unlock the mutex
        pthread_mutex_unlock(&mutex);
    }

private:
    int hash(int key) {
        return key % table.size();
    }
    struct Node {
        int key;
        string value;
        Node* next;
        Node(int key, string value) {
            this->key = key;
            this->value = value;
            this->next = NULL;
        }
    };
    struct List {
        Node* head;
        Node* tail;
        int size;
        pthread_mutex_t mutex;
    };

    void initList(List* list) {
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
        pthread_mutex_init(&list->mutex, NULL);
    }

    bool insert(int key, string value, List* list) {
        // Lock the mutex
        pthread_mutex_lock(&list->mutex);
        Node* node = new Node(key, value);
        if (list->head == NULL) {
            list->head = node;
            list->size++;
            // Unlock the mutex
            pthread_mutex_unlock(&list->mutex);
            return true;
        }
        Node* current = list->head;
        while (current->next != NULL) {
            if (current->key == key) {
                // Unlock the mutex
                pthread_mutex_unlock(&list->mutex);
                return false;
            }
            current = current->next;
        }
        current->next = node;
        list->size++;
        // Unlock the mutex
        pthread_mutex_unlock(&list->mutex);
        return true;
    }

    string get(int key, List* list) {
        // Lock the mutex
        pthread_mutex_lock(&list->mutex);
        Node* current = list->head;
        while (current != NULL) {
            if (current->key == key) {
                // Unlock the mutex
                pthread_mutex_unlock(&list->mutex);
                return current->value;
            }
            current = current->next;
        }
        // Unlock the mutex
        pthread_mutex_unlock(&list->mutex);
        return "No " + to_string(key);
    }

    void print(List* list) {
        // Lock the mutex
        pthread_mutex_lock(&list->mutex);
        Node* current = list->head;
        while (current != NULL) {
            cout << current->key << " " << current->value << " ";
            current = current->next;
        }
        cout << endl;
        // Unlock the mutex
        pthread_mutex_unlock(&list->mutex);
    }

    bool remove(int key, List* list) {
        // Lock the mutex
        pthread_mutex_lock(&list->mutex);
        Node* current = list->head;
        if (current == NULL) {
            // Unlock the mutex
            pthread_mutex_unlock(&list->mutex);
            return false;
        }
        if (current->key == key) {
            list->head = current->next;
            list->size--;
            // Unlock the mutex
            pthread_mutex_unlock(&list->mutex);
            return true;
        }
        while (current->next != NULL) {
            if (current->next->key == key) {
                current->next = current->next->next;
                list->size--;
                // Unlock the mutex
                pthread_mutex_unlock(&list->mutex);
                return true;
            }
            current = current->next;
        }
        // Unlock the mutex
        pthread_mutex_unlock(&list->mutex);
        return false;
    }
    vector<List> table;
    pthread_mutex_t mutex;
};