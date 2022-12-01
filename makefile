# please reference README.txt if you need help utilizing this

compile:
	g++ -pthread FileGenerator.cpp -o filegenerator -I . -std=c++17
	g++ -pthread ObjectInterface.cpp UserInterface.cpp EvansMap.cpp -o userinterface -I . -std=c++17
	g++ Compare.cpp -o compare -I . -std=c++17


clean:
	rm *.txt

clean_compiled:
	rm *.o a.out userinterface compare filegenerator mariosbst evansmap objectinterface userinterface

# hashmap unit tests

test1t_hash:
	./filegenerator test 500 250 750 1
	./userinterface test.txt test_out.txt 0 1

test2t_hash:
	./filegenerator test 500 250 750 2
	./userinterface test.txt test_out.txt 0 1

test4t_hash:
	./filegenerator test 500 250 750 4
	./userinterface test.txt test_out.txt 0 1

test8t_hash:
	./filegenerator test 500 250 750 8
	./userinterface test.txt test_out.txt 0 1

test0t_hash:
	./filegenerator test 500 250 750 0
	./userinterface test.txt test_out.txt 0 1
	
test64t_hash:
	./filegenerator test 500 250 750 64
	./userinterface test.txt test_out.txt 0 1

testlarge_hash:
	./filegenerator test 750 250 1500 4
	./userinterface test.txt test_out.txt 0 1

testverylarge_hash:
	./filegenerator test 7500 2500 15000 4
	./userinterface test.txt test_out.txt 0 1

testmassive_hash:
	# This is going to take a while.
	./filegenerator test 75000 25000 150000 4
	./userinterface test.txt test_out.txt 0 1

testempty_hash:
	./filegenerator test 0 0 0 4
	./userinterface test.txt test_out.txt 0 1

# bst unit tests

# hashmap unit tests

test1t_bst:
	./filegenerator test 500 250 750 1
	./userinterface test.txt test_out.txt 1 1

test2t_bst:
	./filegenerator test 500 250 750 2
	./userinterface test.txt test_out.txt 1 1

test4t_bst:
	./filegenerator test 500 250 750 4
	./userinterface test.txt test_out.txt 1 1

test8t_bst:
	./filegenerator test 500 250 750 8
	./userinterface test.txt test_out.txt 1 1

test0t_bst:
	./filegenerator test 500 250 750 0
	./userinterface test.txt test_out.txt 1 1
	
test64t_hash:
	./filegenerator test 500 250 750 64
	./userinterface test.txt test_out.txt 1 1

testlarge_bst:
	./filegenerator test 750 250 1500 4
	./userinterface test.txt test_out.txt 1 1

testverylarge_bst:
	./filegenerator test 7500 2500 15000 4
	./userinterface test.txt test_out.txt 1 1

testmassive_bst:
	# This is going to take a while.
	./filegenerator test 75000 25000 150000 4
	./userinterface test.txt test_out.txt 1 1

testempty_bst:
	./filegenerator test 0 0 0 4
	./userinterface test.txt test_out.txt 1 1
