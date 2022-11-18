// Written by Evan Scales

#include <string>
#include <iostream>
using namespace std;
#include <shared_mutex>
#include <mutex>
// #include <shared_lock>



class EvansBucket {
public:
   EvansBucket() {
        // init the mutex
        mutex = PTHREAD_MUTEX_INITIALIZER;

   }
    ~EvansBucket() {
        pthread_mutex_destroy(&mutex);
        // Delete the list
        Node *current = head;
        while (current != NULL) {
            Node *next = current->next;
            delete current;
            current = next;
        }
    }
    bool insert(int key, string value) {
        // lock the mutex
        if (pthread_mutex_lock(&mutex)) {
            cout << "mutex lock failed" << endl;
        }

        // std::unique_lock lock(mutex);
        // check if the key is already in the list
        Node *current = head;
        while (current != NULL) {
            if (current->key == key) {
                // unlock the mutex
                pthread_mutex_unlock(&mutex);
                return false;
            }
            current = current->next;
        }
        // insert the new node
        Node *node = new Node(key, value);
        node->next = head;
        head = node;
        size++;
        // unlock the mutex
        pthread_mutex_unlock(&mutex);
        return true;
    }
    string get(int key) {
        // lock the mutex
        pthread_mutex_lock(&mutex);
        // std::shared_lock lock(mutex);
        Node *current = head;
        while (current != NULL) {
            if (current->key == key) {
                // unlock the mutex
                pthread_mutex_unlock(&mutex);
                return current->value;
            }
            current = current->next;
        }
        // unlock the mutex
        pthread_mutex_unlock(&mutex);
        return "No " + to_string(key);
    }
    bool remove(int key) {
        // // lock the mutex
        pthread_mutex_lock(&mutex);
        // std::unique_lock lock(mutex);
        Node *current = head;
        Node *prev = NULL;
        while (current != NULL) {
            if (current->key == key) {
                if (prev == NULL) {
                    head = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                size--;
                // unlock the mutex
                pthread_mutex_unlock(&mutex);
                return true;
            }
            prev = current;
            current = current->next;
        }
        // unlock the mutex
        pthread_mutex_unlock(&mutex);
        return false;
    }

private:
    struct Node {
        int key;
        string value;
        Node *next;
        Node(int key, string value) {
            this->key = key;
            this->value = value;
            this->next = NULL;
        }
    };

    Node *head;
    int size;
    pthread_mutex_t mutex;
};