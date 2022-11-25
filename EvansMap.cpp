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