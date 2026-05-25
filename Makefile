.PHONY:all clean
all:server client
server:server.cc
	g++ -o $@ $^ -std=c++11
client:client.cc
	g++ -o $@ $^ -std=c++11

clean:
	rm -f server client