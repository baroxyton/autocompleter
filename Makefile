main.o: main.cpp
	g++ main.cpp -o main.o -lcurl
install:
	cp main.o /usr/local/bin/autocompleter
uninstall:
	rm /usr/local/bin/autocompleter