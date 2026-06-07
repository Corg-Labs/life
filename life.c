#include "raylib.h"
#include <stdlib.h>

#define GRID_W 120
#define GRID_H 80
#define CELL 8

static unsigned char grid[GRID_H][GRID_W];
static unsigned char nextg[GRID_H][GRID_W];
static unsigned char age[GRID_H][GRID_W];
static unsigned char trail[GRID_H][GRID_W];

static void SeedWorld(void)
{
    for(int y=0;y<GRID_H;y++)
    for(int x=0;x<GRID_W;x++)
    {
        grid[y][x] = (GetRandomValue(0,100) < 18);
        age[y][x] = grid[y][x];
        trail[y][x] = 0;
    }
}

static int Neighbors(int y,int x)
{
    int n=0;
    for(int dy=-1;dy<=1;dy++)
    for(int dx=-1;dx<=1;dx++)
    {
        if(dx==0 && dy==0) continue;

        int ny=(y+dy+GRID_H)%GRID_H;
        int nx=(x+dx+GRID_W)%GRID_W;

        n += grid[ny][nx] ? 1 : 0;
    }
    return n;
}

static void Step(void)
{
    for(int y=0;y<GRID_H;y++)
    for(int x=0;x<GRID_W;x++)
    {
        int alive = grid[y][x];
        int n = Neighbors(y,x);

        nextg[y][x] = alive ? (n==2 || n==3) : (n==3);
    }

    for(int y=0;y<GRID_H;y++)
    for(int x=0;x<GRID_W;x++)
    {
        if(nextg[y][x])
        {
            age[y][x] = age[y][x] < 250 ? age[y][x]+1 : 250;
            trail[y][x] = 0;
        }
        else
        {
            if(grid[y][x])
                trail[y][x] = 255;
            else if(trail[y][x] > 4)
                trail[y][x] -= 4;

            age[y][x] = 0;
        }

        grid[y][x] = nextg[y][x];
    }
}

static Color CellColor(int a,int t)
{
    if(a > 0)
    {
        if(a < 3)  return WHITE;
        if(a < 8)  return YELLOW;
        if(a < 20) return ORANGE;
        if(a < 50) return RED;
        return MAROON;
    }

    if(t > 0)
        return (Color){40,120,255,(unsigned char)t};

    return BLACK;
}

int main(void)
{
    const int screenW = GRID_W * CELL;
    const int screenH = GRID_H * CELL;

    InitWindow(screenW, screenH, "Game of Life");
    SetTargetFPS(60);

    SeedWorld();

    bool paused = false;
    float accumulator = 0.0f;
    const float simStep = 0.08f;

    while(!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_R))
            SeedWorld();

        if(IsKeyPressed(KEY_SPACE))
            paused = !paused;

        accumulator += GetFrameTime();

        while(accumulator >= simStep && !paused)
        {
            Step();
            accumulator -= simStep;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        for(int y=0;y<GRID_H;y++)
        for(int x=0;x<GRID_W;x++)
        {
            Color c = CellColor(age[y][x], trail[y][x]);

            if(c.a || age[y][x])
                DrawRectangle(
                    x * CELL,
                    y * CELL,
                    CELL,
                    CELL,
                    c
                );
        }

        DrawText("SPACE = Pause   R = New World",10,10,20,WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
