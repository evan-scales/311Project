# please reference readme_instructions.md if you need help utilizing this

generatetxt: GenerateTxt.cpp
	g++ -I . GenerateTxt.cpp -o generatetxt

userinterface:
	g++ -I . EvansMap.cpp MariosBST.cpp UserInterface.cpp -o userinterface

test:
	g++ -I . Test.cpp -o test

clear:
	rm *.o a.out userinterface test generatetxt

cleartxt:
	rm *.txt