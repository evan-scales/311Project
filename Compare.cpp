/* Written by Chris Moore
   CSCE311, USC Columbia Fall '22
*/

/* This code is mostly deprecated and moved to UserInterface.cpp.
   However, it can still be used to compare an output of ./userinterface
   with its expected output as produced by ./filegenerator
   NOTE: does not test scalability, just correctness.
   USAGE: ./compare test_out.txt test_expected.txt
*/ 

#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>
#include <mutex>
#include <vector>
#include "./EvansMap.cpp"

using std::cin, std::cout, std::endl, std::vector, std::string;

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