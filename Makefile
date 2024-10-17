parse.x: main.o commentDFA.o tokenizer.o parser.o
	g++ -std=c++17 -g main.o commentDFA.o tokenizer.o parser.o -o parse.x

main.o: main.cpp commentDFA.h tokenizer.h parser.h
	g++ -std=c++17 -g main.cpp -o main.o -c

commentDFA.o: commentDFA.cpp commentDFA.h
	g++ -std=c++17 -g commentDFA.cpp -o commentDFA.o -c

tokenizer.o: tokenizer.cpp tokenizer.h
	g++ -std=c++17 -g tokenizer.cpp -o tokenizer.o -c

parser.o: parser.cpp parser.h
	g++ -std=c++17 -g parser.cpp -o parser.o -c

clean:
	rm -f parse.x *.o
