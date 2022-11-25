# please reference readme_instructions.md if you need help utilizing this

generatetxt: GenerateTxt.cpp
	g++ -I . GenerateTxt.cpp -o generatetxt

userinterface:
	g++ -Wall -std=c++17 -I . -lpthread EvansMap.cpp EvansBucket.cpp UserInterface.cpp ObjectInterface.cpp -o userinterface

test:
	g++ -I . Test.cpp -o test

clear:
	rm *.o a.out userinterface test generatetxt

cleartxt:
	rm *.txt