# please reference README.txt if you need help utilizing this

compile:
	g++ FileGenerator.cpp -o filegenerator -I . -std=c++17
	g++ ObjectInterface.cpp UserInterface.cpp EvansMap.cpp -o userinterface -I . -std=c++17
	g++ Compare.cpp -o compare -I . -std=c++17


clean:
	rm test*.txt

clean_compiled:
	rm *.o a.out userinterface compare filegenerator mariosbst evansmap objectinterface userinterface