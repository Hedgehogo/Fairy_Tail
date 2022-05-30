#include "fairyMaze.h"

FairyMaze::FairyMaze() :
	ivanMap(), ivan(Character::Ivan, ivanMap, maze), elenaMap(), elena(Character::Elena, elenaMap, maze),
	commonMap(), possibleLocationsMaps(), charactersGoEachOther(false), mapFullyExplored(false), meet(false), turnCountBeforeMeeting(0) {
	commonMap.clear();
}

void FairyMaze::setPossibleLocationsMaps(Direction ivanDirection, Direction elenaDirection) {
	Vector ivanPosition{ivan.getPosition()};
	Vector elenaPosition{elena.getPosition()};
	Vector location{ivanPosition.x - elenaPosition.x, ivanPosition.y - elenaPosition.y};
	//Add a situation in which the characters stand on the same cell
	std::vector<Vector> possibleLocations{1, location};
	
	//Add situations in which the characters passed through each other
	if(ivanDirection == Direction::Down && elenaDirection == Direction::Up) {
		Vector additionalLocation{location.x, location.y - 1};
		possibleLocations.push_back(additionalLocation);
	} else if(ivanDirection == Direction::Up && elenaDirection == Direction::Down) {
		Vector additionalLocation{location.x, location.y + 1};
		possibleLocations.push_back(additionalLocation);
	} else if(ivanDirection == Direction::Right && elenaDirection == Direction::Left) {
		Vector additionalLocation{location.x - 1, location.y};
		possibleLocations.push_back(additionalLocation);
	} else if(ivanDirection == Direction::Left && elenaDirection == Direction::Right) {
		Vector additionalLocation{location.x + 1, location.y};
		possibleLocations.push_back(additionalLocation);
	}
	
	if(!possibleLocationsMaps.empty()) {
		//Looking for a match with past data
		std::vector<Vector> result;
		for(auto &possibleLocationMaps: possibleLocationsMaps) {
			bool locationSuitable{false};
			for(const auto &possibleLocation: possibleLocations) {
				if(possibleLocationMaps.x == possibleLocation.x && possibleLocationMaps.y == possibleLocation.y) {
					locationSuitable = true;
				}
			}
			if(locationSuitable) {
				result.push_back(possibleLocationMaps);
			}
		}
		possibleLocationsMaps = result;
	} else {
		possibleLocationsMaps = possibleLocations;
	}
}

int FairyMaze::getTurnCount() {
	return maze.getTurnCount();
}

int FairyMaze::getTurnCountBeforeMeeting() const {
	return turnCountBeforeMeeting;
}

bool FairyMaze::ableCombineMaps() {
	return !commonMap.empty();
}

bool FairyMaze::checkMeet() const {
	return this->meet;
}

void FairyMaze::writeMap() {
	if(!commonMap.empty()) {
		commonMap.print(Vector{}, mapsOffset);
	} else {
		ivan.printMap();
		elena.printMap();
	}
}

bool FairyMaze::makeTurn() {
	//If you managed to generate a common map and managed to build a path of characters to each other, then set the characters this path.
	if(!meet && !charactersGoEachOther && !commonMap.empty()) {
		Path way = commonMap.findShortestWay(ivan.getPosition(), elena.getPosition());
		if(!way.isEmpty()) {
			charactersGoEachOther = true;
			ivan.setPath(way.getFirstHalf());
			elena.setPath(way.getSecondHalf().inverse());
		}
	}
	
	//Both characters must be able to make a move.
	//If at least one of them couldn't, and they still haven't met.
	//Then they are isolated from each other and the problem has no solution.
	bool ivanTurn = ivan.makeTurn();
	bool elenaTurn = elena.makeTurn();
	if(ivanTurn || elenaTurn) {
		//Made a move - check if the characters have not met
		if(maze.go(ivan.getTurnDirection(), elena.getTurnDirection())) {
			if(!this->meet) {
				turnCountBeforeMeeting = maze.getTurnCount();
			}
			this->meet = true;
			if(commonMap.empty()) {
				setPossibleLocationsMaps(ivan.getTurnDirection(), elena.getTurnDirection());
			}
		}
		//Characters have moved - study the cells around them
		ivan.checkNextCells();
		elena.checkNextCells();
	} else {
		mapFullyExplored = true;
	}
	
	//If a common map has not yet been created, then we try to create it.
	if(commonMap.empty()) {
		if(possibleLocationsMaps.empty()) {
			mapsOffset = commonMap.mergeMaps(ivan.getMap(), elena.getMap());
		} else {
			mapsOffset = commonMap.mergeMaps(ivan.getMap(), elena.getMap(), possibleLocationsMaps);
		}
		//If it was possible to create a common map, then give it to both characters, now the characters can study the map more effectively, because they know what the other has already studied.
		if(!commonMap.empty()) {
			ivan.setMap(commonMap, Vector{});
			elena.setMap(commonMap, mapsOffset);
		}
	}
	
	if(!mapFullyExplored && (!meet || maze.getTurnCount() < 250)) {
		return true;
	}
	return false;
}
