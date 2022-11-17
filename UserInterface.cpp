#include "./EvansMap.cpp"
#include "queue"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

// shared queue
queue<string> commands;

queue<string> getCommands(string fileName);
void run(ObjectInterface *object, string file);
void* threadFunction(void* arg);

int main(int argc, char const *argv[])
{
    string file = argv[1];


    EvansMap *evansMap = new EvansMap(10);
    // MariosBST *mariosBST = new MariosBST();
    
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
        string value = command.substr(space2 + 2, command.length() - space2 - 3);

        if (object->insert(key, value)) {
            cout << "PASS" << endl;
        } else {
            cout << "FAIL" << endl;
        }
    
    } else if (command[0] == 'D') {
        // Delete a key from the hash map
        int key = stoi(command.substr(2));
        if (object->remove(key)) {
            cout << "PASS" << endl;
        } else {
            cout << "FAIL" << endl;
        }
        // object->remove(key);
    } else if (command[0] == 'L') {
        // Lookup a key in the hash map
        int key = stoi(command.substr(2));
        cout << object->get(key) << endl;
    }

    return NULL;
}

void run(ObjectInterface *object, string file) {
    commands = getCommands(file);

    vector<pthread_t> threads;
    string numThreadsCommand = commands.front();
    commands.pop();
    int maxNumThreads = stoi(numThreadsCommand.substr(2));


    while(!commands.empty()) {
        int numThreads = commands.size() < maxNumThreads ? commands.size() : maxNumThreads;

        for (int i = 0; i < numThreads; i++) {
            pthread_t thread;

            pthread_create(&thread, NULL, &threadFunction, (void*) object);

            threads.push_back(thread);
        }

        for (int i = 0; i < numThreads; i++) {
            pthread_join(threads[i], NULL);
        }

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