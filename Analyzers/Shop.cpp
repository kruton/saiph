#include <stdlib.h>
#include "Shop.h"
#include "../Debug.h"
#include "../Globals.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Shop::Shop(Saiph *saiph) : Analyzer("Shop"), saiph(saiph), drop_pick_axe(false), look_at_ground(false) {
}

/* methods */
void Shop::parseMessages(const string &messages) {
	if (messages.find(SHOP_MESSAGE_LEAVE_TOOL, 0) != string::npos || messages.find(SHOP_MESSAGE_LEAVE_TOOL_ANGRY, 0) != string::npos) {
		/* we're most likely standing in a doorway, next to a shopkeeper.
		 * head for nearest CORRIDOR or FLOOR and drop pick-axe */
		unsigned char dir = ILLEGAL_DIRECTION;
		const PathNode &node = saiph->shortestPath(CORRIDOR);
		dir = node.dir;
		if (node.cost >= UNPASSABLE) {
			const PathNode &node2 = saiph->shortestPath(FLOOR);
			if (node2.cost >= UNPASSABLE) {
				/* this is bad */
				Debug::warning(saiph->last_turn) << SHOP_DEBUG_NAME << "Unable to path to CORRIDOR or FLOOR from shopkeeper" << endl;
				return;
			}
			dir = node2.dir;
		}

		drop_pick_axe = true;
		command = dir;
		priority = PRIORITY_SHOP_DROP_DIGGING_TOOL;
	} else if (saiph->world->question && messages.find(MESSAGE_WHAT_TO_DROP, 0) == 0 && drop_pick_axe) {
		/* drop our tools */
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.name != "pick-axe" && i->second.name != "mattock")
				continue;
			command = i->first;
			priority = PRIORITY_CONTINUE_ACTION;
			drop_pick_axe = false;
			look_at_ground = true;
			return;
		}
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (drop_pick_axe && saiph->getDungeonSymbol() != OPEN_DOOR) {
		/* we should've moved away from shopkeeper now, drop the pick-axe */
		command = DROP;
		priority = PRIORITY_SHOP_DROP_DIGGING_TOOL;
	} else if (look_at_ground) {
		/* we'll look at ground after dropping the tools.
		 * this makes us aware of the stash,
		 * and the loot analyzer won't "visit" the stash after we move */
		/* FIXME:
		 * possibly not safe. what if another analyzer do something
		 * with higher priority?
		 * unlikely, but may be possible */
		command = LOOK;
		priority = PRIORITY_LOOK;
		look_at_ground = false;
	}
}

void Shop::analyze() {
	unsigned char symbol = saiph->getDungeonSymbol();
	/* FIXME:
	 * this currently bugs. she's marking SHOP_TILE as FLOOR
	 * and picks up items. why? */
//	if (symbol == SHOP_TILE) {
//		/* if we're standing on SHOP_TILE, check if we can see the shopkeeper.
//		 * if we can't we probably killed him/her, and then we should remove the SHOP_TILE */
//		bool shopkeeper_seen = false;
//		for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
//			if (!m->second.shopkeeper || !m->second.visible)
//				continue;
//			shopkeeper_seen = true;
//			break;
//		}
//		if (!shopkeeper_seen) {
//			/* can't see any shopkeeper, make the tile FLOOR */
//			saiph->levels[saiph->position.level].setDungeonSymbol(FLOOR);
//			/* we should also look at floor so Loot gets a chance to
//			 * pick up the item as the last time Loot checked, this
//			 * tile was a SHOP_TILE */
//			command = LOOK;
//			priority = PRIORITY_CONTINUE_ACTION;
//			/* return because there's no need to check for a shopkeeper again */
//			return;
//		}
//	}
	if (symbol != FLOOR && symbol != UNKNOWN_TILE)
		return; // not standing on FLOOR or UNKNOWN_TILE, no shop here (or detected already)

	for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
		if (!m->second.shopkeeper || !m->second.visible)
			continue;

		/* figure out if we're in the same room as the shopkeeper */
		Point nw = saiph->position;
		Point se = saiph->position;

		/* find north wall */
		symbol = saiph->getDungeonSymbol();
		while ((symbol == FLOOR || symbol == UNKNOWN_TILE) && --nw.row)
			symbol = saiph->getDungeonSymbol(nw);
		++nw.row;

		/* find west wall */
		symbol = saiph->getDungeonSymbol();
		while ((symbol == FLOOR || symbol == UNKNOWN_TILE) && --nw.col)
			symbol = saiph->getDungeonSymbol(nw);
		++nw.col;

		/* find south wall */
		symbol = saiph->getDungeonSymbol();
		while ((symbol == FLOOR || symbol == UNKNOWN_TILE) && ++se.row)
			symbol = saiph->getDungeonSymbol(se);
		--se.row;

		/* find east wall */
		symbol = saiph->getDungeonSymbol();
		while ((symbol == FLOOR || symbol == UNKNOWN_TILE) && ++se.col)
			symbol = saiph->getDungeonSymbol(se);
		--se.col;

		if (m->first.row < nw.row || m->first.col < nw.col || m->first.row > se.row || m->first.col > se.col)
			return; // we're not in the same room as the shopkeeper

		/* check that area assumed to be a shop does not contain VERTICAL_WALL, HORIZONTAL_WALL or ALTAR */
		Point p;
		for (p.row = nw.row; p.row <= se.row; ++p.row) {
			for (p.col = nw.col; p.col <= se.col; ++p.col) {
				symbol = saiph->getDungeonSymbol(p);
				if (symbol == VERTICAL_WALL || symbol == HORIZONTAL_WALL || symbol == ALTAR)
					return; // detected too large area as shop
			}
		}

		Debug::notice(saiph->last_turn) << SHOP_DEBUG_NAME << "bounds are " << nw << " to " << se << endl;

		/* mark all tiles within boundaries as SHOP_TILE */
		for (p.row = nw.row; p.row <= se.row; ++p.row) {
			for (p.col = nw.col; p.col <= se.col; ++p.col)
				saiph->setDungeonSymbol(p, SHOP_TILE);
		}
		/* we should LOOK at floor to prevent Loot from picking
		 * up an item if we fall into a shop on an item we want
		 * before we get to mark the tiles as SHOP_TILE */
		command = LOOK;
		priority = PRIORITY_CONTINUE_ACTION;
	}
}
