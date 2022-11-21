/* Written by Chris Moore
   CSCE311, USC Columbia Fall '22
*/

/* This file does not link with any other source code in the repository.
   It is purely to generate random test input, with constraints if desired (coming soon?).
*/ 

#include <iostream>
#include <string>
#include <fstream>

using std::cin, std::cout, std::endl;
using std::string;
using std::ifstream, std::ofstream;
using std::rand, std::srand;

string RandomString(int size) {
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string str = "";
    for (int i = 0; i < size; i++) {
        str += alphabet[rand() % 26];
    }
    return str;
}

void GenerateRandom(int numFiles, int threads, int keyLimit, int numCommands) {
    string fileName;
    for (int i = 0; i < numFiles; i++) {
        srand(time(0));
        cout << "Please name file #" << i + 1 << ". (no .txt extension necessary):" << endl;
        cin >> fileName;
        ofstream file(fileName + ".txt");
        cout << "Generating random test file..." << endl;
        if (file.is_open()) {
            file << "N " << threads << "\n";
            for (int j = 0; j < numCommands; j++) {
                // Generate random insert/lookup/delete command
                int cmd = rand() % 3 + 1;
                switch (cmd) {
                    case 1: { // Insert
                        int strSize = rand() % 10 + 4;
                        string str = RandomString(strSize);
                        file << "I " << rand() % keyLimit + 0 << " " << str << "\n";
                        break;
                    }
                    case 2: {  // Lookup
                        file << "L " << rand() % keyLimit + 0 << "\n";
                        break;
                    }
                    case 3: { // Delete
                        file << "D " << rand() % keyLimit + 0 << "\n";
                        break;
                    }
                }
            }
            cout << "Done." << endl;
            file.close();
        } else {
            cout << "Unable to generate file." << endl;
            return;
        }
    }
    cout << "All test files generated successfully." << endl;
}

/*  void GenerateWithConstraints(int numFiles, int threads, int keyLimit, int numCommands) {
      srand(time(0));
    }
*/

int main() {
    int numFiles, threads, keyLimit, numCommands;
    char hasConstraints;
    cout << "Enter the number of input files you want to generate:" << endl;
    cin >> numFiles;
    cout << "Enter the number of threads the input file(s) will specify:" << endl;
    cin >> threads;
    cout << "Enter the highest number key that can be generated (max = 2^22)" << endl;
    cin >> keyLimit;
    cout << "Enter the number of commands you wish the input file(s) to include" << endl;
    cin >> numCommands;
    /*  cout << "Do you wish to add constraints to the file generation [y] or simply generate randomly [n]?" << endl;
        cin >> hasConstraints;
        switch (hasConstraints) {
            case 'y': 
                cout << "Not done with constraints yet..." << endl;
                break;
            default: 
                cout << "Generating random test file..." << endl;
                GenerateRandom(numFiles, threads, keyLimit, numCommands);
                break;
        }
    */
    GenerateRandom(numFiles, threads, keyLimit, numCommands);
}