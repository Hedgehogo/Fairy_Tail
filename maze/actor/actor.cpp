#include "actor.h"

Actor::Actor(Character character, Map &map, Fairyland &maze) : character(character), maze(maze), map(&map), currentCell(0, 0), path(), nextTurn(Direction::Pass) {
	//инициализируем все соседние ячейки
	checkNextCells();
}

void Actor::setMap(Map &map, Vector offset) {
	this->map = &map;
	currentCell.x += offset.x;
	currentCell.y += offset.y;
}

void Actor::setPath(Path path) {
	this->path = path;
}

Vector Actor::getNextCellCoord(Direction direction) {
	Vector position{currentCell};
	//меняем позицию в соответствии с направлением
	switch (direction) {
		case Direction::Up:
			--position.y;
			break;
		case Direction::Down:
			++position.y;
			break;
		case Direction::Left:
			--position.x;
			break;
		case Direction::Right:
			++position.x;
			break;
		case Direction::Pass:
			break;
	}
	return position;
}

void Actor::checkNextCell(Direction direction) {
	Vector cellPosition = getNextCellCoord(direction);
	//Write the value to the cell, if it was not known before
	if(map->getCell(cellPosition).state == State::Unknown) {
		map->setCell(cellPosition, Cell(maze.canGo(character, direction) ? State::Empty : State::Wall));
	}
}

void Actor::moveToDirection(Direction direction) {
	nextTurn = direction;
	currentCell = getNextCellCoord(direction);
}

Direction Actor::getTurnDirection() {
	return nextTurn;
}

Vector Actor::getPosition() {
	return currentCell;
}

Map &Actor::getMap() {
	return *map;
}

void Actor::printMap() {
	map->print(Vector{0, 0}, character);
}

bool Actor::makeTurn() {
	if(path.isEmpty()) {
		path = map->findShortestWayToClosestUnknown(currentCell);
		if(path.isEmpty()) {
			nextTurn = Direction::Pass;
			return false;
		}
	}
	moveToDirection(path.getLast());
	path.removeLast();
	return true;
}

void Actor::checkNextCells() {
	checkNextCell(Direction::Up);
	checkNextCell(Direction::Right);
	checkNextCell(Direction::Down);
	checkNextCell(Direction::Left);
}
