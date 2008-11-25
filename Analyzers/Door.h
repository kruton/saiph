#ifndef DOOR_H
#define DOOR_H
/* priorities */
#define DOOR_OPEN_PRIORITY 150
/* messages */
#define DOOR_DOOR_LOCKED "  This door is locked.  "
#define DOOR_UNLOCK_IT "  Unlock it? [yn] (n)  "

#include <string>
#include "../Analyzer.h"
#include "../Point.h"

class Saiph;

class Door : public Analyzer {
	public:
		Door(Saiph *saiph);

		void analyze();
		void complete();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		std::string command2;
		Point position;
		int sequence;
		unsigned char unlock_tool_key;
		bool in_a_pit;

		void findUnlockingTool();
};
#endif
