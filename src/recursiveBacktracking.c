#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "MazeTools.h"

static void recursive(Maze_t *maze, Point_t point) {
    Direction_t dir[4];
    size_t dirSz = getRandomDirections(point, *maze, dir);
    size_t index = pointToIndex(point, maze->width);

    maze->cells[index].visited = 1;

    for (size_t i = 0; i < dirSz; i++) {
        Point_t newPoint = pointShift(point, dir[i]);
        size_t newIndex = newPoint.y * maze->width + newPoint.x;
        if (!maze->cells[newIndex].visited) {
			mazeBreakWall(maze, index, newIndex, dir[i]);
            maze->cells[newIndex].visited = 1;
            recursive(maze, newPoint);
        }
    }
}

static void recursiveWithSteps(Maze_t *maze, Point_t point,
                               FILE *restrict stream) {
    Direction_t dir[4];
    size_t dirSz = getRandomDirections(point, *maze, dir);
    size_t index = pointToIndex(point, maze->width);

	fprintStepIgnoreVisted(stream, maze);

    maze->cells[index].visited = 1;

    for (size_t i = 0; i < dirSz; i++) {
        Point_t newPoint = pointShift(point, dir[i]);
        size_t newIndex = newPoint.y * maze->width + newPoint.x;
        if (!maze->cells[newIndex].visited) {
			mazeBreakWall(maze, index, newIndex, dir[i]);
            maze->cells[newIndex].visited = 1;
            recursiveWithSteps(maze, newPoint, stream);
        }
    }
}

void recursiveBacktracking(Maze_t *maze) {
    Point_t startCell, start, stop;

    srand(time(NULL));

    startCell.x = rand() % maze->width;
    startCell.y = rand() % maze->height;

    recursive(maze, startCell);

	for (size_t i = 0; i < maze->width * maze->height; i++) {
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
    maze->cells[pointToIndex(stop, maze->width)].stop = 1;

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void recursiveBacktrackingWithSteps(Maze_t *maze, FILE *restrict stream) {
    Point_t startCell, start, stop;

    srand(time(NULL));

    startCell.x = rand() % maze->width;
    startCell.y = rand() % maze->height;

    recursiveWithSteps(maze, startCell, stream);

	for (size_t i = 0; i < maze->width * maze->height; i++) {
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