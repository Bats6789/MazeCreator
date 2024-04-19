#ifndef __MAZE_TOOLS_H__
#define __MAZE_TOOLS_H__

typedef struct {
	size_t width;
	size_t height;
	char *maze;
} Maze_t;

Maze_t createMaze(const char* maze);
Maze_t createMazeWH(size_t width, size_t height);
void freeMaze(Maze_t maze);

#endif /* ifndef __MAZE_TOOLS_H__ */
