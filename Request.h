#ifndef REQUEST_H
#define REQUEST_H
/* requests */
#define ILLEGAL_REQUEST 0
#define REQUEST_APPLY_UNIHORN 1
#define REQUEST_ELBERETH_OR_REST 2
#define REQUEST_UPDATED_INVENTORY 3
#define REQUEST_DIRTY_INVENTORY 4
#define REQUEST_DIRTY_STASH 5
#define REQUEST_EAT 6
#define REQUEST_PRAY 7
#define REQUEST_ARMOR_WEAR 8
#define REQUEST_ITEM_GROUP_SET_AMOUNT 9
#define REQUEST_ITEM_GROUP_ADD 10
#define REQUEST_ITEM_PICKUP 11
#define REQUEST_WEAPON_WIELD 12
#define REQUEST_ADD_THROWN_WEAPON 13
#define REQUEST_RING_WEAR 14
#define REQUEST_AMULET_WEAR 15
#define REQUEST_QUAFF_HEALING 16
#define REQUEST_CALL_ITEM 17
#define REQUEST_NAME_ITEM 18
#define REQUEST_BEATIFY_ITEMS 19
#define REQUEST_BECOME_BLIND 20
#define REQUEST_UNBLIND 21

#include <string>
#include "Coordinate.h"

class Request {
	public:
		int request;
		int priority;
		int value;
		int beatitude;
		bool sustain;
		bool only_unknown_enchantment;
		unsigned char key;
		std::string data;
		Coordinate coordinate;

		Request();
};
#endif
