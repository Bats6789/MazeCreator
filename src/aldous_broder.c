#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MazeTools.h"
#include "aldous_broder.h"

void aldousBroder(Maze_t *maze) {
	Point_t point, start, stop;
	size_t sz = maze->width * maze->height;
	size_t cellsNotVisitedCount = sz;
	size_t index;

	srand(time(NULL));

	point.x = rand() % maze->width;
	point.y = rand() % maze->height;

	index = pointToIndex(point, maze->width);

	maze->cells[index].visited = 1;
	cellsNotVisitedCount--;

	while (cellsNotVisitedCount > 0) {
		Direction_t dir = getRandomDirection(point, *maze);
		Point_t newPoint = pointShift(point, dir);
		size_t newIndex = pointToIndex(newPoint, maze->width);

		if (!maze->cells[newIndex].visited) {
			mazeBreakWall(maze, index, newIndex, dir);
			maze->cells[newIndex].visited = 1;
			cellsNotVisitedCount--;
		}

		index = newIndex;
		point = newPoint;
	}

	for (size_t i = 0; i < sz; i++) {
		maze->cells[i].visited = 0;
	}

    // assign start and stop location
    if (rand() % 2 == 0) {
        start.x = rand() % maze->width;
        stop.x = rand() % maze->width;
        if (rand() % 2 == 0) {
            start.y = 0;
            stop.y = maze->height - 1;
        } else {
            start.y = maze->height - 1;
            stop.y = 0;
        }
    } else {
        start.y = rand() % maze->height;
        stop.y = rand() % maze->height;
        if (rand() % 2 == 0) {
            start.x = 0;
            stop.x = maze->width - 1;
        } else {
            start.x = maze->width - 1;
            stop.x = 0;
        }
    }

    maze->cells[pointToIndex(start, maze->width)].start = 1;
    maze->cells[pointToIndex(stop, maze->height)].stop = 1;

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void aldousBroderWithSteps(Maze_t *maze, FILE *restrict stream) {
	Point_t point, start, stop;
	size_t sz = maze->width * maze->height;
	size_t cellsNotVisitedCount = sz;
	size_t index;

	srand(time(NULL));

	point.x = rand() % maze->width;
	point.y = rand() % maze->height;

	index = pointToIndex(point, maze->width);

	maze->cells[index].visited = 1;
	maze->cells[index].observing = 1;
	fprintStepIgnoreVisted(stream, maze);
	cellsNotVisitedCount--;

	while (cellsNotVisitedCount > 0) {
		Direction_t dir = getRandomDirection(point, *maze);
		Point_t newPoint = pointShift(point, dir);
		size_t newIndex = pointToIndex(newPoint, maze->width);

		if (!maze->cells[newIndex].visited) {
			mazeBreakWall(maze, index, newIndex, dir);
			maze->cells[newIndex].visited = 1;
			cellsNotVisitedCount--;
		}
		maze->cells[index].observing = 0;
		maze->cells[newIndex].observing = 1;
		fprintStepIgnoreVisted(stream, maze);

		index = newIndex;
		point = newPoint;
	}
	maze->cells[index].observing = 0;

	for (size_t i = 0; i < sz; i++) {
		maze->cells[i].visited = 0;
	}

    // assign start and stop location
    if (rand() % 2 == 0) {
        start.x = rand() % maze->width;
        stop.x = rand() % maze->width;
        if (rand() % 2 == 0) {
            start.y = 0;
            stop.y = maze->height - 1;
        } else {
            start.y = maze->height - 1;
            stop.y = 0;
        }
    } else {
        start.y = rand() % maze->height;
        stop.y = rand() % maze->height;
        if (rand() % 2 == 0) {
            start.x = 0;
            stop.x = maze->width - 1;
        } else {
            start.x = maze->width - 1;
            stop.x = 0;
        }
    }

    maze->cells[pointToIndex(start, maze->width)].start = 1;
    fprintStep(stream, maze);
    maze->cells[pointToIndex(stop, maze->width)].stop = 1;

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}
