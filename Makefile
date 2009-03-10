CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -gdwarf-2
LDFLAGS = -lutil -g3 -gdwarf-2
OBJECTS = Analyzer.o Connection.o Coordinate.o Debug.o Item.o Level.o Local.o Monster.o PathNode.o Player.o Point.o Request.o Saiph.o Stash.o Telnet.o World.o

saiph: $(OBJECTS) Analyzers/*.h Analyzers/*.cpp Data/*.h Data/*.cpp
	$(MAKE) -C Analyzers
	$(MAKE) -C Data
	$(CXX) $(OBJECTS) Analyzers/*.o Data/*.o $(LDFLAGS) -o saiph

Analyzer.o: Analyzer.h Analyzer.cpp
Connection.o: Connection.h Connection.cpp
Coordinate.o: Coordinate.h Coordinate.cpp
Debug.o: Debug.h Debug.cpp
Item.o: Item.h Item.cpp
Level.o: Level.h Level.cpp
Local.o: Local.h Local.cpp
Monster.o: Monster.h Monster.cpp
PathNode.o: PathNode.h PathNode.cpp
Player.o: Player.h Player.cpp
Point.o: Point.h Point.cpp
Request.o: Request.h Request.cpp
Saiph.o: Saiph.h Saiph.cpp
Stash.o: Stash.h Stash.cpp
Telnet.o: Telnet.h Telnet.cpp
World.o: World.h World.cpp

.PHONY: clean test
clean:
	$(RM) *.o *.gch saiph
	$(MAKE) -C Analyzers clean
	$(MAKE) -C Data clean
	$(MAKE) -C Tests clean

test:
	$(MAKE) -C Tests

#Launch game
game:
	echo -e '\e[8;50;160t' # Sets the terminal size
	./saiph 2> err.log
