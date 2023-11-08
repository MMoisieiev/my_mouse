#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readline.h"


#include <stdlib.h>
#include <stdio.h>
typedef struct Cell
{
    int x;             // x-coordinate of the cell
    int y;             // y-coordinate of the cell
    int distance;      // distance to current point from start
    struct Cell *next; // pointer to the parent cell
} Cell;

Cell *addCell(Cell *first, int x, int y, int d)
{

    Cell *curr = (Cell *)malloc(sizeof(Cell));
    curr->x = x;
    curr->y = y;
    curr->distance = d;
    curr->next = NULL;
    Cell *temp = first;
    if (first != NULL)
    {
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = curr;
        return first;
    }
    else
    {
        return curr;
    }
}
void printQueue(Cell *queue)
{

    Cell *temp = queue;

    while (temp != NULL)
    {

        printf("x = %d , y = %d , d = %d\n", temp->x, temp->y, temp->distance);
        temp = temp->next;
    }
}

Cell *removeFirstCell(Cell *queue)
{

    Cell *removed = queue;
    if (queue->next == NULL)
    {
        queue = NULL;
    }
    else
    {
        queue = queue->next;
    }
    free(removed);

    return queue;
}

void printMap(char **array, int size)
{

    for (int i = 0; i < size; i++)
    {

        printf("%s", array[i]);
    }
    printf("\n");
}

void printGrid(int **array, int size)
{

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}

void CreateGrid(int **grid, char **map, int dim)
{
    for (int x = 0; x < dim; x++)
    {
        grid[x] = malloc(sizeof(int) * dim);
        for (int y = 0; y < dim; y++)
        {
            if (map[x][y] == '*')
            {
                grid[x][y] = 0;
            }
            else if (map[x][y] == '1')
            {
                grid[x][y] = 1;
            }
            else
            {
                grid[x][y] = 1;
            }
        }
    }
}

struct queueNode
{
    int x;
    int y; // The coordinates of a cell
    int dist;
    // cell's distance of from the source
};

int isValid(int row, int col, int dim, int **map)
{

    if (row >= 0 && row < dim && col >= 0 && col < dim)
    {

        if (map[row][col] == 0)
        {
            return 0;
        }
        return 1;
    }
    return 0;
}

int isVisited(int **list, int x, int y)
{

    if (list[x][y] == 1)
    {
        return 1; // if visited return 1
    }

    return 0;
}

int BFS(int **map, int **visited, int *start, int *end, int dim){
    int distance = 0;
    int rowStep[] = {-1, 0, 0, 1};
    int colStep[] = {0, -1, 1, 0};
    visited[start[0]][start[1]]++;
    Cell *queue = NULL;
    int row = 0, col = 0, frow = 0, fcol = 0;
    queue = addCell(queue, start[0], start[1], distance + 1);
    while (queue != NULL){
        // Cell* temp = queue;
        frow = queue->x;
        fcol = queue->y;
        distance = queue->distance;
        queue = removeFirstCell(queue);
        distance++;
        for (int i = 0; i < 4; i++){
            row = frow + rowStep[i];
            col = fcol + colStep[i];
            if (isValid(row, col, dim, map) == 1 && visited[row][col] < 2){
                visited[row][col]++;
                map[row][col] = distance;
                queue = addCell(queue, row, col, distance);
                if (row == end[0] && col == end[1]){

                    return distance - 1;
                }
            }
        }
    }

    return -1;
}
void getPath(int **dMap, char **sMap, int dim, int *end, int *start,char * mapInfo){
    int x = end[0] - 1, y = end[1], tempx = 0, tempy = 0;
    int rowStep[] = {-1, 0, 0};
    int colStep[] = {0, -1, 1};
    while (1){
        for (int i = 0; i < 3; i++){
            tempx = x + rowStep[i];
            tempy = y + colStep[i];
            if (isValid(tempx, tempy, dim, dMap)){
                if (dMap[tempx][tempy] == dMap[x][y] - 1){
                    sMap[x][y] = mapInfo[2];
                    x = tempx;
                    y = tempy;
                    break;
                }
            }
        }
        if (x == start[0] && y == start[1]){
            break;
        }
    }
}

