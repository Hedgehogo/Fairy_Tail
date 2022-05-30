#pragma once
#include "../../fairyTail/fairy_tail.hpp"

class Path {
private:
	std::vector<Direction> way;
	
public:
	explicit Path(std::vector<Direction> way = {});
	
	bool isEmpty();
	
	Direction getLast();
	
	void removeLast();
	
	Path getFirstHalf();
	
	Path getSecondHalf();
	
	Path inverse();
};
