# COMPILATION

### To compile the source code, run:
make compile

### To delete any compiled c++ executables, run: #
make clean_compiled

# USAGE

### To generate an input file (courtesy of Dr. O'Reilly's code), run:
./filegenerator <filename (no .txt)> <# of inserts> <#of deletes> <# of lookups> <# of threads>

### To run the program on a single input file, run:
./userinterface <test.txt> <test_out.txt> <0/1> <0/1>

(the first <0/1> is <Hashmap/BST>, the second <0/1> is <Don't test/Do Test vs. single-threaded version>)

### To compare an output file to its expected output, run:
./compare <test_out.txt> <test_expected.txt>

### To clean all .txt files (generated or otherwise), run:
make clean

# UNIT TESTING
## There are several included unit tests to test standard and edge cases.
### *NOTE: replace "_hash" with "_bst" to run the equivalent test on the BST.*

### To test a single threaded example, run:
make test1t_hash

### To test a two-threaded example, run:
make test2t_hash

### To test a four-threaded example, run:
make test4t_hash

### To test an eight-threaded example, run:
make test8t_hash

### To test a zero-threaded example (should default to 1 thread), run:
make test0t_hash

### To test a ridiculously-many-threaded example (should default to 32 threads), run:
make test64t_hash

### To test a large (2500 commands) file on four threads, run:
make testlarge_hash

### To test a very large (25000 commands) file on four threads, run:
make testverylarge_hash

### To test a massive (250000 commands) file on four threads, run:
make testmassive_hash

### To test an empty (0 commands) file on four threads, run:
make testempty_hash

# TESTING LARGE (>1MB) FILES
## Please keep in mind that the standard UserInterface test (setting the fourth argument to 1) takes a very long time on large files, since it takes the average of 10 single-threaded runs against 10 custom-threaded runs. To test large files without waiting several minutes, run these two commands instead:
./userinterface <gianttest.txt> <gianttest_out.txt> <0/1> 0

./compare gianttest_out.txt gianttest_expected.txt

# Evans Approach

The first issue was reading in the commands that the data structure will execute. The input file is read, then it is read line by line to create an 

opsStruct for each command. Each opsStruct has a key, the operation to perform, the value, and result. The result variable was added to keep the general 

order of the executed operations. The run method in UserInterface will read the first command for the number of threads, then while there are still 

commands to execute a the specified number of threads will be created to run each operation. Each data structure has a runOperation method that will 

insert, delete, or lookup depending on the command and return the result. Finally inside the thread function the result will be set in the appropriate 

opsStruct. Finally to write the output it will simply go through all the opsStruct and print the result of each operation to the specified file.


