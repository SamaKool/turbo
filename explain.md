# AI practicals: understand the logic, then the code

This guide follows the eight semester practicals in order. The Python filenames refer to the supplied GitHub repository; the matching C++ files are included in `AI_practicals_CPP.zip`. The C++ programs use the same core algorithms and sample data, with simpler output in a few places.

For an exam explanation, say **what a state is**, **what actions are allowed**, **how the next state is chosen**, and **when the program stops**. Those four points describe almost every practical here.

## 1. Simple reflex vacuum cleaner agent

**Files:** `VACCUM.py` and `01_vacuum.cpp`

### Logic

The world has two rooms, Left and Right. Each room is either Dirty or Clean. The agent senses only its **current location** and that room's **status**. It follows fixed condition-action rules:

```text
Current room dirty?  Suck.
Otherwise at Left?  Move Right.
Otherwise?          Move Left.
```

For a start at Left with both rooms dirty, the actions are **Suck → Right → Suck**. The program then stops because both rooms are clean. This is called a *simple reflex agent* because the action depends on the current percept, without consulting a memory of previous percepts.

### How the code works

`vacuum_agent(location, status)` chooses an action with `if/elif/else`. `run_vacuum_world(...)` holds the environment state, reads the current room's status, asks the agent for an action, applies it, and prints the result. After each action, it checks whether both rooms are clean. The C++ `action(...)` and `run(...)` functions divide the work in the same way.

**Important distinction:** the rule does not know whether the *other* room is clean. The simulation's goal check can see both rooms. If the goal check were removed, the agent would keep moving left and right after cleaning everything.

**Viva answer:** In PEAS, performance means cleanliness with little time or movement; environment means the two rooms; actuators are Suck/Left/Right; sensors report location and cleanliness.

## 2. A* search for the 8-puzzle

**Files:** `A-STAR.py` and `02_astar.cpp`

### Logic

A state is a 3 × 3 board stored as nine values; `0` is the blank. A legal action swaps `0` with one neighboring tile. The goal is to reach the target arrangement. A* chooses the most promising board using:

```text
f(state) = g(state) + h(state)
g = number of moves already made
h = estimated moves still needed
```

Here `h` is **Manhattan distance**: for each numbered tile, count how many rows plus columns separate it from its goal square, then add those values. The blank is excluded. On the sample start board, `h = 5`.

### How the code works

`neighbors(state)` finds the blank, tests Up/Down/Left/Right, and yields the valid resulting boards. `heuristic(state)` calculates Manhattan distance. `a_star(...)` stores `(f, g, state)` entries in a `heapq` priority queue, so a low `f` comes out first. `costs` remembers the cheapest known `g` for each board; `visited` prevents repeated expansion; `parents` remembers the previous board and move. Once the goal is removed from the queue, the program follows `parents` backward and reverses the path. The C++ version uses `priority_queue`, `map`, and `set` for the same jobs.

The supplied Python example finds **5 moves**. A* is an *informed* search because it uses `h`. Manhattan distance is admissible and consistent for ordinary 8-puzzle moves, so this implementation finds a shortest path for its sample. A handout statement that A* does not always find a shortest path needs a qualification: optimality depends on the heuristic and search conditions.

**Viva answer:** A* balances the cost already paid (`g`) with the estimate of what remains (`h`). Using only `h` would be greedy best-first search.

## 3. Tower of Hanoi using state-space search

**Files:** `hanoiSTATESPACE.py` and `03_hanoi.cpp`

### Logic

A state contains three stacks of disks, one for each peg. Initially every disk is on peg A; the goal is to move them all to peg C. A legal action moves only a peg's **top disk**, and only onto an empty peg or a larger disk. Each move costs one.

The program uses **breadth-first search (BFS)**: explore all states one move away, then two moves away, and so on. The first time it reaches the goal, it has found a shortest solution. For `n` disks, a shortest solution has `2^n - 1` moves; three disks need seven.

### How the code works

`successors(state)` tries each source peg and destination peg. It rejects an empty source, moving onto the same peg, and moving a larger disk onto a smaller one. It copies the peg stacks and returns each legal next state with the move that produced it. `state_space_search(disks)` puts the start state in a FIFO queue. The `parent` dictionary records where each newly discovered state came from. When the goal is reached, walking backward through `parent` reconstructs the peg moves. The C++ file represents a state as three vectors and uses a `queue` and `map`.

**Viva answer:** A state is a complete arrangement of disks; an operator is one legal top-disk move; the path cost is the number of moves.

## 4. Local beam search for the Travelling Salesman Problem

**Files:** `LOCAL.py` and `04_local_beam.cpp`

### Logic

A state is a complete tour through the six cities. Its cost is the sum of distances between consecutive cities **plus the return edge to the first city**. Lower cost is better. Rather than keeping one candidate, local beam search keeps `k` candidates; the repository uses `k = 2`.

The program makes neighbors by **swapping two cities** in a tour. It keeps city 0 fixed at the start, so it swaps only positions 1 through 5. It considers the neighbors of both current tours, keeps the best two unseen ones, and repeats for at most ten iterations. Because a future beam can be worse, a separate `best` variable preserves the best tour ever found.

### How the code works

`cost(path)` adds all six edges of the closed tour. `neighbors(path)` creates copies with pairs of positions swapped. The initial tours are randomly shuffled. `states` is the current beam; `visited` avoids reusing tours; `candidates` collects new neighbors. Sorting candidates by `cost` selects the next beam. The C++ version follows the same process with `set`, `vector`, and `sort`; it uses a fixed random seed so its run is repeatable.

This is a **heuristic**, so the final answer is a short tour, not a guaranteed optimum. Random initial tours can change the result. The handout also mentions **2-opt**, which reverses a segment; the repository code uses **swap**, a different neighborhood operation.

