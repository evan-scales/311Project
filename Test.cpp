/* Written by Chris Moore
   CSCE311, USC Columbia Fall '22
*/

/* Waiting on user interface code to be commented before I attempt this

*/

#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>
#include <mutex>

using std::cin, std::cout, std::endl;

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

int main() {
    cout << "Hey, there's nothing here yet!" << endl;
    return 0;
}