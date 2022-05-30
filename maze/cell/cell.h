#pragma once

enum class State {
	Wall,
	Empty,
	Unknown
};

class Cell {
public:
	State state;
	
	explicit Cell(State state = State::Unknown);
	
	bool isEmpty();
	
	bool sameOrUnknown(const Cell& cell) const;
	
	Cell merge(const Cell &cell) const;
};
