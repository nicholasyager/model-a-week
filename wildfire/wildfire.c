#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ncurses.h>

void populateWorld(float percent, int rows, int cols, int world[rows][cols]);
void printWorld(int rows, int cols, int world[rows][cols], int counter);
int evaluateWorld(int rows, int cols, int world[rows][cols]);

int main()  
{
    // Initialize random seed
    srand(time(NULL));
	
	// Get terminal size.
	struct winsize ws;
	ioctl(0, TIOCGWINSZ, &ws);

	float percent = (((float) rand()/RAND_MAX) + 0.25)/(2), bestPercent, percentPad;
	int bestRun = 0;
    int cols = ws.ws_col;
    int rows = ws.ws_row-2;
    int world[rows][cols];
    int time, lifeforms, lifeformBuffer = -1, counter = 0;

    // Initialize ncurses.
    initscr();
    printw("Wildfire");
    refresh();

    // Initialize colors
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    populateWorld(percent, rows, cols, world);
	
	// Set one position on fire.
	world[rand() % rows][rand() % cols] = 2;

    for (time=0; time > -1; time++) {
		lifeforms = evaluateWorld(rows, cols, world);
		printWorld(rows, cols, world, counter);
        refresh();
        usleep(10000);
		
		if (lifeforms == lifeformBuffer)
        {
            counter += 1;

			if (counter == 1 || lifeforms == 0) {
				
				if (time > bestRun) {
					bestRun = time;
					bestPercent = percent;
				}
				percentPad = (-1+2*((float)rand())/RAND_MAX)*0.05;
				percent = bestPercent + percentPad;
				
				populateWorld(percent, rows,cols,world);
				world[rand() % rows][rand() % cols] = 2;
				counter = 0;
				lifeformBuffer = -1;
                time = 0;
				//exit(0);
                usleep(500000);
                clear();
			}
        } else {
            lifeformBuffer = lifeforms;
            counter = 0;
        }

    }

}


int evaluateWorld(int rows, int cols, int world[rows][cols]) {
    int i, j, n;
    int neighbors[8][2] = {{1,0},{0,1},{-1,0},{0,-1},{1,1},{-1,-1},{1,-1},{-1,1}};
    int newWorld[rows][cols];
    int lifeforms = 0;

	for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++){
            newWorld[i][j] = world[i][j];
			if (newWorld[i][j] == 1) {
				lifeforms++;
			}
        }
    }
	
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++){
			world[i][j] = newWorld[i][j];
			// Check if on fire
			if (newWorld[i][j] == 1) {
				// Iterate through neighbors
				for (n = 0; n < 8; n++) {
					int i2=i+neighbors[n][0];
					int j2=j+neighbors[n][1];
	
					if (i2 >= rows) i2-=rows;
					if (i2 < 0) i2+=rows;
					if (j2 >= cols) j2-=cols;
					if (j2 < 0) j2+=cols;

					if (newWorld[i2][j2] == 2) 
					{
						world[i][j] = 2;
						lifeforms--;
						continue;
					} 
				}
			}
        }
    }
    
    return(lifeforms);

}

void populateWorld(float percent, int rows, int cols, int world[rows][cols])
{
    int i, j;

    for (i=0; i < rows; i++) {
        for (j=0; j < cols; j++) {
            if ((float) rand()/RAND_MAX < percent){
                world[i][j] = 1;
            } else {
                world[i][j] = 0;
            }
        }
    }
}


void printWorld(int rows, int cols, int world[rows][cols], int counter) {
    int i, j;
    int lifeforms = 0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++){
            if (world[i][j] == 1) {
                lifeforms += 1;
                attron(COLOR_PAIR(1));
                mvprintw(i,j,"#");
            }else if (world[i][j] == 2){
                attron(COLOR_PAIR(2));
                mvprintw(i,j,"O");
            }
        }
    }
   
}
