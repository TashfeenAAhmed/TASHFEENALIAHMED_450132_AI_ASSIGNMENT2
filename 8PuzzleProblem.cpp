// this code takes user input for GOAL STATE to be achieved and INITIAL STATE from where search starts to achieve the goal
// the code takes user input for which search algorithm to apply (A* or Greedy First) and which heuristic function to use (misplaced Tiles or Manhattan Distance)
// the code can be used for testing different input cases against type of searches as some cases can be solved by certain searches and some can't

#include <bits/stdc++.h>
using namespace std;

#define r 3         //no. of rows 
#define c 3         //no. of columns

struct Node
{
    Node* parent;               //pointer to structure named parent
    int mat[r][c];
    int x, y;
    int cost;
    int level;
};

int printState(int mat[r][c])
{
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
    return 0;
}

//Creates a new node for the puzzle, copying the state of the matrix and updating coordinates and other node properties.
Node* newNode(int mat[r][c], int x, int y, int newX, int newY, int level, Node* parent)
{
    Node* node = new Node;        //new allocates memory for new struct Node and stores its memory in pointernode 
    node->parent = parent;              //-> assigns members of struct node 
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            node->mat[i][j] = mat[i][j];
        }
    }
    int temp = node->mat[x][y];                         //assigns matrix 0 to variable
    node->mat[x][y] = node->mat[newX][newY];            //assigns new updated mat[x][y] value to 0 location of matrix
    node->mat[newX][newY] = temp;                       //assigns the other element value 0 basically moving the tile 0 to adjacent tile
    node->cost = INT_MAX;                               
    node->level = level;
    node->x = newX;
    node->y = newY;
    return node;
}

int MisplacedTile(int mat[r][c], int goal[r][c])
{
    int count = 0;
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            if (mat[i][j] != 0 && mat[i][j] != goal[i][j])
                count++;
    return count;
}

int ManhattanDistance(int mat[r][c], int goal[r][c]) {
    int totalManhattanDistance = 0;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (mat[i][j] != 0) { // Skip the empty tile
                for (int I = 0; I < r; I++) {
                    for (int J = 0; J < c; J++) {
                        if (mat[i][j] == goal[I][J]) {          // element value where equal, use position values to calculate distance 
                            totalManhattanDistance += abs(i - I) + abs(j - J);
                        }
                    }
                }
            }
        }
    }
    return totalManhattanDistance;
}

//Checks if a given coordinate (x, y) is within the bounds of the puzzle matrix.
int inBounds(int x, int y)
{
    return (x >= 0 && x < r && y >= 0 && y < c);
}

//prints solution from initial to goal state
void printSolution(Node* root)
{
    if (root == NULL)               
        return;
    printSolution(root->parent);        //recursively function is called and when parent ==null which is node for initial state, it starts returning to previously called function and executes next line 
    printState(root->mat);          // starts printing each state
    printf("\n");
}
 
//used for performing right, up, left, down action of 0 tile
int row[] = { 1, 0, -1, 0 };
int col[] = { 0, -1, 0, 1 };

struct compGreedyFirst                  //priority setting from lowest evaluation (f) value to highest value of nodes
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return (lhs->cost) > (rhs->cost);
    }
};

//Greedy First Search Algorithm
void GreedyFirst(int goal[r][c], int initial[r][c], int x, int y, int heuristic)
{
    priority_queue<Node*, vector<Node*>, compGreedyFirst> pq;              //container adopter with comp as setting the priority order 

    Node* root = newNode(initial, x, y, x, y, 0, NULL);
    if (heuristic == 1) {
        root->cost = ManhattanDistance(initial, goal);
    }
    else if (heuristic == 2) {
        root->cost = MisplacedTile(initial, goal);
    }
    pq.push(root);

    while (!pq.empty())
    {
        Node* min = pq.top();
        pq.pop();

        if (min->cost == 0)
        {
            printSolution(min);
            return;
        }

        for (int i = 0; i < 4; i++)
        {
            if (inBounds(min->x + row[i], min->y + col[i]))
            {
                Node* child = newNode(min->mat, min->x, min->y, min->x + row[i], min->y + col[i], min->level, min);
                if (heuristic == 1) {
                    child->cost = ManhattanDistance(child->mat, goal);
                }
                else if (heuristic == 2) {
                    child->cost = MisplacedTile(child->mat, goal);
                }
                pq.push(child);
            }
        }
    }
}

struct compAstar                //priority setting from lowest evaluation (f) value to highest value of nodes
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return (lhs->cost + lhs->level) > (rhs->cost + rhs->level);
    }
};

//A* search algorithm
void ASTAR(int goal[r][c], int initial[r][c], int x, int y, int heuristic)
{
    priority_queue<Node*, vector<Node*>, compAstar> pq;

    Node* root = newNode(initial, x, y, x, y, 0, NULL);
    if (heuristic == 1) {
        root->cost = ManhattanDistance(initial, goal);
    }
    else if (heuristic == 2) {
        root->cost = MisplacedTile(initial, goal);
    }
    pq.push(root);

    while (!pq.empty())
    {
        Node* min = pq.top();
        pq.pop();

        if (min->cost == 0)
        {
            printSolution(min);
            return;
        }

        for (int i = 0; i < 4; i++)
        {
            if (inBounds(min->x + row[i], min->y + col[i]))
            {
                Node* child = newNode(min->mat, min->x, min->y, min->x + row[i], min->y + col[i], min->level + 1, min);
                if (heuristic == 1) {
                    child->cost = ManhattanDistance(child->mat, goal);
                }
                else if (heuristic == 2) {
                    child->cost = MisplacedTile(child->mat, goal);
                }
                pq.push(child);
            }
        }
    }
    
}

int main()
{
    int goalState[r][c];                        //Enter Goal State from user
    printf("\nEnter GoalState: \n");
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            cout << "Element (" << i << "," << j << "): ";
            cin >> goalState[i][j];
        }
    }
    printf("\nGoal State = \n");
    printState(goalState);       

    int initialState[r][c];
    int x, y;                               //initialState[x][y]==0 for location of x in initial state
    printf("\nEnter InitialState: \n");
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            cout << "Element (" << i << "," << j << "): ";
            cin >> initialState[i][j];
            if (initialState[i][j] == 0) {          //0 location in initial state
                x = i;
                y = j;
            }
        }
    }
    printf("\nInitial State = \n");
    printState(initialState);

    int searchType;
    printf("\nSelect Search Algorithm to apply (A Star = 1, Greedy First Search = 2): ");
    cin >> searchType;

    int heuristicType;
    printf("\nSelect Heuristic Function to apply (Manhattan Distance = 1, Misplaced Tiles = 2): ");
    cin >> heuristicType;
     
    if (searchType==1) {
        printf("\nSolving using A* Search.....\n");
        ASTAR(goalState, initialState, x, y, heuristicType);
    }
    else if (searchType == 2) {
        printf("\nSolving using Greedy First Search.....\n");
        GreedyFirst(goalState, initialState, x, y, heuristicType);
    }

    return 0;
}