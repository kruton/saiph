#include "Armor.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Armor::Armor(Saiph *saiph) : Analyzer("Armor"), saiph(saiph), wear_armor(false) {
}

/* methods */
void Armor::parseMessages(const string &messages) {
	string::size_type pos;
	if (saiph->world->question && !command2.empty() && (messages.find(MESSAGE_WHAT_TO_WEAR, 0) != string::npos || messages.find(MESSAGE_WHAT_TO_TAKE_OFF, 0) != string::npos)) {
		/* take off or wear something */
		command = command2;
		command2.clear();
		priority = PRIORITY_CONTINUE_ACTION;
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (!saiph->world->question && !command2.empty() && messages.find(MESSAGE_YOU_WERE_WEARING, 0) != string::npos) {
		/* took off last piece of armor (no "what do you want to take off?" question then) */
		command2.clear();
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (saiph->world->question && (pos = messages.find(MESSAGE_FOOCUBUS_QUESTION, 0)) != string::npos) {
		/* A foocubus wants to remove something. Check the beatitude of
		 * that piece of armor! */
		command = NO;
		priority = PRIORITY_CONTINUE_ACTION;
		if (messages.find(" cloak, ", pos) == pos
				|| messages.find(" wrapping, ", pos) == pos
				|| messages.find(" robe, ", pos) == pos
				|| messages.find(" apron, ", pos) == pos
				|| messages.find(" smock, ", pos) == pos) {
			/* Remove the cloak? (yes, these are all the cases) */
			/* also take off cloak if suit or shirt is cursed */
			if (isCursed(ARMOR_CLOAK) || isCursed(ARMOR_SUIT) || isCursed(ARMOR_SHIRT))
				command = YES;
		} else if (messages.find(" suit, ", pos) == pos) {
			/* Remove the suit? */
			/* also take off suit if shirt is cursed */
			if (isCursed(ARMOR_SUIT) || isCursed(ARMOR_SHIRT))
				command = YES;
		} else if (messages.find(" boots, ", pos) == pos) {
			/* Remove the boots? */
			if (isCursed(ARMOR_BOOTS))
				command = YES;
		} else if (messages.find(" gloves, ", pos) == pos) {
			/* Remove the gloves? */
			if (isCursed(ARMOR_GLOVES))
				command = YES;
		} else if (messages.find(" shield, ", pos) == pos) {
			/* Remove the shield? */
			if (isCursed(ARMOR_SHIELD))
				command = YES;
		} else if (messages.find(" helmet, ", pos) == pos) {
			/* Remove the helmet? */
			if (isCursed(ARMOR_HELMET))
				command = YES;
		} else if (messages.find(" shirt, ", pos) == pos) {
			/* Remove the shirt? */
			if (isCursed(ARMOR_SHIRT))
				command = YES;
		}
		/* Request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (messages.find(MESSAGE_FOOCUBUS_REMOVE, 0) != string::npos) {
		/* A foocubus removed something without asking us. */
		/* Request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (messages.find(MESSAGE_YOU_FINISH_TAKING_OFF, 0) != string::npos) {
		/* took of last armor, mark inventory dirty */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (saiph->inventory_changed || wear_armor) {
		wearArmor();
	}
}

bool Armor::request(const Request &request) {
	if (request.request == REQUEST_ARMOR_WEAR) {
		/* player wish to wear this armor */
		if (request.key < 0 || request.key >= ARMOR_SLOTS)
			return false;
		carry_amount[request.key] = request.value;
		ArmorData ad;
		ad.beatitude = request.beatitude;
		ad.priority = request.priority;
		ad.keep = request.sustain;
		ad.name = request.data;
		armor[request.key].push_back(ad);
		/* tell the loot analyzer to pick up this armor too */
		req.request = REQUEST_ITEM_PICKUP;
		req.value = request.value;
		req.beatitude = request.beatitude | BEATITUDE_UNKNOWN;
		req.data = request.data;
		saiph->request(req);
		return true;
	}
	return false;
}

/* private methods */
bool Armor::isCursed(int armor_slot) {
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (i->second.additional != "being worn")
			continue;
		for (vector<ArmorData>::size_type a = 0; a < armor[armor_slot].size(); ++a) {
			if (armor[armor_slot][a].name != i->second.name)
				continue;
			return i->second.beatitude == CURSED;
		}
	}

	/* Couldn't find something on that slot? */
	return false;
}

void Armor::wearArmor() {
	/* put on or change armor (which means we'll have to take something off) */
	/* check that we're (still) wearing our preferred armor */
	unsigned char worn[ARMOR_SLOTS] = {0};
	unsigned char best_key[ARMOR_SLOTS] = {0};
	int best_armor[ARMOR_SLOTS];
	for (int s = 0; s < ARMOR_SLOTS; ++s)
		best_armor[s] = INT_MIN;
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		for (int s = 0; s < ARMOR_SLOTS; ++s) {
			for (vector<ArmorData>::iterator a = armor[s].begin(); a != armor[s].end(); ++a) {
				if (a->name != i->second.name)
					continue;
				else if (i->second.additional == "being worn")
					worn[s] = i->first;
				if (i->second.beatitude == BEATITUDE_UNKNOWN) {
					/* armor with unknown beatitude, request it beatified */
					req.request = REQUEST_BEATIFY_ITEMS;
					saiph->request(req);
				}
				int score = priority + i->second.enchantment - i->second.damage;
				if (i->second.unknown_enchantment)
					score += ARMOR_UNKNOWN_ENCHANTMENT_BONUS;
				if (score <= best_armor[s])
					continue;
				else if ((a->beatitude & i->second.beatitude) == 0)
					continue;
				best_key[s] = i->first;
				best_armor[s] = score;
			}
		}
	}

	wear_armor = false;
	for (int s = 0; s < ARMOR_SLOTS; ++s) {
		if (best_key[s] == 0 || (worn[s] != 0 && saiph->inventory[worn[s]].name == saiph->inventory[best_key[s]].name))
			continue; // wearing best armor or got no armor to wield
		if (isCursed(s))
			continue; // the item we're wearing in this slot it cursed and cannot be taken off
		if (s == ARMOR_SUIT || s == ARMOR_SHIRT) {
			/* wish to put on shirt or suit.
			 * if cloak or suit is cursed, this can't be done */
			if (isCursed(ARMOR_CLOAK) || (s == ARMOR_SHIRT && isCursed(ARMOR_SUIT))) {
				/* cloak is cursed, or suit is cursed and we're putting on a shirt */
				continue;
			}
			/* are we wearing a cloak? */
			if (worn[ARMOR_CLOAK] != 0) {
				/* yes, we must take it off first */
				command = TAKE_OFF;
				command2 = worn[ARMOR_CLOAK];
				wear_armor = true;
				break;
			}
			if (s == ARMOR_SHIRT) {
				/* are we wearing a suit? */
				if (worn[ARMOR_SUIT] != 0) {
					/* yes, we must take it off first */
					command = TAKE_OFF;
					command2 = worn[ARMOR_SUIT];
					wear_armor = true;
					break;
				}
			}
		}
		if (worn[s] != 0) {
			/* we'll have to take this armor off first */
			command = TAKE_OFF;
			command2 = worn[s];
			wear_armor = true;
			break;
		}
		/* we should put on this piece of armor */
		command = WEAR;
		command2 = best_key[s];
		wear_armor = true;
		break;
	}
	if (wear_armor) {
		priority = PRIORITY_ARMOR_WEAR;
		/* tell Loot to drop unwanted armor */
		for (int s = 0; s < ARMOR_SLOTS; ++s) {
			int worn_priority_modifier = 0;
			map<unsigned char, Item>::iterator i = saiph->inventory.find(worn[s]);
			if (i != saiph->inventory.end())
				worn_priority_modifier += i->second.enchantment - i->second.damage;
			for (vector<ArmorData>::iterator a = armor[s].begin(); a != armor[s].end(); ++a) {
				req.request = REQUEST_ITEM_PICKUP;
				req.beatitude = a->beatitude | BEATITUDE_UNKNOWN;
				req.data = a->name;
				if (a->keep || a->priority + ARMOR_UNKNOWN_ENCHANTMENT_BONUS + worn_priority_modifier >= best_armor[s]) {
					/* we [still] want this armor.
					 * in case we lost good armor and now wear less good
					 * armor we'll need to tell Loot to pick up this armor */
					req.value = carry_amount[s];
				} else {
					/* we don't want to keep this armor and it'll never
					 * be better than the armor we currently got */
					req.value = 0;
				}
				saiph->request(req);
			}
		}
	} else {
		/* nothing to wear */
		command.clear();
	}
}
