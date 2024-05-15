# MazeCreator
Generates mazes

## Build

### CMAKE
To build, simply run:
```
mkdir build
mkdir lib
mkdir bin
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
The compiled program will be found in **bin**.

## Generate a maze and solve
Simply running `MazeSolver` will generate a 10x10 maze.

Running `MazeSolver <W> <H>` will generate a maze of size WxH.

### Generating example
This is an example of a generated maze:
```
#####################
#        X  # #   # #
### ##### # # # ### #
# # #     #       # #
# # ### # ### ##### #
# # # # # #     # # #
# ### # ##### # # # #
#     #   # # # #   #
# ### # # # # # # # #
# #   # # #   #   # #
# # # # # ######### #
# # #   #     #     #
# ##### ### ### #####
# # # # # # #       #
### # ### ### ### ###
#   #           # # #
# ######### # ### # #
#       # # # #     #
# ### # # ### ##### #
# #   #       #    S#
#####################
```

### Symbol Definition
The following symbols are defined for a maze:
* \#    - Wall
* space - Path
* S     - Starting location
* X     - Destination
* :     - Observing

### Format
A valid maze will have:
1. A border of walls.
2. A start labeled `S`.
3. An end labeled `X`.

The maze consist of cells. Each cell is defined as a 9x9 grid of text.

This is an example of a cell:
```
###
# #
###
```
Only the 4 cardinal directions (up, down, left, right) are considered
for walls. The corners are included mainly for alignment and ascetics.

This is an example of a cell with no walls:
```
# #
   
# #
```

Two neighboring cells will share the same wall.

This is an example of two neighboring cells (1 and 2):
```
#####
#1 2#
#####
```

This is an example of four cells in a 2x2 formation:
```
#####
#1 2#
### #
#3 4#
#####
```
Notice that for a maze of size MxN, it takes $(2M+1)\times(2N+1)$ characters
to store the maze.

#### Notes:
Oberving cells (:) only come into play for Prim's algorithm, and only during verbose
step generation. The observing cell allows the user to see which cells the algorithm
is currently considering for processing. It may have application for other algoritms
in the future.

## TODO
- [x] Maze Viewer (text based)
- [ ] Maze Generator
- - [x] [Kruskal](https://weblog.jamisbuck.org/2011/1/3/maze-generation-kruskal-s-algorithm)
- - [x] [Prim](https://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm)
- - [x] [Recursive Backtracker](https://weblog.jamisbuck.org/2010/12/27/maze-generation-recursive-backtracking)
- - [x] [Aldous-Broder](https://weblog.jamisbuck.org/2011/1/17/maze-generation-aldous-broder-algorithm)
- - [x] [Growing Tree](https://weblog.jamisbuck.org/2011/1/27/maze-generation-growing-tree-algorithm#)
- - - [x] Add oldest cell
- - - [x] Add newest cell
- - - [x] Add middle cell
- - - [x] Add random cell
- - - [x] Allow mixing of methods
- - [x] [Hunt-and-Kill](https://weblog.jamisbuck.org/2011/1/24/maze-generation-hunt-and-kill-algorithm)
- - [x] [Wilson](https://weblog.jamisbuck.org/2011/1/20/maze-generation-wilson-s-algorithm)
- - [x] [Eller](https://weblog.jamisbuck.org/2010/12/29/maze-generation-eller-s-algorithm#)
- - [x] [Recursive Division](https://weblog.jamisbuck.org/2011/1/12/maze-generation-recursive-division-algorithm.html)
- - [x] [Sidewinder](https://weblog.jamisbuck.org/2011/2/3/maze-generation-sidewinder-algorithm)
- - [x] [Binary Tree](https://weblog.jamisbuck.org/2011/2/1/maze-generation-binary-tree-algorithm#)
- - [ ] Cellular Automaton
- [x] Verbose Steps
- [x] Write Help
