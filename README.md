# Project 3-Puzzle A* solver

A simple A* solver for 3-Puzzle game.

# Dependencies

To compile the project you will need: GCC and Make

```
sudo apt-get install gcc
sudo apt-get install make
```

# Compilation

```
make
```

# Execution

```
make execute
```

# Usage

### Menu

When you start the program, you will have to choose a distance. Simply enter 1 or 2.

```
3-Puzzle solver
Choose a distance: 
1. Manhattan
2. Hamming
Enter 1 or 2:
```

### Solution

You will then get a solution for a random 3-Puzzle.  
Starting from a random game to the solution found with A*.

```
Solution found ! 
Explored possibilities: ...
Number of swap: ...
Path: 

-------------
| 7 | 1 | 5 |
-------------
| 2 | 8 | 3 |
-------------
| 4 | 6 |   |
-------------

...

-------------
| 1 | 2 | 3 |
-------------
| 4 | 5 | 6 |
-------------
| 7 | 8 |   |
-------------
```

# Author

Evan Clausse ING2 GSI Groupe 1
