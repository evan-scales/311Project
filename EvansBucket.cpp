// Written by Evan Scales

#include <string>
#include <shared_mutex>

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
    EvansBucket() { }
    ~EvansBucket() {
        Node *current = head;
        while (current != NULL) {
            Node *next = current->next;
            delete current;
            current = next;
        }
    }
    bool insert(int key, std::string value) {
        std::unique_lock lock(mutex_);
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->key == key) {
                return false;
            }
            curr = curr->next;
        }

        Node* newNode = new Node(key, value);
        newNode->next = head;
        head = newNode;
        size++;
        return true;
    }
    std::string get(int key) {
        std::shared_lock lock(mutex_);
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->key == key) {
                return curr->value;
            }
            curr = curr->next;
        }
        return "No " + std::to_string(key);
    }

    bool remove(int key) {
        std::unique_lock lock(mutex_);
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
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

private:

    Node *head;
    int size;
    mutable std::shared_timed_mutex mutex_;
};
}