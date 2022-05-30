#include "maze/fairyMaze/fairyMaze.h"

int main()
{
	FairyMaze maze;
	
	//Cycle of 1000 moves
	for(int i = 0; i < 1000; ++i) {
		//Making a turn
		//If the turn is unsuccessful, then exit the cycle
		if (!maze.makeTurn()) {
			break;
		}
	}
	
	if(maze.checkMeet()) {
		std::cout << "Characters met in " << maze.getTurnCountBeforeMeeting() << " turns." << std::endl;
	} else {
		std::cout << "Characters are blocked." << std::endl;
	}
	
	std::cout << "The study of the map ended in " << maze.getTurnCount() << " turns." << std::endl;
	
	if(maze.ableCombineMaps()) {
		std::cout << "The algorithm was able to create a general map:" << std::endl;
	} else {
		std::cout << "The algorithm was able to study two fragments of the map, but it is unknown how they are located relative to each other:" << std::endl;
	}
	maze.writeMap();
	
	std::cin.get();
	
    return 0;
}
