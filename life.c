/*
 * life.c - Conway's Game of Life in the terminal
 *
 * Cycles through famous Life patterns (R-pentomino, Acorn,
 * Gosper Glider Gun, Pulsar) on a fixed-size grid. Cells are
 * colored by their age, and recently-dead cells leave a brief
 * fading trail. ANSI 256-color output.
 *
 * Build:  gcc life.c -o life
 * Run:    ./life
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WIDTH   80
#define HEIGHT  36

static unsigned char grid[HEIGHT][WIDTH];
static unsigned char next_grid[HEIGHT][WIDTH];
static unsigned char ghost[HEIGHT][WIDTH];

/* ---------- Patterns ---------- */
static const char *r_pent[] = {
    " ##",
    "## ",
    " # ",
    NULL
};

static const char *acorn[] = {
    " #     ",
    "   #   ",
    "##  ###",
    NULL
};

static const char *pulsar[] = {
    "  ###   ###  ",
    "             ",
    "#    # #    #",
    "#    # #    #",
    "#    # #    #",
    "  ###   ###  ",
    "             ",
    "  ###   ###  ",
    "#    # #    #",
    "#    # #    #",
    "#    # #    #",
    "             ",
    "  ###   ###  ",
    NULL
};

static const char *gun[] = {
    "                        #           ",
    "                      # #           ",
    "            ##      ##            ##",
    "           #   #    ##            ##",
    "##        #     #   ##              ",
    "##        #   # ##    # #           ",
    "          #     #       #           ",
    "           #   #                    ",
    "            ##                      ",
    NULL
};

typedef struct {
    const char **rows;
    int h, w;
    int generations;
    const char *name;
    int row_offset, col_offset;  /* relative to centered placement */
} Pattern;

static Pattern patterns[] = {
    { r_pent, 3,  3,  400, "R-Pentomino",        0,  0 },
    { acorn,  3,  7,  600, "Acorn",              0,  0 },
    { gun,    9, 36,  280, "Gosper Glider Gun", -3,-12 },
    { pulsar,13, 13,  120, "Pulsar",             0,  0 },
};
#define NPATTERNS ((int)(sizeof patterns / sizeof patterns[0]))

static void clear_world(void) {
    memset(grid,  0, sizeof grid);
    memset(ghost, 0, sizeof ghost);
}

static void place(const Pattern *p) {
    int r0 = HEIGHT / 2 - p->h / 2 + p->row_offset;
    int c0 = WIDTH  / 2 - p->w / 2 + p->col_offset;
    for (int r = 0; p->rows[r]; r++) {
        const char *line = p->rows[r];
        for (int c = 0; line[c]; c++) {
            int gr = r0 + r, gc = c0 + c;
            if (gr < 0 || gr >= HEIGHT || gc < 0 || gc >= WIDTH) continue;
            if (line[c] == '#') grid[gr][gc] = 1;
        }
    }
}

static int neighbors(int r, int c) {
    int n = 0;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr, nc = c + dc;
            if (nr < 0 || nr >= HEIGHT || nc < 0 || nc >= WIDTH) continue;
            if (grid[nr][nc]) n++;
        }
    }
    return n;
}

static void step(void) {
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            int n = neighbors(r, c);
            int alive = grid[r][c] > 0;
            int will_live = alive ? (n == 2 || n == 3) : (n == 3);
            if (will_live) {
                next_grid[r][c] = (grid[r][c] < 250) ? grid[r][c] + 1 : 250;
            } else {
                next_grid[r][c] = 0;
            }
        }
    }
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            if (next_grid[r][c] > 0) {
                ghost[r][c] = 0;
            } else if (grid[r][c] > 0) {
                ghost[r][c] = 1;
            } else if (ghost[r][c] > 0) {
                ghost[r][c]++;
                if (ghost[r][c] > 4) ghost[r][c] = 0;
            }
        }
    }
    memcpy(grid, next_grid, sizeof grid);
}

/* Color for a cell's current state */
static int color_for(int age, int gh) {
    if (age == 0) {
        if (gh == 0) return 16;          /* black                  */
        if (gh == 1) return 17;          /* very dark blue (fresh) */
        if (gh == 2) return 18;
        if (gh == 3) return 19;
        return 17;
    }
    if (age <= 2)  return 231;           /* white                  */
    if (age <= 5)  return 226;           /* yellow                 */
    if (age <= 12) return 214;           /* orange                 */
    if (age <= 25) return 196;           /* red                    */
    return 88;                           /* dark red               */
}

int main(void) {
    printf("\x1b[?25l\x1b[2J");
    char line[WIDTH * 24 + 32];
    int idx = 0;

    for (;;) {
        clear_world();
        place(&patterns[idx]);

        for (int gen = 0; gen < patterns[idx].generations; gen++) {
            printf("\x1b[H");
            for (int r = 0; r < HEIGHT; r++) {
                int prev = -1;
                char *q = line;
                for (int c = 0; c < WIDTH; c++) {
                    int col = color_for(grid[r][c], ghost[r][c]);
                    if (col != prev) {
                        q += sprintf(q, "\x1b[48;5;%dm", col);
                        prev = col;
                    }
                    *q++ = ' ';
                }
                q += sprintf(q, "\x1b[0m\n");
                fwrite(line, 1, q - line, stdout);
            }
            fflush(stdout);
            step();
            usleep(60000);
        }

        idx = (idx + 1) % NPATTERNS;
    }
    return 0;
}
