#include "map.h"
#include <cmath>

Map::Map() : map({{ Cell{State::Empty} }}), size(1, 1), offset(0, 0), boundariesDetected(false) {}

void Map::detectBoundaries() {
	if(boundariesDetected) {
		return;
	}
	if(size.x >= 12) {
		for(auto& cell : map[0]) {
			cell.state = State::Wall;
		}
		for(auto& cell : map[map.size() - 1]) {
			cell.state = State::Wall;
		}
	}
	if(size.y >= 12) {
		for(auto& line : map) {
			line[0].state = State::Wall;
		}
		for(auto& line : map) {
			line[line.size() - 1].state = State::Wall;
		}
	}
	if(size.x >= 12 && size.y >= 12) {
		boundariesDetected = true;
	}
}

Cell Map::getCell(int x, int y) const {
	if(x < offset.x || x >= offset.x + size.x ||
		y < offset.y || y >= offset.y + size.y) {
		return Cell(State::Unknown);
	}
	return map[x - offset.x][y - offset.y];
}

Cell Map::getCell(Vector position) const {
	return getCell(position.x, position.y);
}

void Map::setCell(int x, int y, Cell cell) {
	bool sizeChanged{false};
	if(x < offset.x) {
		map.insert(map.begin(), std::vector<Cell>(size.y, Cell()));
		--offset.x;
		++size.x;
		sizeChanged = true;
	}
	if(x >= offset.x + size.x) {
		map.emplace_back(size.y, Cell());
		++size.x;
		sizeChanged = true;
	}
	if(y < offset.y) {
		for(auto& line : map) {
			line.insert(line.begin(), Cell());
		}
		--offset.y;
		++size.y;
		sizeChanged = true;
	}
	if(y >= offset.y + size.y) {
		for(auto& line : map) {
			line.emplace_back();
		}
		++size.y;
		sizeChanged = true;
	}
	if(sizeChanged) {
		detectBoundaries();
	}
	map[x - offset.x][y - offset.y] = cell;
}

void Map::setCell(Vector position, Cell cell) {
	setCell(position.x, position.y, cell);
}

bool Map::empty() {
	return map.empty();
}

void Map::clear() {
	map.clear();
	size = Vector{0, 0};
	offset = Vector{0, 0};
}

Vector Map::excludePossibleBoundaries(std::vector<std::vector<Cell>>& map) {
	Vector offset{0, 0};
	
	bool upperEdge{true};
	for(auto& line : map) {
		if(line[0].state == State::Empty) {
			upperEdge = false;
		}
	}
	if(upperEdge) {
	 	++offset.y;
		for(auto& line : map) {
			line.erase(line.begin());
		}
	}
	
	bool lowerEdge{true};
	for(auto& line : map) {
		if(line[line.size() - 1].state == State::Empty) {
			lowerEdge = false;
		}
	}
	if(lowerEdge) {
		for(auto & x : map) {
			x.pop_back();
		}
	}
	
	bool  leftEdge{true};
	for(auto& cell : map[0]) {
		if(cell.state == State::Empty) {
			leftEdge = false;
		}
	}
	if(leftEdge) {
	 	++offset.x;
		map.erase(map.begin());
	}
	
	bool rightEdge{true};
	for(auto& cell : map[map.size() - 1]) {
		if(cell.state == State::Empty) {
			rightEdge = false;
		}
	}
	if(rightEdge) {
		map.pop_back();
	}
	
	return offset;
}

