/*Assignment 2
 *David Katz
 *101157096
*/
#include <math.h>
#include <stdio.h>
#define PATH "."
#define WALL "@"
#define SMALL_MAZE_SIZE 8
#define MERGED_MAZE_SIZE 14
#define BIG_GRID_SIZE 32
//Prints to console the 32x32 maze given the array and dimension
void printEncodedMaze(int maze[], unsigned char mazeDimension)
{
    for (int i = 0; i < mazeDimension; i++)
    {
        for (int j = mazeDimension - 1; j >= 0; j--)
        {
            //Sets the current bit as the bitwise operation on the current row/column
            int curBit = (maze[i] & (1 << j)) >> j;
            //Print a wall if the bit is not 0 or a space if the bit is 0
            if (curBit == 1 || curBit == -1)
            {
                printf("%s", WALL);
            }
            if (curBit == 0)
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}
//Prints to console the 32x32 maze with path given the array and dimension
void printEncodedMazeWithPath(int maze[], int path[], unsigned char mazeDimension)
{
    for (int i = 0; i < mazeDimension; i++)
    {
        for (int j = mazeDimension - 1; j >= 0; j--)
        {
            //Sets the current maze bit as the bitwise operation on the current row/column
            int curMazeBit = (maze[i] & (1 << j)) >> j;
            //Sets the current path bit as the bitwise operation on the current row/column
            int curPathBit = (path[i] & (1 << j)) >> j;
            //Print a wall if the maze bit is not 0, a dot if the bit is 1 or a space if the bit is 0
            if (curMazeBit == 1 || curMazeBit == -1)
            {
                printf("%s", WALL);
            }
            if (curPathBit == 1)
            {
                printf(PATH);
            }
            else if (curMazeBit == 0)
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}
//Converts an array of 8 chars and stores the decimal value of the 2's complement on the int array
void encode8by8Maze(char inputMaze[SMALL_MAZE_SIZE][SMALL_MAZE_SIZE], int encodedMaze[SMALL_MAZE_SIZE])
{
    for (int i = 0; i < SMALL_MAZE_SIZE; i++)
    {
        for (int j = 0; j < SMALL_MAZE_SIZE; j++)
        {
            //Since the loop goes left to right, we need to read it right to left to get the proper decimal value
            encodedMaze[i] += (inputMaze[i][SMALL_MAZE_SIZE - j - 1] * (int)pow(2, j));
        }
        //Stores the value as a 2's complement using the formula found in the pdf notes
        encodedMaze[i] -= (int)pow(2, 8);
    }
}
//Merges 4 8x8 mazes into one 14x14 maze
void merge8by8Mazes(int topLeft[SMALL_MAZE_SIZE], int topRight[SMALL_MAZE_SIZE], int bottomLeft[SMALL_MAZE_SIZE], int bottomRight[SMALL_MAZE_SIZE], int finalMaze[MERGED_MAZE_SIZE])
{
    /**Skip the top and bottom row and then convert the complement to normal decimals
     * Left side: remove the least significant bit and shift it seven spots to make it 14 bits long
     * Right Side:clear the most significant bit and then store the complement
     */
    for (int i = 0; i < 8; i++)
    {
        // Top Left/Right skip the last row
        if (i < 7)
        {
            finalMaze[i] = ((((topLeft[i] + (int)pow(2, 8)) >> 1) << 7) + ((topRight[i] + (int)pow(2, 8)) & (~(1 << 7)))) - (int)pow(2, 14);
        }
        // Bottom Left/Right skip the first row
        if (i >= 1)
        {
            finalMaze[i + 6] = ((((bottomLeft[i] + (int)pow(2, 8)) >> 1) << 7) + ((bottomRight[i] + (int)pow(2, 8)) & (~(1 << 7)))) - (int)pow(2, 14);
        }
    }
}
//Determines whether the path fits in the 14x14 maze and returns a 1 if yes, otherwise 0.
char isPathInMaze(int maze[], int path[], unsigned char mazeDimension)
{
    for (int i = 0; i < mazeDimension; i++)
    {
        for (int j = mazeDimension - 1; j >= 0; j--)
        {
            int curMazeBit = (maze[i] & (1 << j)) >> j;
            int curPathBit = (path[i] & (1 << j)) >> j;
            if ((curMazeBit != 0) && (curPathBit == curMazeBit))
            {
                return 0;
            }
        }
    }
    return 1;
}
int main()
{
    //Arrays filled with Int values for the 32 x 32 maze, 32x 32 maze with path, and 14 x 14 maze path
    int maze[] = {-1, -2146430423, -1109928277, -1525661045, -1602496045, -1358950569, -1451509759, -1927115297,
                  -1578180479, -1354977603, -1476294999, -2084818261, -1163749757, -1964319689, -1341614619, -1750141691,
                  -1256060947, -1515522751, -1204136611, -1955867371, -1190652827, -1553272339, -1100839163, -1999963019,
                  -631368865, -1882138047, -1172944163, -1412279803, -1567107339, -164346691, -2138762879, -1};
    int path[] = {0, 0, 0, 0, 12, 8, 56, 32,
                  8032, 4416, 134115648, 67354944, 67109184, 67109312, 133169152,
                  1048576, 1835008, 262144, 262144, 262144, 458752, 65536, 65536,
                  65536, 65536, 983040, 67633152, 67633152, 201850880, 164102144, 259522560, 0};
    int pathBig[] = {0, 4096, 4096, 4096, 4096, 7936, 256, 256, 448, 112, 16, 28, 6, 0};
    //Prints the big 32 x 32 maze
    printf("32x32 Maze\n");
    printEncodedMaze(maze, BIG_GRID_SIZE);
    printf("\n32x32 Maze with path included\n");
    printEncodedMazeWithPath(maze, path, BIG_GRID_SIZE);
    //Arrays for the 4 small mazes

    char smallMaze1[SMALL_MAZE_SIZE][SMALL_MAZE_SIZE] = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 1, 0, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}};
    int smallMaze1Encoded[SMALL_MAZE_SIZE] = {0};
    encode8by8Maze(smallMaze1, smallMaze1Encoded);
    char smallMaze2[SMALL_MAZE_SIZE][SMALL_MAZE_SIZE] = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 1, 0, 1},
        {1, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 0, 1, 0, 1, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}};
    int smallMaze2Encoded[SMALL_MAZE_SIZE] = {0};
    encode8by8Maze(smallMaze2, smallMaze2Encoded);
    char smallMaze3[SMALL_MAZE_SIZE][SMALL_MAZE_SIZE] = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 1, 0, 0, 0, 1, 1},
        {1, 0, 1, 0, 1, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 1},
        {1, 0, 1, 0, 1, 0, 1, 1},
        {1, 0, 1, 0, 1, 0, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}};
    int smallMaze3Encoded[SMALL_MAZE_SIZE] = {0};
    encode8by8Maze(smallMaze3, smallMaze3Encoded);
    char smallMaze4[SMALL_MAZE_SIZE][SMALL_MAZE_SIZE] = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 1, 0, 1, 0, 1, 1},
        {1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 1, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 1, 1, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}};
    int smallMaze4Encoded[SMALL_MAZE_SIZE] = {0};
    encode8by8Maze(smallMaze4, smallMaze4Encoded);
    int mergeMaze[MERGED_MAZE_SIZE] = {0};
    int *encodedMazes[] = {smallMaze1Encoded, smallMaze2Encoded, smallMaze3Encoded, smallMaze4Encoded};
    //Prints the 256 combinations for the 14x14 mazes
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                for (int l = 0; l < 4; l++)
                {
                    merge8by8Mazes(encodedMazes[i], encodedMazes[j], encodedMazes[k], encodedMazes[l], mergeMaze);
                    printf("\n%d,%d,%d,%d\n", (i + 1), (j + 1), (k + 1), (l + 1));
                    if (isPathInMaze(mergeMaze, pathBig, MERGED_MAZE_SIZE) == 1)
                    {
                        printf("The maze fits properly\n");
                    }
                    printEncodedMaze(mergeMaze, MERGED_MAZE_SIZE);
                }
            }
        }
    }
    return (0);
}