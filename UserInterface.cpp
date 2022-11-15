#include "./EvansMap.cpp"
#include "queue"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

// shared queue
queue<string> commands;

void testFunction(ObjectInterface *object, int key, string value);
queue<string> getCommands(string fileName);
void run(ObjectInterface *object, string file);
void* threadFunction(void* arg);

int main(int argc, char const *argv[])
{
    string file = argv[1];

    EvansMap *evansMap = new EvansMap(10);

    run(evansMap, file);


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
        object->insert(key, value);
    } else if (command[0] == 'D') {
        // Delete a key from the hash map
        int key = stoi(command.substr(2));
        object->remove(key);
    } else if (command[0] == 'L') {
        // Lookup a key in the hash map
        int key = stoi(command.substr(2));
        object->get(key);
    }

    return NULL;
}

void run(ObjectInterface *object, string file) {
    commands = getCommands(file);

    vector<pthread_t> threads;
    string numThreadsCommand = commands.front();
    commands.pop();
    int numThreads = stoi(numThreadsCommand.substr(2));

    while(!commands.empty()) {

        for (int i = 0; i < numThreads; i++) {
            string command = commands.front();
            commands.pop();

            pthread_t thread;
            threads.push_back(thread);
            
            pthread_create(&thread, NULL, &threadFunction, (void*) &object);

    
        }

        for (int i = 0; i < numThreads; i++) {
            pthread_join(threads[i], NULL);
        }
    }
}

void testFunction(ObjectInterface *object, int key, string value) {
    object->insert(key, value);
    object->print();
    cout << object->get(key) << endl;
    object->remove(key);
    object->print();
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