void Map::print(Vector characterPosition, Character character) {
	std::vector<std::vector<Cell>> writeMap{map};
	Vector writeOffset = excludePossibleBoundaries(writeMap);
	writeOffset = Vector{offset.x + writeOffset.x, offset.y + writeOffset.y};
	Vector writeSize = Vector(static_cast<int>(writeMap.size()), static_cast<int>(writeMap[0].size()));
	
	for(int y = 0; y < writeSize.y; ++y) {
		for(int x = 0; x < writeSize.x; ++x) {
			if(x == characterPosition.x - writeOffset.x && y == characterPosition.y - writeOffset.y) {
				if(character == Character::Ivan) {
					std::cout << "@";
				} else {
					std::cout << "&";
				}
			} else {
				switch(writeMap[x][y].state) {
					case State::Wall:
						std::cout << "#";
						break;
					case State::Empty:
						std::cout << ".";
						break;
					case State::Unknown:
						std::cout << "?";
						break;
				}
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void Map::print(Vector ivanPosition, Vector elenaPosition) {
	std::vector<std::vector<Cell>> writeMap{map};
	Vector writeOffset{excludePossibleBoundaries(writeMap)};
	writeOffset = Vector{offset.x + writeOffset.x, offset.y + writeOffset.y};
	Vector writeSize = Vector(static_cast<int>(writeMap.size()), static_cast<int>(writeMap[0].size()));
	
	for(int y = 0; y < writeSize.y; ++y) {
		for(int x = 0; x < writeSize.x; ++x) {
			if(x == ivanPosition.x - writeOffset.x && y == ivanPosition.y - writeOffset.y) {
				std::cout << "@";
			} else if (x == elenaPosition.x - writeOffset.x && y == elenaPosition.y - writeOffset.y) {
				std::cout << "&";
			} else {
				switch(writeMap[x][y].state) {
					case State::Wall:
						std::cout << "#";
						break;
					case State::Empty:
						std::cout << ".";
						break;
					case State::Unknown:
						std::cout << "?";
						break;
				}
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

bool Map::cellHasUnknownNeighbors(int x, int y) const {
	return getCell(x, y - 1).state == State::Unknown ||
		   getCell(x + 1, y).state == State::Unknown ||
		   getCell(x, y + 1).state == State::Unknown ||
		   getCell(x - 1, y).state == State::Unknown;
}

bool Map::cellUnexplored(int x, int y) const {
	return getCell(x, y).state == State::Empty && cellHasUnknownNeighbors(x, y);
}

Path Map::findShortestWay(Vector whence, Vector where) {
	//The BFS pathfinding algorithm
	std::vector<std::vector<int>> widthMap = std::vector<std::vector<int>>(size.x, std::vector<int>(size.y, -1));
	widthMap[whence.x - offset.x][whence.y - offset.y] = 0;
	Vector foundCellCoord;
	bool stop{false};
	for(int i = 0; i < 100; ++i) {
		//Search algorithm step
		for(int x = 1; x < size.x - 1; ++x) {
			for(int y = 1; y < size.y - 1; ++y) {
				if(map[x][y].isEmpty() && widthMap[x][y] == -1) {
					if(widthMap[x][y - 1] == i ||
					   widthMap[x + 1][y] == i ||
					   widthMap[x][y + 1] == i ||
					   widthMap[x - 1][y] == i) {
						
						widthMap[x][y] = i + 1;
						if(x == where.x - offset.x && y == where.y - offset.y) {
							foundCellCoord = Vector{x, y};
							stop = true;
						}
					}
				}
				if(stop)
					break;
			}
			if(stop)
				break;
		}
		if(stop)
			break;
	}
	//No unexplored cells were found, so we return an empty path
	if(!stop) {
		return Path{};
	}
	
	//The cell stores the number of cells needed to reach it, hence the length of the path to reach it.
	int pathSize = widthMap[foundCellCoord.x][foundCellCoord.y];
	std::vector<Direction> path(pathSize, Direction::Pass);
	//Making a path based on a width map
	for(int i = 0; i < pathSize; ++i) {
		int cellValue = widthMap[foundCellCoord.x][foundCellCoord.y];
		if(widthMap[foundCellCoord.x][foundCellCoord.y + 1] == cellValue - 1) {
			foundCellCoord.y += 1;
			path[i] = Direction::Up;
		} else if(widthMap[foundCellCoord.x + 1][foundCellCoord.y] == cellValue - 1) {
			foundCellCoord.x += 1;
			path[i] = Direction::Left;
		} else if(widthMap[foundCellCoord.x][foundCellCoord.y - 1] == cellValue - 1) {
			foundCellCoord.y -= 1;
			path[i] = Direction::Down;
		} else if(widthMap[foundCellCoord.x - 1][foundCellCoord.y] == cellValue - 1) {
			foundCellCoord.x -= 1;
			path[i] = Direction::Right;
		}
	}
	return Path{path};
}

Path Map::findShortestWayToClosestUnknown(Vector whence) {
	if(cellUnexplored(whence.x, whence.y - 1)) {
		return Path{{Direction::Up}};
	}
	if(cellUnexplored(whence.x + 1, whence.y)) {
		return Path{{Direction::Right}};
	}
	if(cellUnexplored(whence.x, whence.y + 1)) {
		return Path{{Direction::Down}};
	}
	if(cellUnexplored(whence.x - 1, whence.y)) {
		return Path{{Direction::Left}};
	}
	
	//The BFS pathfinding algorithm
	std::vector<std::vector<int>> widthMap = std::vector<std::vector<int>>(size.x, std::vector<int>(size.y, -1));
	widthMap[whence.x - offset.x][whence.y - offset.y] = 0;
	Vector foundCellCoord;
	bool stop{false};
	for(int i = 0; i < 100; ++i) {
		//Search algorithm step
		for(int x = 1; x < size.x - 1; ++x) {
			for(int y = 1; y < size.y - 1; ++y) {
				if(map[x][y].isEmpty() && widthMap[x][y] == -1) {
					if(widthMap[x][y - 1] == i ||
					   widthMap[x + 1][y] == i ||
					   widthMap[x][y + 1] == i ||
					   widthMap[x - 1][y] == i) {
						
						widthMap[x][y] = i + 1;
						if(cellUnexplored(x + offset.x, y + offset.y - 1) ||
						   cellUnexplored(x + offset.x + 1, y + offset.y) ||
						   cellUnexplored(x + offset.x, y + offset.y + 1) ||
						   cellUnexplored(x + offset.x - 1, y + offset.y)) {
							foundCellCoord = Vector{x, y};
							stop = true;
						}
					}
				}
				if(stop)
					break;
			}
			if(stop)
				break;
		}
		if(stop)
			break;
	}
	//No unexplored cells were found, so we return an empty path
	if(!stop) {
		return Path{};
	}
	
	//The cell stores the number of cells needed to reach it, hence the length of the path to reach it.
	int pathSize = widthMap[foundCellCoord.x][foundCellCoord.y];
	std::vector<Direction> path(pathSize, Direction::Pass);
	//Making a path based on a width map
	for(int i = 0; i < pathSize; ++i) {
		int cellValue = widthMap[foundCellCoord.x][foundCellCoord.y];
		if(widthMap[foundCellCoord.x][foundCellCoord.y + 1] == cellValue - 1) {
			foundCellCoord.y += 1;
			path[i] = Direction::Up;
		} else if(widthMap[foundCellCoord.x + 1][foundCellCoord.y] == cellValue - 1) {
			foundCellCoord.x += 1;
			path[i] = Direction::Left;
		} else if(widthMap[foundCellCoord.x][foundCellCoord.y - 1] == cellValue - 1) {
			foundCellCoord.y -= 1;
			path[i] = Direction::Down;
		} else if(widthMap[foundCellCoord.x - 1][foundCellCoord.y] == cellValue - 1) {
			foundCellCoord.x -= 1;
			path[i] = Direction::Right;
		}
	}
	return Path{path};
}

bool Map::locationMapsAcceptable(const Map &first, const Map &second, Vector location) {
	Vector currentOffset{location.x - first.offset.x + second.offset.x, location.y - first.offset.y + second.offset.y};
	Vector comparisonZoneStart{std::max(0, currentOffset.x), std::max(0, currentOffset.y)};
	Vector comparisonZoneEnd{std::min(first.size.x, second.size.x + currentOffset.x), std::min(first.size.y, second.size.y + currentOffset.y)};
	
	bool locationCorrect{true};
	for(int x = comparisonZoneStart.x; x < comparisonZoneEnd.x; ++x) {
		for(int y = comparisonZoneStart.y; y < comparisonZoneEnd.y; ++y) {
			if(!first.map[x][y].sameOrUnknown(second.map[x - currentOffset.x][y - currentOffset.y])) {
				locationCorrect = false;
				break;
			}
		}
		if(!locationCorrect) {
			break;
		}
	}
	
	return locationCorrect;
}

void Map::mergeMaps(const Map &first, const Map &second, Vector location) {
	clear();
	Vector currentOffset{location.x - first.offset.x + second.offset.x, location.y - first.offset.y + second.offset.y};
	Vector mapStart{std::min(0, currentOffset.x), std::min(0, currentOffset.y)};
	Vector mapEnd{std::max(first.size.x, second.size.x + currentOffset.x), std::max(first.size.y, second.size.y + currentOffset.y)};
	this->offset = Vector{mapStart.x + first.offset.x, mapStart.y + first.offset.y};
	this->size = Vector{mapEnd.x - mapStart.x, mapEnd.y - mapStart.y};
	this->map.resize(this->size.x, std::vector<Cell>(this->size.y, Cell(State::Unknown)));
	for(int x = mapStart.x; x < mapEnd.x; ++x) {
		for(int y = mapStart.y; y < mapEnd.y; ++y) {
			Vector firstCell{x + first.offset.x, y + first.offset.y};
			Vector secondCell{x + second.offset.x - currentOffset.x, y + second.offset.y - currentOffset.y};
			Cell cell = first.getCell(firstCell).merge(second.getCell(secondCell));
			setCell(x + first.offset.x, y + first.offset.y, cell);
		}
	}
}

Vector Map::mergeMaps(const Map &first, const Map &second, const std::vector<Vector>& locations) {
	bool locationFound{false};
	bool moreOnePossibleLocation{false};
	Vector detectedLocation;
	for(const auto& location: locations) {
		Vector currentOffset{location.x - first.offset.x + second.offset.x, location.y - first.offset.y + second.offset.y};
		Vector mapStart{std::min(0, currentOffset.x), std::min(0, currentOffset.y)};
		Vector mapEnd{std::max(first.size.x, second.size.x + currentOffset.x), std::max(first.size.y, second.size.y + currentOffset.y)};
		if(mapEnd.x - mapStart.x <= 12 || mapEnd.y - mapStart.y <= 12) {
			if(locationMapsAcceptable(first, second, location)) {
				if(locationFound) {
					moreOnePossibleLocation = true;
					break;
				} else {
					locationFound = true;
					detectedLocation = location;
				}
			}
		}
	}
	
	if(moreOnePossibleLocation) {
		clear();
		return Vector{0, 0};
	}
	
	mergeMaps(first, second, detectedLocation);
	return detectedLocation;
}

Vector Map::mergeMaps(const Map& first, const Map& second) {
	//If the cards can be located without intersecting, it makes no sense to look for their location relative to each other
	if(first.size.x + second.size.x <= 12 + 1 || first.size.y + second.size.y <= 12 + 1) {
		clear();
		return Vector{0, 0};
	}
	
	Vector offsetZoneStart{first.size.x - 12, first.size.y - 12};
	Vector offsetZoneEnd{12 - second.size.x, 12 - second.size.y};
	
	bool locationFound{false};
	bool moreOnePossibleLocation{false};
	Vector detectedLocation;
	for(int offsetX = offsetZoneStart.x; offsetX <= offsetZoneEnd.x; ++offsetX) {
		for(int offsetY = offsetZoneStart.y; offsetY <= offsetZoneEnd.y; ++offsetY) {
			Vector location{offsetX - second.offset.x + first.offset.x, offsetY - second.offset.y + first.offset.y};
			if(locationMapsAcceptable(first, second, location)) {
				if(locationFound) {
					moreOnePossibleLocation = true;
					break;
				} else {
					locationFound = true;
					detectedLocation = location;
				}
			}
		}
		if(moreOnePossibleLocation) {
			break;
		}
	}
	
	if(moreOnePossibleLocation) {
		clear();
		return Vector{0, 0};
	}
	
	mergeMaps(first, second, detectedLocation);
	return detectedLocation;
}
