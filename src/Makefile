CXXFLAGS=-Wall -g -pthread
LDLIBS=-lboost_system

OBJS=hades.o minecraft_listener.o minecraft_connection.o http_listener.o http_connection.o

hades: $(OBJS)
	g++ -pthread $(OBJS) $(LDLIBS) -o hades
