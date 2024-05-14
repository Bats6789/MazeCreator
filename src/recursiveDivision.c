#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MazeTools.h"
#include "recursiveDivision.h"

static void recursive(Maze_t *maze, Point_t topLeft, Point_t bottomRight) {
	size_t width = bottomRight.x - topLeft.x;
	size_t height = bottomRight.y - topLeft.y;
	size_t x, y;

	// The grid should bisect the longest direction.
	// If they are the same, the choice is random.
	if ((width == height && rand() % 2 == 0) || height > width) { // bissect horizontally
		x = 0;
	} else { // bissect virtically
		
	}
}

void recursiveDivisionGen(Maze_t *maze) {
	Point_t topLeft = {0, 0};
	Point_t bottomRight = {maze->width - 1, maze->height - 1};

    srand(time(NULL));

	// remove all walls
	for (size_t i = 0; i < maze->width * maze->height; i++) {
		maze->cells[i].left = 0;
		maze->cells[i].right = 0;
		maze->cells[i].top = 0;
		maze->cells[i].bottom = 0;
	}

	recursive(maze, topLeft, bottomRight);

    // assign start and stop location
    assignRandomStartAndStop(maze);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void recursiveDivisionGenWithSteps(Maze_t *maze, FILE *restrict stream) {
    srand(time(NULL));
    // assign start and stop location
    assignRandomStartAndStopWithSteps(maze, stream);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}
