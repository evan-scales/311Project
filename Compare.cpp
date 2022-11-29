/* Written by Chris Moore
   CSCE311, USC Columbia Fall '22
*/

#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>
#include <mutex>
#include <vector>
#include "./EvansMap.cpp"

using std::cin, std::cout, std::endl, std::vector, std::string;

/**
 * @brief How to go about creating tests to compare our multi-threaded data structures to the single-threaded ones
 * 
 * opsStruct, holds char op int key string value, if no value then value is empty string, string result
 * 
 * Generate commands, creatting a new opsStruct for each command
 * Use unordered_map to get the expected result for the opsStruct
 * 
 * vector<opsStruct> commands
 * int i = 0
 * unoraered_map<int, string> map
 * while (i++ < commands.size()) 
 *    commands[i].result = map.runOp(commands[i])
 * 
 * No runOp command on unordered_map, so use switch statement to get op
 * then insert, delete, or find the key
 * Insert: "OK" if inserted, "FAIL" if not inserted
 * Delete: "OK" if deleted, "FAIL" if not deleted
 * Lookup: "{value}" if found, "No {key}" if not found
 * 
 * Write the commands to a file
 * Write the expected results to a file
 * Write a function to compare 2 result output files to see if they match
 * 
 */

bool Compare(string arg1, string arg2) {
    ifstream file1(arg1);
    ifstream file2(arg2);
    int count = 1;
    string compare1, compare2;
    while (file1.good()) {
        file1 >> compare1;
        file2 >> compare2;
        if (compare1 != compare2) {
            cout << "[FAIL]\n" << arg1 << " produced: " << compare1 <<
            "\nBut expected: " << compare2 << "\nAt string " << count << endl;
            return false;
        }
        count++;
    }
    file1.close();
    file2.close();
    cout << "[SUCCESS]\n<" << arg1 << "> and <" << arg2 << "> match." << endl;
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "./compare has format: <file1.txt> <file2.txt> " << endl;
        return -1;
    }

    Compare(argv[1], argv[2]);

    return 0;
}