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

void* threadFunction(void* args) {
    pair<TS::ObjectInterface*, opsStruct*> *data = (pair<TS::ObjectInterface*, opsStruct*>*) args;
    output.push_back(data->first->runOp(data->second));
    return NULL;
    // return (void*) &data->first->runOp(data->second);
}


int main(int argc, char const *argv[])
{


    // get the file name from the command line
    string inputFile = argv[1];
    string outputFile = argv[2];

    // Initialize the objbjects
    TS::EvansMap evansMap(10);
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


    output.push_back("Using " + to_string(maxNumThreads) + " threads");

    // while there are still commands to run
    // create a thread for each command for the max number of threads
    // then join the threads
    while(!commands.empty()) {
        int numThreads = commands.size() < maxNumThreads ? commands.size() : maxNumThreads;

        // threads to use
        pthread_t threads[numThreads];
        opsStruct ops[numThreads];

        for (int i = 0; i < numThreads; i++) {
            string command = commands.front();
            char c = command[0];
            commands.pop();

            int key;
            string value;

            int space1 = command.find(" ");
            int space2 = command.find(" ", space1 + 1);

            // check if there is a second space
            if (space2 != -1) {
                key = stoi(command.substr(space1 + 1, space2 - space1 - 1));
                value = command.substr(space2 + 1);
            } else {
                key = stoi(command.substr(space1 + 1));
                value = "";
            }

            ops[i].op = c;
            ops[i].key = key;
            ops[i].value = value;
            pair<TS::ObjectInterface*, opsStruct*> *args = new pair<TS::ObjectInterface*, opsStruct*>(&object, &ops[i]);
            pthread_create(&threads[i], NULL, threadFunction, (void *) args);

            // sleep for 1 microsecond to make sure the threads are created in order
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            
        }

        for (int i = 0; i < numThreads; i++) {
            // void *result = NULL;
            // pthread_join(threads[i], (void**) result);
            // output.push_back((char*) result);

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