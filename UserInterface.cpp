#include <chrono>
#include <thread>
#include "./EvansMap.cpp"
// #include "./MariosBST.cpp"
#include "queue"
#include <iostream>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <string>

using std::string;

using namespace std;

// shared queue
queue<string> commands;


// shared array for output
vector<string> output;

// change this to use buffer to read large files
queue<string> getCommands(string fileName);
void run(TS::ObjectInterface &object, string file, string outputFile);


int main(int argc, char const *argv[])
{


    // get the file name from the command line
    string inputFile = argv[1];
    string outputFile = argv[2];

    // Initialize the objbjects
    TS::EvansMap evansMap(100);
    // MariosBST *mariosBST = new MariosBST();
    
    output.reserve(1000000);


    // // run the program 
    run(evansMap, inputFile, outputFile);
    // run(mariosBST, inputFile, outputFile);


    return 0;
}



// run the program with a given object inputfile and outputfile
// will read the input file and run the commands with the number of specified threads
// will write the output to the output file
void run(TS::ObjectInterface &object, string file, string outputFile) {
    // get the commands from the file
    commands = getCommands(file);

    // get the max number of threads to use
    string numThreadsCommand = commands.front();
    commands.pop();
    int maxNumThreads = stoi(numThreadsCommand.substr(2));

    // threads to use
    pthread_t threads[maxNumThreads];

    output.push_back("Using " + to_string(maxNumThreads) + " threads");

    // while there are still commands to run
    // create a thread for each command for the max number of threads
    // then join the threads
    while(!commands.empty()) {
        int numThreads = commands.size() < maxNumThreads ? commands.size() : maxNumThreads;

        for (int i = 0; i < numThreads; i++) {
            pthread_t thread = nullptr;

            // lambda function to run the command
            if (pthread_create(&thread, NULL, [](void *arg) -> void* {
                TS::ObjectInterface *object = (TS::ObjectInterface *)arg;
                string command = commands.front();
                commands.pop();

                if (command[0] == 'I') {
                    int space1 = command.find(" ");
                    int space2 = command.find(" ", space1 + 1);
                    int key = stoi(command.substr(space1 + 1, space2 - space1 - 1));
                    std::string value = command.substr(space2 + 1);

                    object->insert(key, value) ? output.push_back("OK") : output.push_back("FAIL");
                    
                } else if (command[0] == 'L') {
                   int key = stoi(command.substr(2));

                   output.push_back(object->get(key));
                    
                } else if (command[0] == 'D') {
                    int key = stoi(command.substr(2));

                    object->remove(key) ? output.push_back("OK") : output.push_back("FAIL");
                    
                } else {
                    cout << "Invalid command" << endl;
                }
                return NULL;
            }, &object)) {
                cout << "Error: unable to create thread, " << i << endl;
                exit(-1);
            }

            threads[i] = thread;
        }

        for (int i = 0; i < numThreads; i++) {
            pthread_join(threads[i], NULL);
        }

    }

    // write to outputFile
    ofstream out;
    out.open(outputFile, ios::app);
    for (int i = 0; i < output.size(); i++) {
        out << output[i] << endl;
    }
    out.close();

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