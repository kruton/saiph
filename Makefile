CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g3 -pg -gdwarf-2
LDFLAGS = -lutil -g3 -pg -gdwarf-2
OBJECTS = Analyzer.o Connection.o Coordinate.o Debug.o Item.o Level.o Local.o Monster.o Player.o Point.o Request.o Saiph.o Stash.o Telnet.o World.o
LIBRARIES = Analyzers/Analyzers.a

all: saiph

analyzers:
	$(MAKE) -C Analyzers

saiph: $(OBJECTS) analyzers $(LIBRARIES)
	$(CXX) $(OBJECTS) $(LIBRARIES) $(LDFLAGS) -o saiph

Analyzer.o: Analyzer.h Analyzer.cpp
Connection.o: Connection.h Connection.cpp
Coordinate.o: Coordinate.h Coordinate.cpp
Debug.o: Debug.h Debug.cpp
Item.o: Item.h Item.cpp
Level.o: Level.h Level.cpp
Local.o: Local.h Local.cpp
Monster.o: Monster.h Monster.cpp
Player.o: Player.h Player.cpp
Point.o: Point.h Point.cpp
Request.o: Request.h Request.cpp
Saiph.o: Saiph.h Saiph.cpp
Stash.o: Stash.h Stash.cpp
Telnet.o: Telnet.h Telnet.cpp
World.o: World.h World.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch saiph
	$(MAKE) -C Analyzers clean

#Launch game
game:
	echo -e '\e[8;50;160t' # Sets the terminal size
	./saiph 2> err.log
