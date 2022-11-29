#include <chrono>
#include <thread>
#include "./MariosBST.cpp"
#include "queue"
#include <iostream>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <string>

using std::string;

using namespace std;


// shared queue
vector<opsStruct> commands;

//shared map
EvansMap *evansMap = new EvansMap(750);
MariosBST *mariosBST = new MariosBST();



int EVANS_MAP = 0;
int MARIOS_BST = 1;

// change this to use buffer to read large files
vector<opsStruct> getCommands(string fileName);
void run(int object, string file, string outputFile);


void* threadFunction(void* args) {
    struct opsStruct *op = (struct opsStruct*) args;
    if (op->object == EVANS_MAP) {
        op->result = evansMap->runOp(op);
    } 
    else {
        op->result = mariosBST->runOp(op);
    }
    return NULL;
}


int main(int argc, char const *argv[])
{


    // get the file name from the command line
    string inputFile = argv[1];
    string outputFile = argv[2];


    // // run the program 
    run(EVANS_MAP, inputFile, outputFile);
    // run(MARIOS_BST, inputFile, outputFile);


    return 0;
}


// run the program with a given object inputfile and outputfile
// will read the input file and run the commands with the number of specified threads
// will write the output to the output file
void run(int object, string file, string outputFile) {
    // get the commands from the file
    commands = getCommands(file);
    int opsIndex = 0;


    int maxNumThreads = commands[opsIndex].key;
    commands[opsIndex].result = "Using " + to_string(commands[opsIndex].key) + " threads";
    commands[opsIndex].object = object;
    opsIndex++;


    // while there are still commands to run
    // create a thread for each command for the max number of threads
    // then join the threads
    // time this
    auto start = std::chrono::high_resolution_clock::now();
    while(opsIndex < commands.size()) {
        int commandsLeft = commands.size() - opsIndex;
        int numThreads = commandsLeft < maxNumThreads ? commandsLeft : maxNumThreads;

        // threads to use
        pthread_t threads[numThreads];

        for (int i = 0; i < numThreads; i++) {
            commands[opsIndex].object = object;
            pthread_create(&threads[i], NULL, &threadFunction, (void *) &commands[opsIndex]);
            // sleep for 1 microsecond to allow the threads to be created
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            opsIndex++;
        }

        for (int i = 0; i < numThreads; i++) {
            pthread_join(threads[i], NULL);
        }

    }
    auto finish = std::chrono::high_resolution_clock::now();

    // get the time
    std::chrono::duration<double> elapsed = finish - start;
    cout << "Elapsed time: " << elapsed.count() << " s" << endl;

    // write to outputFile
    ofstream out;
    out.open(outputFile, ios::app);
    for (int i = 0; i < commands.size(); i++) {
        // cout << commands[i].result << endl;
        out << commands[i].result << endl;
    }
    out.close();

    // clear the commands
    commands.clear();
}

// reads in a file with commands to be added to a global queue
// will change to use a buffer to handle files larger than memory
vector<opsStruct> getCommands(string fileName) {
    vector<opsStruct> commands;
    std::ifstream in(fileName);
    string line = "";
    if (in.is_open()) {
        while (getline(in, line)) {
            char c = line[0];
            int space1 = line.find(" ");
            int space2 = line.find(" ", space1 + 1);

            int key;
            string value;
            // check if there is a second space
            if (space2 != -1) {
                key = stoi(line.substr(space1 + 1, space2 - space1 - 1));

                string badValue = line.substr(space2 + 1);
                // remove the quotes around the value
                // value = value.substr(1, value.length() - 2);
                // remove the first and last character
                // value = value.substr(1, value.length() - 2);
                for (int i = 1; i < badValue.length() - 1; i++) {
                    value += badValue[i];
                }
                // cout << "value: " << value << endl;
            } else {
                key = stoi(line.substr(space1 + 1));
                value = "";
            }
            opsStruct op = {c, key, value};
            commands.push_back(op);
        }
    } else {
        cout << "Unable to open file" << endl;
    }
    in.close();
    return commands;
}

