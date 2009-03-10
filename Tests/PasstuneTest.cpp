#include <string>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Analyzer.h"
#include "Debug.h"

#include "Analyzers/Passtune.h"
#include "MockSaiph.h"

using namespace std;

class PasstuneTest : public Passtune {
	public:
		PasstuneTest(MockSaiph *saiph) : Passtune((Saiph *)NULL), saiph(saiph) {}

		MockSaiph *saiph;
		using Passtune::discarded;
		using Passtune::nextGuess;
		using Passtune::guess;
};

int main() {
	const char *passtune = "DEADA";
	srandom(time(0)+getpid());

	string logfile = "PasstuneTest.log";

	Debug::open(logfile);

	MockSaiph *saiph = new MockSaiph();
	PasstuneTest *a = new PasstuneTest(saiph);

	int possibilities;

	possibilities = a->nextGuess(UNKNOWN, UNKNOWN);

	assert(possibilities > 0);

	while (true) {
		char *check = strdup(passtune);
		int gears = 0;
		int tumblers = 0;
		bool used[PLACES];

		for (int x = 0; x < PLACES; ++x) {
			if (check[x] == a->guess[x]) {
				gears++;
				check[x] = -1;
				cout << "gear: " << a->guess << '[' << x << "] matches " << passtune << '[' << x << ']' << endl;
				used[x] = true;
			} else
				used[x] = false;
		}

		if (gears == PLACES)
			break;

		for (int x = 0; x < PLACES; ++x) {
			if (!used[x]) {
			for (int y = 0; y < PLACES; ++y) {
				if (a->guess[x] == check[y]) {
					cout << "tumbler: " << a->guess << '[' << x << "] matches " << passtune << '[' << y << ']' << endl;
					tumblers++;
					check[y] = -1;
					break;
				}
			}
			}
		}

		free(check);
		cout << "From " << a->guess << " got " << gears << " gears and " << tumblers << " tumblers." << endl;

		possibilities = a->nextGuess(gears, tumblers);

		assert(possibilities > 0);
	}

	cout << "Correct guess of " << a->guess << endl;
	Debug::close();
}
