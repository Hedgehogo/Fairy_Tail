#pragma once
#include "../map/map.h"

class Actor {
private:
	Character character;
	Fairyland& maze;
	Map* map;
	Vector currentCell;
	Path path;
	Direction nextTurn;
	
	Vector getNextCellCoord(Direction direction);
	
	void checkNextCell(Direction direction);
	
	void moveToDirection(Direction direction);

public:
	Actor(Character character, Map &map, Fairyland &maze);
	
	void setMap(Map& map, Vector offset);
	
	void setPath(Path path);
	
	Direction getTurnDirection();
	
	Vector getPosition();
	
	Map& getMap();
	
	void printMap();
	
	bool makeTurn();
	
	void checkNextCells();
};
