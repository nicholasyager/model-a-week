#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

void populateWorld(int rows, int cols, int world[rows][cols]);
void printWorld(int rows, int cols, int world[rows][cols]);
int evaluateWorld(int rows, int cols, int world[rows][cols]);

int main()
{
    // Initialize random seed.
    srand(time(NULL));

    // Create the world and indices.
    int xsize = 80;
    int ysize = 24;
    int world[ysize][xsize];
    int i,j, time, lifeforms, lifeformBuffer = -1, counter = 0;

    populateWorld(ysize, xsize, world);

    for (time=0; time > -1; time++) {
        lifeforms = evaluateWorld(ysize, xsize, world);
        printWorld(ysize, xsize, world);
        if (lifeforms == lifeformBuffer)
        {
            counter += 1;
        } else {
            lifeformBuffer = lifeforms;
            counter = 0;
        }
        if (counter == 10) {
            populateWorld(ysize,xsize,world);
        }

        Sleep(100);
    }

}



int evaluateWorld(int rows, int cols, int world[rows][cols]) {
    int i, j, n;
    int neighbors[8][2] = {{1,0},{0,1},{-1,0},{0,-1},{1,1},{-1,-1},{1,-1},{-1,1}};
    int newWorld[rows][cols];
    int lifeforms = 0;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++){
            
            int numNeighbors = 0;
            // Iterate through neighbors
            for (n = 0; n < 8; n++) {
                int i2=i+neighbors[n][0];
                int j2=j+neighbors[n][1];

                if (i2 >= rows) i2-=rows;
                if (i2 < 0) i2+=rows;
                if (j2 >= cols) j2-=cols;
                if (j2 < 0) j2+=cols;

                numNeighbors += world[i2][j2];
                
            }
            
            // Set life based on neighbors
            //
            

            if (numNeighbors < 2) 
            {
                newWorld[i][j] = 0;
            } else if (numNeighbors == 2) {
                if(world[i][j] == 1)
                {
                    newWorld[i][j] = 1;
                } else {
                    newWorld[i][j] = 0;
                }
            } else if (numNeighbors == 3) 
            {
                newWorld[i][j] = 1;
            } else if (numNeighbors > 3) {
                newWorld[i][j] = 0;
            }


        }
    
    }
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++){

            world[i][j] = newWorld[i][j];
            if (world[i][j] > 1) {
                    printf("Overflow!! (%d, %d) = %d \n",i, j, world[i][j]);
                    exit(1);
            } else if(world[i][j] == 1) {
                lifeforms += 1;
            }
            
        }
    }
    return(lifeforms);

}

void populateWorld(int rows, int cols, int world[rows][cols]) {
    int i, j;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++){
            if ((float) rand()/RAND_MAX < 0.25) {
                world[i][j] = 1;
            }else{
                world[i][j] = 0;
            }
            if (world[i][j] > 1) {
                printf("(%d,%d) = %d\n", i, j, world[i][j]);
                exit(1);
            }

        }
    }
}

void printWorld(int rows, int cols, int world[rows][cols]) {
    printf("\e[1;1H\e[2J");
    int i, j;
    int lifeforms = 0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++){
            if (world[i][j] == 1) {
                lifeforms += 1;
                printf("#");
            }else if (world[i][j] == 0){
                printf(" ");
            } else {
                printf("!");
            }
        }
        printf("\n");
    }
    printf("==== %d Lifeforms\n",lifeforms);
   
}
