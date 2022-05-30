#pragma once
#include "../actor/actor.h"

class FairyMaze {
private:
	Fairyland maze;
	
	Map ivanMap;
	Actor ivan;
	Map elenaMap;
	Actor elena;
	
	Map commonMap;
	Vector mapsOffset;
	bool charactersGoEachOther;
	std::vector<Vector> possibleLocationsMaps;
	
	bool meet;
	int turnCountBeforeMeeting;
	bool mapFullyExplored;
	
	void setPossibleLocationsMaps(Direction ivanDirection, Direction elenaDirection);

public:
	FairyMaze();
	
	int getTurnCount();
	
	int getTurnCountBeforeMeeting() const;
	
	bool ableCombineMaps();
	
	bool checkMeet() const;
	
	void writeMap();
	
	bool makeTurn();
};
