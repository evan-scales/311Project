#### COMPILATION ####

# To compile the source code, run:

    make compile

# To delete any compiled c++ executables, run:

    make clean_compiled

#### TESTING ####

# To run the program, enter:
# (the first 0/1 is Hashmap/BST)
# (the second 0/1 is Don't Test/Do Test vs. Single Threaded Version)

    ./userinterface <test.txt> <test_out.txt> <0/1> <0/1> 

# To generate an input file (courtesy of Dr. O'Reilly's code), run:

    ./filegenerator <filename (no .txt)> <# of inserts> <#of deletes> <# of lookups> <# of threads>

# To compare an output file to its expected output, run:

    ./compare <test_out.txt> <test_expected.txt>