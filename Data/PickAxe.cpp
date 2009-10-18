#include "PickAxe.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, PickAxe*> PickAxe::pick_axes;

PickAxe::PickAxe(const string& name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void PickAxe::addToMap(const string& name, PickAxe* pick_axe) {
	PickAxe::pick_axes[name] = pick_axe;
	Weapon::addToMap(name, pick_axe);
}

void PickAxe::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new PickAxe(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void PickAxe::init() {
	/* pick axes */
	create("pick-axe", 50, 100, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_IRON, '(', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("dwarvish mattock", 50, 120, Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 2, 6), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("broad pick", pick_axes["dwarvish mattock"]);
}
