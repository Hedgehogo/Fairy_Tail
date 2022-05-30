#include "cell.h"

Cell::Cell(State state) : state(state) {}

bool Cell::isEmpty() {
	return state == State::Empty;
}

bool Cell::sameOrUnknown(const Cell &cell) const {
	if(state == State::Unknown || cell.state == State::Unknown) {
		return true;
	}
	if(state == cell.state) {
		return true;
	}
	return false;
}

Cell Cell::merge(const Cell &cell) const {
	//If the cells are the same, return the original cell
	if(state == cell.state) {
		return *this;
	}
	//If one of the cells is unknown, return the second one
	if(state == State::Unknown) {
		return cell;
	}
	if(cell.state == State::Unknown) {
		return *this;
	}
	//If the cells cannot be equal, we return unknown
	return Cell(State::Unknown);
}
