#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <ncurses.h>
#include <unistd.h>

struct ant {
    int row;
    int col;
    int heading;
    int food;
};

void clearWorld(int rows, int cols, int world[2][rows][cols]);
void printAnt(int maxAnts, struct ant Ants[maxAnts]);
void renderAnts(int maxAnts, struct ant Ants[maxAnts]);
void makeAnts(int maxAnts, struct ant Ants[maxAnts], int rows, int cols);
void renderTrails(int rows, int cols, int world[2][rows][cols]);

int main()
{

    // Initialize random seed.
    srand(time(NULL));

    // Get the terminal size
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    // Initialize ncurses
    initscr();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_WHITE, COLOR_YELLOW);

    // Initiaize the world
    int rows = ws.ws_row - 2;
    int cols = ws.ws_col;
    int world[2][rows][cols];
    clearWorld(rows, cols, world);

    // Create an array of ants.
    int maxAnts = 10;
    struct ant Ants[maxAnts];

    // Populate the ant array
    makeAnts(maxAnts, Ants, rows, cols);

    // Start ze loop
    int time, ant;
    for (time = 0; time < 1000; time++) {

        // Random walk for the ants.
        for (ant = 0; ant < maxAnts; ant++)
        {
 
            // Food determines trail
            int trail;
            if (Ants[ant].food == 0) {
                trail = 0;
            } else {
                trail = 1;
            }
            world[trail][Ants[ant].row][Ants[ant].col] = 100;

            // Food determines behavior
            Ants[ant].row += (rand() % 3) - 1;
            Ants[ant].col += (rand() % 3) - 1;

            if (Ants[ant].row < 0) {
                Ants[ant].row += rows;
            } else if (Ants[ant].row >= rows) {
                Ants[ant].row -= rows;
            }

            if (Ants[ant].col < 0) {
                Ants[ant].col += cols;
            } else if (Ants[ant].col >= cols) {
                Ants[ant].col -= cols;
            }


        }

        renderTrails(rows, cols, world);
        renderAnts(maxAnts, Ants);
        refresh();
        usleep(10000);
    }

    endwin();
    return 0;
}


void clearWorld(int rows, int cols, int world[2][rows][cols])
{
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            world[0][i][j] = 0;
            world[1][i][j] = 0;
        }
    }
}

void renderTrails(int rows, int cols, int world[2][rows][cols])
{
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (world[0][i][j] > 0) {
                attron(COLOR_PAIR(3));
                mvprintw(i,j," ");
                world[0][i][j]--;
            } else if (world[1][i][j] > 0) {
                attron(COLOR_PAIR(2));
                mvprintw(i,j," ");
                world[1][i][j]--;
            }
            if (world[0][i][j] <= 0 && world[1][i][j] <= 0) {
                attron(COLOR_PAIR(1));
                mvprintw(i,j," ");
            }
        }
    }
}

void makeAnts(int maxAnts, struct ant Ants[maxAnts], int rows, int cols)
{
    int i;
    for (i = 0; i < maxAnts; i++) {
        Ants[i].row = rand() % rows;
        Ants[i].col = rand() & cols;
        Ants[i].heading = rand() % 8;
        Ants[i].food = 0;
    }
}

void renderAnts(int maxAnts, struct ant Ants[maxAnts])
{
    int i;
    for (i = 0; i < maxAnts; i++)
    {
        mvprintw(Ants[i].row, Ants[i].col,"#");
    }
}

void printAnt(int maxAnts, struct ant Ants[maxAnts])
{
    int i;
    for (i = 0; i < maxAnts; i++)
    {
        printf("%d, %d, %d\n",Ants[i].row, Ants[i].col, Ants[i].heading);
    }
}
