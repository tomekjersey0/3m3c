# Missionaries and Cannibals Simulation

This project is a C-based simulation and solver for the classic **Missionaries and Cannibals problem**, using graph structures and Breadth-First Search (BFS) to find valid sequences of moves that safely transport all missionaries and cannibals across a river.

## Overview

The goal of the problem:

* Three missionaries and three cannibals are on one side of a river.
* They must all cross to the other side using a boat that holds at most two people.
* At no point can cannibals outnumber missionaries on either side, or the missionaries get “eaten.”

This implementation models each possible state of the river and boat as nodes in a graph. BFS is used to:

1. Explore all valid states.
2. Determine if a solution exists.
3. Compute the shortest sequence of moves to reach the goal.

---

## Features

* **Graph-Based Representation**: Each river state is a node; valid moves are edges.
* **BFS Pathfinding**: Efficiently finds the shortest path to solve the puzzle.
* **State Validation**: Automatically discards invalid states (negative counts, missionaries outnumbered).
* **Flexible Move System**: Supports all possible moves (one/two missionaries, one/two cannibals, or mixed).
* **Modular Design**: Separate headers for `graph`, `node`, `queue`, and BFS logic for easy reuse.

---

## File Structure

```
./include/
    bfs.h         - BFS and shortest path declarations
    graph.h       - Graph data structure
    node.h        - Graph node structure
    queue.h       - Circular queue implementation
    bad3m3c.h     - Specific state handling for Missionaries & Cannibals
    demo.h        - Demo BFS program

./src/
    main.c        - Runs the Missionaries & Cannibals simulation
    bfs.c         - BFS logic and shortest path algorithms
    graph.c       - Graph construction and management
    node.c        - Node creation and management
    queue.c       - Queue operations
```

---

## How to Build

Compile using `gcc`, `make`, or `bear` (or any C compiler):

*with gcc:*
```
gcc -Wall -Wextra -std=c11 src/*.c -Iinclude -o bfs
```
*then run:*
```
./bfs
```
*with make:*
```
make
```
*with bear*
```
bear -- make
```

You will see the sequence of boat moves printed step by step.

---

## Example Output

```
found: 1
step [1]: boat moved 1 missionary from left to right
step [2]: boat moved 1 cannibal from right to left
step [3]: boat moved 2 missionaries from left to right
...
and you're done!
```

This output lists the number of missionaries and cannibals moved in each step and the direction of the boat.

---

## Dependencies

* Standard C library only (`stdio.h`, `stdlib.h`, `string.h`, `assert.h`).
* No external libraries are required.

---

## Notes

* The simulation is fully deterministic and guarantees the shortest valid solution.
* Modular BFS and graph code can be reused for other pathfinding problems.