**Viva answer:** Beam width `k` is how many candidate tours survive each iteration. Keeping several candidates explores more alternatives than following a single tour.

## 5. BFS for the 8-puzzle

**Files:** `BFSPLANNING.py` and `05_bfs_puzzle.cpp`

### Logic

The state and legal moves are the same as Practical 2: a nine-value board, with `0` swapped with an adjacent tile. BFS uses no estimate of distance. It explores states in order of **number of moves from the start**. Because every slide costs one move, the first discovered goal has the fewest moves.

### How the code works

`moves(state)` generates legal neighboring boards. `bfs()` puts `(start, 0)` in a FIFO `deque`, where the second value is the level. When a new board is first generated, `parent[new_state] = state` records the route and also marks it discovered, so it will not be queued repeatedly. When the goal is removed from the queue, following `parent` back to the start gives the solution. The Python file also stores and prints all explored states by level; the C++ file focuses on the solution path.

The supplied example takes **3 moves**. BFS is *uninformed*: it knows the rules and goal test, but it does not use a heuristic. Its main weakness is memory use when the frontier becomes large.

**Viva answer:** BFS uses a FIFO queue; DFS uses a stack or recursion. BFS is shortest for equal-cost moves, while DFS is not guaranteed shortest.

## 6. DFS for the water jug problem

**Files:** `WATER.py` and `06_water_jug.cpp`

### Logic

A state `(a, b)` is the number of liters in Jug 1 and Jug 2. The six possible actions are fill either jug, empty either jug, or pour in either direction until the source empties or the destination fills. The repository runs with **3 L and 5 L jugs**, aiming to get **4 L** in either jug. The handout also illustrates a separate 4 L/3 L example.

DFS explores one path as far as possible before trying another. A `visited` set is essential: filling an already full jug or pouring back and forth could otherwise create repeated states. DFS can find a solution, but its route need not have the fewest steps.

### How the code works

`water_jug_dfs(...)` contains a recursive `dfs(jug1, jug2)` function. It marks a state visited, adds it to `path`, and checks whether either amount equals the target. It then constructs the six next states in a fixed order. Before trying each state it records the action. If that branch fails, it removes the action and ultimately removes the current state from the displayed path: this is **backtracking**. The C++ version uses a recursive function, `set` for visited states, and vectors for the route and action names.

**Viva answer:** The state space is finite: at most `(capacity1 + 1) × (capacity2 + 1)` different `(a,b)` pairs exist.

## 7. Eight queens using backtracking

**Files:** `8QUEEN.py` and `07_eight_queens.cpp`

### Logic

Put eight queens on an 8 × 8 board so none attack another. The program chooses **one queen per row**, so row conflicts are impossible by construction. For a proposed column, it only needs to check earlier queens for a shared **column** or **diagonal**.

Two queens at `(r1, c1)` and `(r2, c2)` share a diagonal exactly when `abs(r1-r2) == abs(c1-c2)`. If a placement is safe, try the next row. If later rows cannot be completed, remove that queen and try another column. This is backtracking in a constraint satisfaction problem.

### How the code works

`is_safe(board, row, col)` checks earlier rows. `solve(board, row)` tries columns 0 through 7, places a safe queen, and calls itself for the next row. If that call fails, it changes the square back to `.`. The Python version saves snapshots of placement and backtracking for display; the C++ version prints those actions and the final board. When `row == 8`, all queens have been placed successfully.

**Viva answer:** Variables are rows, each variable's domain is the eight columns, and constraints prohibit matching columns and diagonals.

## 8. Rule-based chatbot

**Files:** `CHATBOT.py` and `08_chatbot.cpp`

### Logic

This chatbot does not learn. It has predefined **intents**, such as greeting, thanks, weather, and joke. Each intent has example patterns and stored responses. The program normalizes the user's text, tests patterns in order, chooses a response from the first matching intent, and uses a default response when nothing matches.

### How the code works

`preprocess(text)` lowercases the text, removes punctuation, and splits it into words. `match_intent(...)` loops through intents and patterns. A pattern matches when **all its words appear among the user's words**; they do not need to be adjacent or in the same order. `get_response(...)` randomly chooses one stored response. `chatbot()` repeats this until the user enters `bye` or `exit`. The C++ file uses `tokens(...)` for normalization and a vector of `Intent` records for rules and responses.

**Important limitation:** word matching is shallow. The bot can match the right keywords while missing the user's real meaning, and it cannot answer questions outside its rules.

**Viva answer:** Lowercasing makes `Hello`, `HELLO`, and `hello` match the same greeting rule. Token matching keeps `hi` from matching inside the unrelated word `high`.

## Compare the search methods

| Method | What is explored next? | Main data structure | Shortest path guaranteed? |
|---|---|---|---|
| BFS | Oldest discovered state | FIFO queue | Yes, for equal-cost actions |
| DFS | Deepest available branch | Recursion or stack | No |
| A* | Lowest `g + h` | Priority queue | Yes here, with Manhattan distance |
| Local beam | Best `k` successor tours | Candidate set/list | No |
| Backtracking | Next valid partial assignment | Recursion | Finds a valid solution; this task does not optimize path length |

## A reliable answer pattern for the practical exam

1. **State:** What information completely describes one configuration?
2. **Actions:** Which changes are legal?
3. **Selection:** What does the algorithm try next, and which data structure makes that happen?
4. **Goal:** How does it recognize success?
5. **Limitation:** Is the answer shortest, optimal, merely valid, or heuristic?

If you can answer these five questions and trace two steps of the code, you can usually explain the practical without memorizing every print statement.
