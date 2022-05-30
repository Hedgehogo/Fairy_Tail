#pragma once
#include "../../fairyTail/fairy_tail.hpp"
#include "../../vector/vector.h"
#include "../cell/cell.h"
#include "../way/way.h"

class Map {
private:
	Vector offset;
	Vector size;
	std::vector<std::vector<Cell>> map;
	bool boundariesDetected;
	
	void detectBoundaries();

public:
	Map();
	
	//The transmitted position may deviate from the studied area by only one tick to either side
	Cell getCell(int x, int y) const;
	
	//The transmitted position may deviate from the studied area by only one tick to either side
	Cell getCell(Vector position) const;
	
	//The transmitted position may deviate from the studied area by only one tick to either side
	void setCell(int x, int y, Cell cell);
	
	//The transmitted position may deviate from the studied area by only one tick to either side
	void setCell(Vector position, Cell cell);
	
	bool empty();
	
	void clear();
	
private:
	static Vector excludePossibleBoundaries(std::vector<std::vector<Cell>>& map);
	
public:
	void print(Vector characterPosition, Character character);
	
	void print(Vector ivanPosition, Vector elenaPosition);
	
private:
	bool cellHasUnknownNeighbors(int x, int y) const;
	
	bool cellUnexplored(int x, int y) const;
	
public:
	Path findShortestWay(Vector whence, Vector where);
	
	Path findShortestWayToClosestUnknown(Vector whence);
	
private:
	static bool locationMapsAcceptable(const Map& first, const Map& second, Vector location);
	
	void mergeMaps(const Map& first, const Map& second, Vector location);
	
public:
	//Returns the position of the characters relative to each other.
	Vector mergeMaps(const Map& first, const Map& second, const std::vector<Vector>& locations);
	
	//Returns the position of the characters relative to each other.
	Vector mergeMaps(const Map& first, const Map& second);
};