// Written by Evan Scales
#include "./ObjectInterface.cpp"
#include "./EvansBucket.cpp"


using namespace std;

namespace TS {
class EvansMap : public ObjectInterface {
public:

    EvansMap(int numBuckets) { 
        // set the number of buckets
        this->numBuckets = numBuckets;
        // make array of buckets
        buckets = new EvansBucket[numBuckets];
    }

    ~EvansMap() {
        // delete the buckets
        delete[] buckets;
    }

    string runOp(struct opsStruct *op) {
        // cout << "OP: " << op->op << " KEY: " << op->key << " VALUE: " << op->value << endl;
        buckets[hsh(op->key)].lock();
        // cout << "Locked bucket " << hsh(op->key) << endl;
        switch (op->op) {
            case 'I':
                // cout << "inserting " << op->key << " " << op->value << endl; 
                if (insert(op->key, op->value)) {
                    buckets[hsh(op->key)].unlock();
                    return "Ok Inserting " + to_string(op->key) + " " + op->value;
                } else {
                    buckets[hsh(op->key)].unlock();
                    return "Fail Inserting " + to_string(op->key) + " " + op->value;
                }
            case 'L':
                buckets[hsh(op->key)].unlock();
                return get(op->key);
            case 'D':
                if (remove(op->key)) {
                    buckets[hsh(op->key)].unlock();
                    return "Ok Deleting " + to_string(op->key);
                } else {
                    buckets[hsh(op->key)].unlock();
                    return "Fail Deleting " + to_string(op->key);
                }
            default:
                buckets[hsh(op->key)].unlock();
                return "Fail";
        }
    }

    bool insert(int key, string value) { 
        return buckets[hsh(key)].insert(key, value);
    }
    string get(int key) { 
        return buckets[hsh(key)].get(key);
    }
    bool remove(int key) { 
        return buckets[hsh(key)].remove(key);
    }
    void print() { }
private:
    int hsh(int key) { return key % numBuckets; }
    EvansBucket *buckets;
    int numBuckets;
};
}