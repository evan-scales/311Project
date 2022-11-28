// Written by Evan Scales
#include "./ObjectInterface.cpp"
#include <list>
#include <pthread.h>


using namespace std;

class EvansMap : public ObjectInterface {
public:

    EvansMap(int numBuckets) { 
        // set the number of buckets
        this->numBuckets = numBuckets;
        // make array of buckets
        buckets = new list<Node>[numBuckets];
        locks = new pthread_mutex_t[numBuckets];
        for (int i = 0; i < numBuckets; i++) {
            pthread_mutex_init(&locks[i], NULL);
        }
    }

    ~EvansMap() {
        // delete the buckets
        delete[] locks;
        for (int i = 0; i < numBuckets; i++) {
            buckets[i].clear();
        }
        delete[] buckets;
    }

    string runOp(struct opsStruct *op) {
        pthread_mutex_lock(&locks[hsh(op->key)]);
        cout << "locked bucket " << hsh(op->key) << " for " << op->op << " " << op->value << endl;
        switch (op->op) {
            case 'I':
                if (insert(op->key, op->value)) {
                    return "Ok Inserting " + to_string(op->key) + " " + op->value;
                } else {
                    return "Fail Inserting " + to_string(op->key) + " " + op->value;
                }
            case 'L':
                return get(op->key);
            case 'D':
                if (remove(op->key)) {
                    return "Ok Deleting " + to_string(op->key);
                } else {
                    return "Fail Deleting " + to_string(op->key);
                }
            default:
                return "Fail";
        }
    }

    bool insert(int key, string value) { 
        // lock the bucket
        // pthread_mutex_lock(&locks[hsh(key)]);
        // see if the list contains the key
        for (auto it = buckets[hsh(key)].begin(); it != buckets[hsh(key)].end(); it++) {
            if (it->key == key) {
                // unlock the bucket
                pthread_mutex_unlock(&locks[hsh(key)]);
                return false;
            }
        }
        // add the key to the list
        Node newNode = Node(key, value);
        buckets[hsh(key)].push_back(newNode);
        // unlock the bucket
        pthread_mutex_unlock(&locks[hsh(key)]);
        return true;
    }
    string get(int key) { 
        // lock the bucket
        // pthread_mutex_lock(&locks[hsh(key)]);
        // see if the list contains the key
        for (auto it = buckets[hsh(key)].begin(); it != buckets[hsh(key)].end(); it++) {
            if (it->key == key) {
                // unlock the bucket
                pthread_mutex_unlock(&locks[hsh(key)]);
                return it->value;
            }
        }
        // unlock the bucket
        pthread_mutex_unlock(&locks[hsh(key)]);
        return "No " + to_string(key);
    }
    bool remove(int key) { 
        // lock the bucket
        // pthread_mutex_lock(&locks[hsh(key)]);
        // see if the list contains the key
        for (auto it = buckets[hsh(key)].begin(); it != buckets[hsh(key)].end(); it++) {
            if (it->key == key) {
                // remove the key from the list
                buckets[hsh(key)].erase(it);
                // unlock the bucket
                pthread_mutex_unlock(&locks[hsh(key)]);
                return true;
            }
        }
        // unlock the bucket
        pthread_mutex_unlock(&locks[hsh(key)]);
        return false;
    }
    void print() { }
private:
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
    std::list<Node> * buckets;
    pthread_mutex_t * locks;
    int hsh(int key) { return key % numBuckets; }
    int numBuckets;
};