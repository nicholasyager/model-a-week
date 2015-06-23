#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>

void populateWorld(int rows, int cols, int world[3][rows][cols]);
void clearWorld(int rows, int cols, int world[3][rows][cols]);
void printWorld(int rows, int cols, int world[3][rows][cols]);

int main()
{

    // Initialize random seed.
    srand(time(NULL));

    // Get the terminal size
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    int rows = ws.ws_row - 3;
    int cols = ws.ws_col;

    // Create matrices of ants and trails.
    int world[3][rows][cols];

    clearWorld(rows,cols, world);
    populateWorld(rows, cols, world);
    printWorld(rows, cols, world);


}


void clearWorld(int rows, int cols, int world[3][rows][cols])
{
    int i, j;
    for ( i = 0; i < rows; i++)
    {
        for ( j = 0; j < cols; j++)
        {
            world[0][i][j] = 0;
            world[1][i][j] = 0;
            world[2][i][j] = 0;
        }
    }
}

void populateWorld(int rows, int cols, int world[3][rows][cols])
{
    int i, j, numAnts, maxAnts = 10, heading;
    
    // Place 10 ants.
    for (numAnts = 0; numAnts < maxAnts; numAnts++) 
    {
        i = rand() % rows;
        j = rand() % cols;
        heading = rand() % 8;
        world[1][i][j] = heading;
        world[0][i][j] = 1;
    }
}

void printWorld(int rows, int cols, int world[3][rows][cols])
{
    int i, j;
    for ( i = 0; i < rows; i++ ) 
    {
        for ( j = 0; j < cols; j++ )
        {
            if ( world[0][i][j] > 0)
            {
                printf("%d", world[1][i][j]);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

}
