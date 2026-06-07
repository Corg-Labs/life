# Conway's Game of Life in C

A real-time implementation of Conway's Game of Life written in C using Raylib.

This program simulates a cellular automaton on a toroidal grid where every cell evolves according to a simple set of local rules. Despite those simple rules, complex patterns, oscillators, spaceships, and self-organizing structures emerge over time.

To make the simulation more visually appealing, cells are colored according to their age and recently deceased cells leave fading trails behind them.

The project focuses on learning:

cellular automata
grid-based simulation
neighbor counting
double buffering
age tracking
real-time rendering
fixed timestep simulation
toroidal worlds
visualization techniques

---

## Features

Randomly generated worlds

Toroidal (wraparound) simulation space

Age-based cell coloring

Fading trails for dead cells

Fixed timestep updates

Pause and resume controls

Real-time rendering using Raylib

Pure C implementation

---

# How It Works

The world is represented as a two-dimensional grid.

Each cell can be in one of two states:

Alive

Dead

During every generation, each cell examines its eight neighbors and decides whether it survives, dies, or becomes alive.

Although every cell only has local information, large-scale structures emerge naturally from these interactions.

---

# Conway's Rules

For every cell:

Any live cell with fewer than two neighbors dies (underpopulation)

Any live cell with two or three neighbors survives

Any live cell with more than three neighbors dies (overpopulation)

Any dead cell with exactly three neighbors becomes alive (reproduction)

These four simple rules generate surprisingly complex behavior.

---

# Simulation Pipeline

### 1. World Initialization

A random world is generated:

```c
grid[y][x] = (GetRandomValue(0,100) < 18);
```

Each cell begins either alive or dead.

Different random seeds create completely different ecosystems.

---

### 2. Neighbor Counting

For every cell, the simulation counts the eight surrounding cells.

```c
for(int dy=-1;dy<=1;dy++)
for(int dx=-1;dx<=1;dx++)
```

The resulting count determines the cell's next state.

---

### 3. Toroidal Wrapping

Instead of treating edges as boundaries, the world wraps around itself.

```c
ny = (y + dy + GRID_H) % GRID_H;
nx = (x + dx + GRID_W) % GRID_W;
```

Cells leaving one side of the world reappear on the opposite side.

This prevents patterns from disappearing at the edges.

---

### 4. Double Buffering

The simulation stores:

Current generation

Next generation

The next generation is computed independently before replacing the current world.

```c
nextg[y][x] = ...
```

This ensures all cells update simultaneously.

---

### 5. Age Tracking

Every surviving cell tracks how long it has been alive.

```c
age[y][x]++;
```

Older cells receive different colors than newly born cells.

This makes stable structures easier to identify visually.

---

### 6. Dead Cell Trails

When a cell dies:

```c
trail[y][x] = 255;
```

A fading trail remains behind.

Every frame the trail intensity decreases:

```c
trail[y][x] -= 4;
```

This creates a motion-history effect and makes pattern evolution easier to observe.

---

### 7. Visualization

Cell color depends on age:

New cells → White

Young cells → Yellow

Mature cells → Orange

Old cells → Red

Ancient cells → Dark Red

Dead trails → Blue fading to black

The color gradient reveals the lifespan of structures in the simulation.

---

### 8. Fixed Timestep Updates

The simulation updates independently from rendering.

```c
const float simStep = 0.08f;
```

This keeps the automaton stable regardless of frame rate.

Rendering occurs at 60 FPS while generations advance at a fixed rate.

---

# Controls

Space

Pause / Resume simulation

R

Generate a new random world

---

# Build

Compile using:

```bash
gcc life.c -o life -lraylib -lm
```

---

# Run

```bash
./life
```

---

# Customizing

Experiment with:

Grid size

Cell size

Simulation speed

Initial population density

Color palette

Trail decay rate

Different values can dramatically change the behavior of the simulation.

---

# Concepts Practiced

Cellular automata

Conway's Game of Life

Grid-based simulations

Double buffering

Neighbor search algorithms

Fixed timestep simulation

State visualization

Age tracking

Procedural generation

Real-time graphics

Raylib rendering

Simulation systems

---

# Dependencies

Raylib

math.h

stdlib.h

No game engine required.

---

# Inspiration

Conway's Game of Life was created by mathematician John Conway in 1970.

It remains one of the most famous examples of emergent behavior in computer science.

The fascinating part is that complex structures emerge from extremely simple rules.

Every generation follows the same four rules.

Yet the simulation continuously produces patterns that appear alive, adaptive, and organized.
