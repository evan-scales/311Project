// Written by Evan Scales
#include <iostream>
#include <pthread.h>
#include <mutex>
#include <string>
#include <vector>
#include "./ObjectInterface.cpp"
#include "./EvansBucket.cpp"

using namespace std;

class EvansMap : public ObjectInterface {
public:

    EvansMap(int numBuckets) { 
        // set the number of buckets
        this->numBuckets = numBuckets;
        // make array of buckets
        buckets = new EvansBucket[numBuckets];
        // locks.resize(numBuckets);
        // init the locks
        // for (int i = 0; i < numBuckets; i++) {
        //     locks[i] = PTHREAD_MUTEX_INITIALIZER;
        // }
    }
    bool insert(int key, string value) { 
        // get the bucket
        int bucketIndex = key % numBuckets;
        // lock the bucket
        // pthread_mutex_lock(&locks[bucketIndex]);
        EvansBucket *bucket = &buckets[bucketIndex];
        // insert into the bucket
        bool ret = bucket->insert(key, value);
        // unlock the bucket
        // pthread_mutex_unlock(&locks[bucketIndex]);
        return ret;
    }
    string get(int key) { 
        // get the bucket
        int bucketIndex = key % numBuckets;
        // lock the bucket
        // pthread_mutex_lock(&locks[bucketIndex]);
        // locks[bucketIndex].lock();
        EvansBucket *bucket = &buckets[bucketIndex];
        // get from the bucket
        string ret = bucket->get(key);
        // unlock the bucket
        // pthread_mutex_unlock(&locks[bucketIndex]);
        return ret;
    }
    bool remove(int key) { 
        // get the bucket
        int bucketIndex = key % numBuckets;
        // lock the bucket
        // pthread_mutex_lock(&locks[bucketIndex]);
        EvansBucket *bucket = &buckets[bucketIndex];
        // remove from the bucket
        bool ret = bucket->remove(key);
        // unlock the bucket
        // pthread_mutex_unlock(&locks[bucketIndex]);
        return ret;
    }
    void print() { }
private:

    int hash(int key) { return 0; }
    EvansBucket *buckets;
    // array of locks
    // vector<pthread_mutex_t> locks;
    int numBuckets;
};