int getDim(char *line)
{
    int dim = 0;
    char temp[10];
    int i = 0;
    while (line[i] != 'x')
    {
        temp[i] = line[i];
        i++;
    }
    temp[i] = '\0';
    
    dim = atoi(temp);  
    return dim;
}
char* getMapInfo(char* line){

    char* mapInfo = malloc(sizeof(char)* 4);
    int count = 0;
    for(int i = strlen(line)-2; line[i] != ' '; i--){
        mapInfo[count] = line[i];    //path character is on 0 , exit is on 1 , entrance is on 2
         count++;
    }
    mapInfo[count] = '\0';
    return mapInfo;
}

void getStrtandEnd(int *start, int *end, char **map, int dim, char* mapInfo)
{
    int row = 0;
    int col = 0;
    
    while (map[row][col] != mapInfo[1])
    {
        col++;
    }
    start[0] = row;
    start[1] = col;
    col = 0;
    row = dim - 1;
    while (map[row][col] != mapInfo[0])
    {
        col++;
    }
    end[0] = row;
    end[1] = col;
}
void freeCharMaps(char** map){
    int i = 0;
    while(map[i]){
        free(map[i]);
        map[i] = NULL;
        i++;
    }
    free(map);
    map = NULL;
}
void freeIntMaps(int** map, int dim){
   for( int i = 0; i < dim ; i++){
    free(map[i]);
   }
   free(map);
}
int isValidDimensions(int dim, char **map) {
    for (int i = 0; i < dim - 1; i++) {
        if (((int)strlen(map[i])-1) != dim) {
            return 0; // Invalid dimensions
        }
    }
    return 1; // Valid dimensions
}

// Add this function to check for the presence of entrance and exit
int hasEntranceAndExit(char **map, int dim, char mapInfo[3]) {
    int entranceFound = 0;
    int exitFound = 0;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (map[i][j] == mapInfo[0]) {
                entranceFound = 1;
            }
            if (map[i][j] == mapInfo[1]) {
                exitFound = 1;
            }
        }
    }
    return entranceFound && exitFound;
}
int proofCheckMap(int dim, char** my_map, char* mapInfo){
        if (!isValidDimensions(dim, my_map))
    {
        printf("Invalid dimensions in the map.\n");
        freeCharMaps(my_map);
        return -1;
    }

    if (!hasEntranceAndExit(my_map, dim, mapInfo))
    {
        printf("Map is missing entrance and/or exit characters.\n");
        freeCharMaps(my_map);
        return -1;
    }
    return 1;
}
int main(int argc, char **argv)
{
    
    if (argc < 1)
    {
        printf("please provide map");
        return -1;
    }
    char *map = argv[1];
    int dim = 0;
    int fd;
    char *line;
    int i = 0;
    int start[2];
    int end[2];

    
    if ((fd = open(map, O_RDONLY)) != -1)
    {
        line = my_readline(fd);
    }
    else{
        printf("bad map file");
        return -1;
    }
    
    dim = getDim(line);
    char* mapInfo;
    mapInfo = getMapInfo(line);

    char **my_map = malloc((dim * sizeof(char *) * dim) + sizeof(char)); // allocating space for 2d string array to save initial map
    int **my_grid = malloc((dim * sizeof(int) * dim));                   // allocating space for 2d int array to save initial map
    int **visited = malloc((dim * sizeof(int) * dim));                   //visited
    if (line)
    {
        free(line);
        line = NULL;
    }
    // Cell* cells = (Cell*)malloc(sizeof(Cell) * dim * dim);
    
    i = 0;
    while ((line = my_readline(fd)) && i < dim)
    {                      // go through each line of the map till the end                                                                   //allocate space for each line
        my_map[i] = line; // copy each line of the map into the 2d array
        i++;
    }
    my_map[i] = NULL;
    if (line)
    {
        free(line);
        line = NULL;
    }

    close(fd);

    if(proofCheckMap(dim, my_map, mapInfo) != 1){
        return -1;
    }
    int steps = 0;
    getStrtandEnd(start, end, my_map, dim, mapInfo);                //get start and end points 
    CreateGrid(my_grid, my_map, dim);                       //create 2 grids one main and one for visited
    CreateGrid(visited, my_map, dim);
    
    //printMap(my_map, dim);
    steps = BFS(my_grid, visited, start, end, dim) ;            //using bfs go through the whole map till end is found leaving distance at each block  
    getPath(my_grid, my_map, dim, end, start, mapInfo);                  //draw a path following next lowest number 
    printMap(my_map, dim);
    printf("%d STEPS! \n",steps);
    freeCharMaps(my_map);
    freeIntMaps(visited, dim);
    freeIntMaps(my_grid, dim);
    free(mapInfo);
    return 0;
}
