// Written by Evan Scales

#include <string>
#include <shared_mutex>
#include <iostream>

struct Node {
    int key;
    std::string value;
    Node *next;
    Node(int key, std::string value) {
        this->key = key;
        this->value = value;
        this->next = nullptr;
    }
};

namespace TS {

class EvansBucket {
public:
    EvansBucket() { 
        head = nullptr;
        size = 0;
        // unlock the mutex
        // mutex_ = PTHREAD_MUTEX_INITIALIZER;
    }

    ~EvansBucket() {
        Node *current = head;
        while (current != NULL) {
            Node *next = current->next;
            delete current;
            current = next;
        }
    }
    bool insert(int key, std::string value) {
        // std::unique_lock lock(mutex_);
        // std::cout << "inserting " << key << " " << value << std::endl;
        // lock the mutex
        // pthread_mutex_lock(&mutex_);
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->key == key) {
                // unlock the mutex
                // pthread_mutex_unlock(&mutex_);
                // std::cout << "Fail to insert " << key << " " << value << std::endl;
                return false;
            }
            curr = curr->next;
        }

        Node* newNode = new Node(key, value);
        newNode->next = head;
        head = newNode;
        size++;
        // unlock the mutex
        // pthread_mutex_unlock(&mutex_);
        // std::cout << "OK inserting " << key << " " << value << std::endl;
        return true;
    }
    std::string get(int key) {
        // std::shared_lock lock(mutex_);
        // lock the mutex
        // pthread_mutex_lock(&mutex_);
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->key == key) {
                // unlock the mutex
                // pthread_mutex_unlock(&mutex_);
                return curr->value;
            }
            curr = curr->next;
        }
        // unlock the mutex
        // pthread_mutex_unlock(&mutex_);
        return "No " + std::to_string(key);
    }

    bool remove(int key) {
        // std::unique_lock lock(mutex_);
        // lock the mutex
        // pthread_mutex_lock(&mutex_);
        Node* curr = head;
        Node* prev = nullptr;
        while (curr != nullptr) {
            if (curr->key == key) {
                if (prev == nullptr) {
                    head = curr->next;
                } else {
                    prev->next = curr->next;
                }
                delete curr;
                size--;
                // unlock the mutex
                // pthread_mutex_unlock(&mutex_);
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        // unlock the mutex
        // pthread_mutex_unlock(&mutex_);
        return false;
    }

    void lock() {
        mutex_.lock();
    }

    void unlock() {
        mutex_.unlock();
    }

private:

    Node *head;
    int size;
    mutable std::shared_timed_mutex mutex_;
    // pthread_mutex_t mutex_;
};
}