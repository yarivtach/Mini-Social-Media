CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: clean StompWCIClient

StompWCIClient: bin/ConnectionHandler.o bin/event.o bin/StompClient.o bin/event.o bin/Frame.o bin/Game.o bin/StompProtocol.o bin/User.o bin/Summary.o
	g++ -o bin/StompWCIClient bin/ConnectionHandler.o bin/event.o bin/StompClient.o bin/Frame.o bin/Game.o bin/StompProtocol.o bin/User.o bin/Summary.o $(LDFLAGS)

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

bin/Frame.o: src/Frame.cpp
	g++ $(CFLAGS) -o bin/Frame.o src/Frame.cpp

bin/Game.o: src/Game.cpp
	g++ $(CFLAGS) -o bin/Game.o src/Game.cpp

bin/StompProtocol.o: src/StompProtocol.cpp
	g++ $(CFLAGS) -o bin/StompProtocol.o src/StompProtocol.cpp

bin/User.o: src/User.cpp
	g++ $(CFLAGS) -o bin/User.o src/User.cpp

bin/Summary.o: src/Summary.cpp
	g++ $(CFLAGS) -o bin/Summary.o src/Summary.cpp
.PHONY: clean
clean:
	rm -f bin/*
	