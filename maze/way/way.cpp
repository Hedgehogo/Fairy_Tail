#include "way.h"

#include <utility>

Path::Path(std::vector<Direction> way) : way(std::move(way)) {}

bool Path::isEmpty() {
	return way.empty();
}

Direction Path::getLast() {
	return way[way.size() - 1];
}

void Path::removeLast() {
	way.pop_back();
}

Path Path::getFirstHalf() {
	int size = static_cast<int>((way.size() - way.size() / 2));
	return Path{std::vector<Direction>{way.end() - size, way.end()}};
}

Path Path::getSecondHalf() {
	int size = static_cast<int>((way.size() - way.size() / 2));
	return Path{std::vector<Direction>{way.begin(), way.begin() + size}};
}

Path Path::inverse() {
	int size = static_cast<int>(way.size());
	std::vector<Direction> result{way};
	for(int i = 0; i < size; ++i) {
		switch(way[size - i - 1]) {
			case Direction::Up:
				result[i] = Direction::Down;
				break;
			case Direction::Down:
				result[i] = Direction::Up;
				break;
			case Direction::Left:
				result[i] = Direction::Right;
				break;
			case Direction::Right:
				result[i] = Direction::Left;
				break;
			case Direction::Pass:
				break;
		}
	}
	return Path{result};
}
