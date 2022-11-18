#include "./EvansMap.cpp"
#include "queue"
#include <iostream>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <mutex>

using namespace std;

// shared queue
queue<string> commands;

// change this to use buffer to read large files
queue<string> getCommands(string fileName);
void run(ObjectInterface *object, string file);
void* threadFunction(void* arg);
// add a get command method to get the char command from a string

// pthread_mutex_t lock1;

// void *testThread1(void *arg) {
//     // lock the mutex
//     pthread_mutex_lock(&lock1);
//     int i = 0;
//     while (i < 200) {
//         cout << "Thread 1: " << i << endl;
//         i++;
//     }
//     // unlock the mutex
//     pthread_mutex_unlock(&lock1);
//     return NULL;
// }

// void *testThread2(void *arg) {
//     // lock the mutex
//     pthread_mutex_lock(&lock1);
//     int i = 0;
//     while (i < 200) {
//         cout << "Thread 2: " << i << endl;
//         i++;
//     }
//     // unlock the mutex
//     pthread_mutex_unlock(&lock1);
//     return NULL;
// }


int main(int argc, char const *argv[])
{
    string file = argv[1];

    // pthread_t thread_id; 
    // printf("Before Thread\n");
    // if(pthread_mutex_init(&lock1, NULL) !=0){
    //     printf("Mutex failed");
    //     return -1;
    // } 
    // pthread_create(&thread_id, NULL, testThread1, NULL); 
    // pthread_create(&thread_id, NULL, testThread2, NULL);
    // pthread_join(thread_id, NULL); 
    // printf("After Thread\n"); 
    // pthread_join(thread_id, NULL);
    // pthread_join(thread_id,NULL);
    // pthread_mutex_destroy(&lock1);


    EvansMap *evansMap = new EvansMap(10);
    // // MariosBST *mariosBST = new MariosBST();
    
    run(evansMap, file);
    // run(mariosBST, file);


    return 0;
}

void* threadFunction(void* arg) {
    ObjectInterface *object = (ObjectInterface*) arg;
    string command = commands.front();
    commands.pop();
   
   if (command[0] == 'I') {
        // Insert a key and value into the hash map
        int space1 = command.find(" ");
        int space2 = command.find(" ", space1 + 1);
        int key = stoi(command.substr(space1 + 1, space2 - space1 - 1));
        string value = command.substr(space2 + 1);
        // lock the mutex


        if (object->insert(key, value)) {
            cout << "OK" << endl;
        } else {
            cout << "Fail" << endl;
        }
    
    } else if (command[0] == 'D') {
        // Delete a key from the hash map
        int key = stoi(command.substr(2));
        if (object->remove(key)) {
            cout << "Ok" << endl;
        } else {
            cout << "Fail" << endl;
        }
        // object->remove(key);
    } else if (command[0] == 'L') {
        // Lookup a key in the hash map
        int key = stoi(command.substr(2));
        cout << object->get(key) << endl;
    }

    // return NULL;
    pthread_exit(NULL);
}

void run(ObjectInterface *object, string file) {
    commands = getCommands(file);

    vector<pthread_t> threads;
    string numThreadsCommand = commands.front();
    commands.pop();
    int maxNumThreads = stoi(numThreadsCommand.substr(2));

    cout << "Using " << maxNumThreads << " threads" << endl;


    while(!commands.empty()) {
        int numThreads = commands.size() < maxNumThreads ? commands.size() : maxNumThreads;

        for (int i = 0; i < numThreads; i++) {
            pthread_t thread;

            pthread_create(&thread, NULL, &threadFunction, (void*) object);

            threads.push_back(thread);
        }

        // cout << "Running " << numThreads << " threads" << endl;

        for (int i = 0; i < numThreads; i++) {
            pthread_join(threads[i], NULL);
        }


        // cout << "Finished " << numThreads << " threads" << endl;

        threads.clear();
    }
}

queue<string> getCommands(string fileName) {
    queue<string> commands;
    std::ifstream in(fileName);
    string line = "";
    if (in.is_open()) {
        while (getline(in, line)) {
            commands.push(line);
        }
    } else {
        cout << "Unable to open file" << endl;
    }
    in.close();
    return commands;
}