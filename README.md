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
Notice that for a maze of size MxN, it takes (2*M+1)x(2*N+1) characters
to store the maze.

## TODO
- [x] Maze Viewer (text based)
- [ ] Maze Generator
- - [x] [Kruskal](https://weblog.jamisbuck.org/2011/1/3/maze-generation-kruskal-s-algorithm)
- - [x] [Prim](https://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm)
- - [ ] Recursive Backtracker
- - [ ] Aldous-Broder
- - [ ] Growing Tree
- - [ ] Hunt-and-Kill
- - [ ] Wilson
- - [ ] Eller
- - [ ] Recursive Division
- - [ ] Sidewinder
- - [ ] Binary Tree
- - [ ] Cellular Automaton
- [x] Verbose Steps
- [x] Write Help
