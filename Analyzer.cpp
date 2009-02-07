#include "Analyzer.h"
#include "Globals.h"

using namespace std;

/* constructors/destructor */
Analyzer::Analyzer(string name) : name(name), priority(ILLEGAL_PRIORITY), command(""), expired(false) {
	/* methods will be called in this order:
	 * 0. init (once, after all analyzers have been created)
	 * 1. parseMessages
	 * 2. analyze (unless question or menu)
	 * 3. complete/fail (only called for analyzer that got its command through)
	 *
	 * additionally, requests may come at any time */
#if 1 /* FLOW_CONTROL */
	ignore_next_prompts = 0;
#endif
}

Analyzer::~Analyzer() {
	/* we must have a virtual destructor.
	 * if we don't then the destructor of classes inheriting
	 * this class won't be called upon destruction */
}

/* methods */
void Analyzer::analyze() {
}

void Analyzer::complete() {
}

void Analyzer::fail() {
}

void Analyzer::init() {
}

void Analyzer::parseMessages(const string &messages) {
}

bool Analyzer::request(const Request &request) {
	return false;
}
