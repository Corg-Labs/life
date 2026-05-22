# Conway's Game of Life in C

A terminal-based, full-color implementation of John Conway's *Game of Life* — the most famous cellular automaton in computer science — written entirely in C.

The program cycles through four iconic patterns (R-Pentomino, Acorn, Gosper Glider Gun, Pulsar) on a single grid. Cells are colored by their age, and recently-dead cells leave a brief fading trail so you can see the *shape* of the chaos as it unfolds.

No graphics library. No GPU. Just rules, a grid, and stdout.

This project focuses on learning:

- cellular automata
- Conway's rules (B3/S23)
- famous Life patterns
- double-buffered grid updates
- age-based and history-based shading
- ANSI 256-color terminal rendering
- real-time animation in C

---

# Features

- Faithful Conway's Game of Life simulator
- Cycles through R-Pentomino, Acorn, Gosper Glider Gun, and Pulsar
- Age-based coloring (white -> yellow -> orange -> red as cells age)
- Fading "ghost" trail for recently-dead cells
- Pure terminal rendering
- ANSI 256-color output
- Written entirely in C
- Standard libraries only

---

# How It Works

The grid is just a `HEIGHT x WIDTH` array of bytes. A second array of the same shape holds the *next* generation while the current generation is still being read. After computing the entire next generation, the buffers are swapped.

For every frame:

- For every cell, count its 8 neighbors
- Apply Conway's rules:
  - A live cell with 2 or 3 live neighbors stays alive
  - A live cell with any other count dies
  - A dead cell with exactly 3 live neighbors becomes alive
- Track each living cell's age (generations since birth)
- Track recently-dead cells in a small ghost buffer
- Render every cell as a colored space using its age (or ghost age) for color

The program loops forever, restarting with the next pattern in the rotation when each pattern's allotted generation count expires.

---

# Tutorial / Rendering Pipeline

## 1. The Grid

Three same-sized 2D arrays:

```
unsigned char grid[H][W];        // current generation; value = age
unsigned char next_grid[H][W];   // next generation work buffer
unsigned char ghost[H][W];       // fading trail of recently-dead cells
```

Storing age (instead of a boolean) lets us color cells by how long they've been alive — a small change that makes Life much prettier.

---

## 2. Conway's Rules

A neighbor count gives all the information you need:

```
int n = count of 8 neighbors;
int alive = grid[r][c] > 0;
will_live = alive ? (n == 2 || n == 3)
                  :  n == 3;
```

That's the entire ruleset. Every other emergent behavior in Life — gliders, oscillators, guns, methuselahs — falls out of these two lines.

---

## 3. Double-Buffered Update

We never modify the grid in place while reading it. We compute the entire next generation into `next_grid`, then `memcpy` it back:

```
for every cell:
    next_grid[r][c] = will_live ? age + 1 : 0;
memcpy(grid, next_grid, sizeof grid);
```

This is essential — modifying in place would corrupt the neighbor counts of cells we haven't visited yet.

---

## 4. Ghost Trail

When a cell goes from alive -> dead, we set `ghost[r][c] = 1`. Each subsequent frame the ghost counter increments. When it exceeds 4, the cell is *forgotten* (ghost reset to 0). This gives a 4-frame fading trail behind dying cells.

---

## 5. Coloring

Live cells:

| Age   | Color  |
|-------|--------|
| 1-2   | white  |
| 3-5   | yellow |
| 6-12  | orange |
| 13-25 | red    |
| 26+   | dark red |

Dead cells with a ghost trail use a deep blue fading into black over 4 frames.

---

## 6. Pattern Library

Four classic patterns, hand-encoded as small string arrays:

- **R-Pentomino** - 5 cells, smallest known methuselah, takes 1103 generations to stabilize
- **Acorn** - 7 cells, takes *5206 generations* to stabilize and grows to 633 cells
- **Gosper Glider Gun** - the first known infinite Life pattern; emits a glider every 30 generations
- **Pulsar** - the most common period-3 oscillator

They cycle automatically — each gets a fixed generation budget before the next one is loaded.

---

## 7. ANSI 256-Color Output

Each pixel is a single space character with a background color set via:

```
printf("\x1b[48;5;%dm ", index);
```

Adjacent pixels with the same color share one escape code to minimize output size.

---

# Build

Compile using:

```
gcc life.c -o life
```

Or just:

```
make
```

No math library needed — Life is pure integer logic.

---

# Run

```
./life
```

Press `Ctrl+C` to exit.

For best results, run in a 256-color terminal at roughly 80 columns × 36 rows.

---

# Customizing

Edit the constants near the top of `life.c`:

- `WIDTH`, `HEIGHT` — grid size
- `patterns[]` — add your own patterns from any Life-encoding source (lifewiki.com has thousands)
- Per-pattern `generations` controls how long each runs
- Color thresholds in `color_for()` change the age palette

---

# Concepts Practiced

- Cellular automata
- Conway's Game of Life rules
- Double-buffered simulation
- Neighborhood counting
- Pattern encoding
- Age tracking
- Ghost/trail rendering
- ANSI 256-color rendering
- Real-time rendering loops
- Terminal graphics

---

# Dependencies

Standard C libraries only:

- `stdio.h`
- `stdlib.h`
- `string.h`
- `unistd.h`

No graphics engine required.

---

# Inspiration

John Conway invented the Game of Life in 1970 as a simple example of how complex behavior can emerge from a tiny set of local rules. More than half a century later, people are still discovering new Life patterns — and the game itself has been proven Turing-complete.

This program is a tiny celebration of Life: a faithful simulator, the most famous patterns, and just enough color to make a video.
