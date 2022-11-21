#include "./EvansMap.cpp"
#include "./MariosBST.cpp"
#include "queue"
#include <iostream>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <mutex>

using namespace std;

// shared queue
queue<string> commands;

// shared array for output
vector<string> output;

// change this to use buffer to read large files
queue<string> getCommands(string fileName);
void run(ObjectInterface *object, string file, string outputFile);
void* threadFunction(void* arg);

// ==================== Just playing around with thread stuff ====================

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


    // ==================== Just playing around with thread stuff ====================


    // get the file name from the command line
    string inputFile = argv[1];
    string outputFile = argv[2];

    // Initialize the objbjects
    // EvansMap *evansMap = new EvansMap(10);
    MariosBST *mariosBST = new MariosBST();
    
    // output.reserve(1000000);
    // run the program
    // run(evansMap, inputFile, outputFile);
    run(mariosBST, inputFile, outputFile);


    return 0;
}

// thread function
// gets command from queue and runs it on the object
// adds the output to the output array
void* threadFunction(void* arg) {
    // get the object
    ObjectInterface *object = (ObjectInterface*) arg;
    // get the command
    string command = commands.front();
    // remove the command from the queue
    commands.pop();

    // cout << "Thread: " << command << endl;
    // output.push_back(command);
   
   if (command[0] == 'I') {
        // Insert a key and value into the hash map
        int space1 = command.find(" ");
        int space2 = command.find(" ", space1 + 1);
        int key = stoi(command.substr(space1 + 1, space2 - space1 - 1));
        string value = command.substr(space2 + 1);

        if (object->insert(key, value)) {
            // cout << "OK" << endl;
            output.push_back("OK");
        } else {
            // cout << "Fail" << endl;
            output.push_back("Fail");
        }
    
    } else if (command[0] == 'D') {
        // Delete a key from the hash map
        int key = stoi(command.substr(2));
        if (object->remove(key)) {
            // cout << "Ok" << endl;
            output.push_back("OK");
        } else {
            // cout << "Fail" << endl;
            output.push_back("Fail");
        }
        // object->remove(key);
    } else if (command[0] == 'L') {
        // Lookup a key in the hash map
        int key = stoi(command.substr(2));
        // cout << object->get(key) << endl;
        output.push_back(object->get(key));
    }
    else {
        cout << "Invalid command" << endl;
    }

    // return NULL;
    pthread_exit(NULL);
}

void run(ObjectInterface *object, string file, string outputFile) {
    // get the commands from the file
    commands = getCommands(file);

    // threads to use
    vector<pthread_t> threads;
    // get the max number of threads to use
    string numThreadsCommand = commands.front();
    commands.pop();
    int maxNumThreads = stoi(numThreadsCommand.substr(2));

    output.push_back("Using " + to_string(maxNumThreads) + " threads");
    // cout << "Using " << maxNumThreads << " threads" << endl;


    // while there are still commands to run
    // create a thread for each command for the max number of threads
    // then join the threads
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

    // print the output
    // will eventually write to a file
    // write to outputFile
    for (int i = 0; i < output.size(); i++) {
        cout << output[i] << endl;
    }

    // clear the output
    output.clear();
}

// reads in a file with commands to be added to a global queue
// will change to use a buffer to handle files larger than memory
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