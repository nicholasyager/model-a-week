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

void clearWorld(int rows, int cols, int world[3][rows][cols]);
void printAnt(int maxAnts, struct ant Ants[maxAnts]);
void renderAnts(int maxAnts, struct ant Ants[maxAnts]);
void makeAnts(int maxAnts, struct ant Ants[maxAnts], int rows, int cols, int homeRow, int homeCol);
void renderTrails(int rows, int cols, int world[3][rows][cols]);
void headingToCoords(int heading, int coords[2]);

int neighbors[8][2] = {{-1,-1},
                       {-1, 0},
                       {-1, 1},
                       { 0, 1},
                       { 1, 1},
                       { 1, 0},
                       { 1,-1},
                       { 0,-1}};

int main()
{

    // Initialize random seed.
    srand(time(NULL));

    // Write to a log
    FILE *f = fopen("ants.log","w");

    // Get the terminal size
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    // Initialize ncurses
    initscr();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_RED);
    init_pair(4, COLOR_WHITE, COLOR_YELLOW);
    init_pair(5, COLOR_WHITE, COLOR_GREEN);

    // Initiaize the world
    int rows = ws.ws_row;
    int cols = ws.ws_col;
    int world[3][rows][cols];
    clearWorld(rows, cols, world);
    int homeRow = rand() % rows;
    int homeCol = rand() % cols;

    int numFood, maxFood = 5;
    for (numFood = 0; numFood < maxFood; numFood++) {
        world[2][rand() % rows][rand() % cols] = 1;
    }

    world[2][homeRow][homeCol] = 2;
    
    // Create an array of ants.
    int maxAnts = 10;
    struct ant Ants[maxAnts];

    // Populate the ant array
    makeAnts(maxAnts, Ants, rows, cols, homeRow, homeCol);

    // Start ze loop
    int time = 0, ant;
    while (time > -1) {
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

            world[trail][Ants[ant].row][Ants[ant].col] += 200;

            int newCoords[2] = {Ants[ant].row, Ants[ant].col};

            // Make a temporary new heading and adjust accordingly.
            int newHeading = Ants[ant].heading + (rand() % 3) - 1;
            int headingIndex, newRow, newCol;
            for (headingIndex = -1; headingIndex < 2; headingIndex++) {
                int netHeading = Ants[ant].heading + headingIndex;
                int searchRow = neighbors[netHeading][0];
                int searchCol = neighbors[netHeading][1];

                if (world)
            }
            

            // Food determines behavior
            Ants[ant].row = newRow;
            Ants[ant].col = newCol;

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

void headingToCoords(int heading, int coords[2]) {
    int currentRow, currentCol;
    currentRow = coords[0];
    currentCol = coords[1];

    coords[0] += neighbors[heading][0]
    coords[1] += neighbors[heading][1]
    return
}

void clearWorld(int rows, int cols, int world[3][rows][cols])
{
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            world[0][i][j] = 0;
            world[1][i][j] = 0;
            world[2][i][j] = 0;
        }
    }
}

void renderTrails(int rows, int cols, int world[3][rows][cols])
{
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (world[0][i][j] > 0 && world[1][i][j] == 0) {
                attron(COLOR_PAIR(4));
                mvprintw(i,j," ");
                world[0][i][j]--;
            } else if (world[1][i][j] > 0 && world[0][i][j] == 0) {
                attron(COLOR_PAIR(3));
                mvprintw(i,j," ");
                world[1][i][j]--;
            } else if (world[0][i][j] > 0 && world[1][i][j] > 0) {
                attron(COLOR_PAIR(5));
                mvprintw(i,j," ");
                world[0][i][j]--;
                world[1][i][j]--;
            }

            
            if (world[2][i][j] > 0) {
                attron(COLOR_PAIR(3));
                mvprintw(i,j,"O");
            } else if (world[0][i][j] <= 1 && world[1][i][j] <= 1) {
                attron(COLOR_PAIR(1));
                mvprintw(i,j," ");
            }
        }
    }
}

void makeAnts(int maxAnts, struct ant Ants[maxAnts], int rows, int cols, int homeRow, int homeCol)
{
    int i;
    for (i = 0; i < maxAnts; i++) {
        //Ants[i].row = rand() % rows;
        //Ants[i].col = rand() & cols;
        Ants[i].row = homeRow + ( rand() % 6) - 3;
        Ants[i].col = homeCol + ( rand() % 6) - 3;
        Ants[i].heading = rand() % 8;
        Ants[i].food = 0;
    }
}

void renderAnts(int maxAnts, struct ant Ants[maxAnts])
{
    int i;
    attron(COLOR_PAIR(2));
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
