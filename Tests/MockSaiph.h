#ifndef FAKESAIPH_H
#define FAKESAIPH_H

#include <string>

class MockSaiph {
	public:
		MockSaiph();
		int last_turn;
};

MockSaiph::MockSaiph(void) : last_turn(0) {}

#endif
