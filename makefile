# please reference README.txt if you need help utilizing this

compile:
	g++ FileGenerator.cpp -o filegenerator -I . -std=c++17
	g++ ObjectInterface.cpp UserInterface.cpp EvansMap.cpp -o userinterface -I . -std=c++17
	g++ Test.cpp -o test -I . -std=c++17


clean_compiled:
	rm *.o a.out userinterface test filegenerator mariosbst evansmap objectinterface userinterface