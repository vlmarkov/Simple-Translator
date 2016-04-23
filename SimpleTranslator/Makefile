all: main

main: main.o LexemNode.o LexemTable.o LexemManager.o DataTypeManager.o SyntaxManager.o SyntaxNode.o AssemblyGen.o header.h
	g++ -g -o main main.o LexemNode.o LexemTable.o LexemManager.o DataTypeManager.o SyntaxManager.o SyntaxNode.o AssemblyGen.o -std=c++11

main.o: main.cpp
	g++ -c main.cpp -std=c++11

LexemNode.o: LexemNode.cpp
	g++ -c LexemNode.cpp -std=c++11

LexemTable.o: LexemTable.cpp
	g++ -c LexemTable.cpp -std=c++11

LexemManager.o: LexemManager.cpp
	g++ -c LexemManager.cpp -std=c++11

DataTypeManager.o: DataTypeManager.cpp
	g++ -c DataTypeManager.cpp -std=c++11

SyntaxManager.o: SyntaxManager.cpp
	g++ -c SyntaxManager.cpp -std=c++11

SyntaxNode.o: SyntaxNode.cpp
	g++ -c SyntaxNode.cpp -std=c++11

AssemblyGen.o: AssemblyGen.cpp
	g++ -c AssemblyGen.cpp -std=c++11

clean:
	rm -f *.o