#include "MazeTools.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Maze_t createMazeWH(size_t width, size_t height) {
	Maze_t maze = {width, height, NULL};

	maze.maze = malloc(sizeof(*maze.maze) * height * width);
	if (maze.maze == NULL) {
		perror("Failed to allocate maze");
		exit(EXIT_FAILURE);
	}

	return maze;
}

Maze_t createMaze(const char* mazeStr) {
	Maze_t maze = {0, 0, NULL};
	size_t len = strlen(mazeStr);
	size_t count = 0;

	maze.maze = malloc(sizeof(*maze.maze) * len);
	if (maze.maze == NULL) {
		perror("Failed to allocate maze");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; i < len; ++i) {
		if (mazeStr[i] == '\n') {
			if (maze.width == 0) {
				maze.width = count;
			}
			maze.height++;
			continue;
		}

		maze.maze[count++] = mazeStr[i];
	}

	if (mazeStr[len - 1] != '\n') {
		if (maze.width == 0) {
			maze.width = count;
		}
		maze.height++;
	}

	maze.maze[count] = '\0';

	maze.maze = realloc(maze.maze, sizeof(*maze.maze) * count);
	if (maze.maze == NULL) {
		perror("Failed to allocate maze");
		exit(EXIT_FAILURE);
	}

	return maze;
}

void freeMaze(Maze_t maze) {
	free(maze.maze);
}
