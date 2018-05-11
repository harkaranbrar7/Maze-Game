/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <string.h>
#include <CommonThings.h>
#include <Maze.h>
#include <iostream>
#include <Timer.h>
#include <player.h>
#include <fstream>
#include <Menu.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <Enemies.h>

#include <wall.h>
#include <math.h>
#include<bits/stdc++.h>
#include<vector>
#include<bits/stdc++.h>
#define ROW 10
#define COL 10

/* GLUT callback Handlers */

using namespace std;

Maze *M = new Maze(10);                         // Set Maze grid size
Player *P = new Player();                       // create player
Menu *menu = new Menu();

wall W[100];                                    // wall with number of bricks
Enemies E[10];                                  // create number of enemies
Timer *T0 = new Timer();                        // animation timer

float wWidth, wHeight;                          // display window width and Height
float xPos,yPos;                                // Viewpoar mapping

int row, col, wallCount, enemyCount;
int _grid[10][10];
vector<int> moves;
int state = 0;
//int visited[10][10];
const char* FileName;

void display(void);                             // Main Display : this runs in a loop
void solidsearch();
void Placer();

void initialize()
{
    moves.at(2)=0;
    moves.at(3)=0;
}

void resize(int width, int height)              // resizing case on the window
{
    wWidth = width;
    wHeight = height;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}
//====================================================================================================================================================
//=============================================== HELPER FUNCTION ====================================================================================
//====================================================================================================================================================


// implement A* Search Algorithm


// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;

// A structure to hold the neccesary parameters
struct cell
{
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
};

// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < ROW) &&
           (col >= 0) && (col < COL);
}

// A Utility Function to check whether the given cell is
// blocked or not
bool isUnBlocked(int grid[][COL], int row, int col)
{
    // Returns true if the cell is not blocked else false
    if (grid[row][col] != 1)
        return (true);
    else
        return (false);
}

// A Utility Function to check whether destination cell has
// been reached or not
bool isDestination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(int row, int col, Pair dest)
{
    // Return using the distance formula
    return ((double)sqrt ((row-dest.first)*(row-dest.first)
                          + (col-dest.second)*(col-dest.second)));
}

// A Utility Function to trace the path from the source
// to destination
void tracePath(cell cellDetails[][COL], Pair dest)
{
   // printf ("\nThe Path is ");
    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;

    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col ))
    {
        Path.push (make_pair (row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.push (make_pair (row, col));
    while (!Path.empty())
    {
        pair<int,int> p = Path.top();
        Path.pop();
       // printf("-> (%d,%d) ",p.first,p.second);
        moves.push_back(p.first);
        moves.push_back(p.second);


    }

    return;
}

// A Function to find the shortest path between
// a given source cell to a destination cell according
// to A* Search Algorithm
void aStarSearch(int grid[][COL], Pair src, Pair dest,int countl)
{
    // If the source is out of range
    if (isValid (src.first, src.second) == false)
    {
        printf ("Source is invalid\n");
        return;
    }

    // If the destination is out of range
    if (isValid (dest.first, dest.second) == false)
    {
        printf ("Destination is invalid\n");
        return;
    }

    // Either the source or the destination is blocked
    if (isUnBlocked(grid, src.first, src.second) == false ||
            isUnBlocked(grid, dest.first, dest.second) == false)
    {
        printf ("Source or the destination is blocked\n");
        return;
    }

    // If the destination cell is the same as source cell
    if (isDestination(src.first, src.second, dest) == true)
    {
        printf ("We are already at the destination\n");
        return;
    }

    // Create a closed list and initialise it to false which means
    // that no cell has been included yet
    // This closed list is implemented as a boolean 2D array
    bool closedList[ROW][COL];
    memset(closedList, false, sizeof (closedList));

    // Declare a 2D array of structure to hold the details
    //of that cell
    cell cellDetails[ROW][COL];

    int i, j;

    for (i=0; i<ROW; i++)
    {
        for (j=0; j<COL; j++)
        {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    /*
     Create an open list having information as-
     <f, <i, j>>
     where f = g + h,
     and i, j are the row and column index of that cell
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
     This open list is implenented as a set of pair of pair.*/
    set<pPair> openList;

    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair (0.0, make_pair (i, j)));

    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;

    while (!openList.empty())
    {
        pPair p = *openList.begin();

        // Remove this vertex from the open list
        openList.erase(openList.begin());

        // Add this vertex to the open list
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

       /*
        Generating all the 8 successor of this cell

            N.W   N   N.E
              \   |   /
               \  |  /
            W----Cell----E
                 / | \
               /   |  \
            S.W    S   S.E

        Cell-->Popped Cell (i, j)
        N -->  North       (i-1, j)
        S -->  South       (i+1, j)
        E -->  East        (i, j+1)
        W -->  West           (i, j-1)
        */

        // To store the 'g', 'h' and 'f' of the 8 successors
        double gNew, hNew, fNew;

        //----------- 1st Successor (North) ------------

        // Only process this cell if this is a valid one
        if (isValid(i-1, j) == true)
        {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i-1, j, dest) == true)
            {
                // Set the Parent of the destination cell
                cellDetails[i-1][j].parent_i = i;
                cellDetails[i-1][j].parent_j = j;
                printf ("The destination cell is found\n");
                tracePath (cellDetails, dest);

                foundDest = true;
                return;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i-1][j] == false &&
                     isUnBlocked(grid, i-1, j) == true)
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue (i-1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i-1][j].f == FLT_MAX ||
                        cellDetails[i-1][j].f > fNew)
                {
                    openList.insert( make_pair(fNew,
                                               make_pair(i-1, j)));

                    // Update the details of this cell
                    cellDetails[i-1][j].f = fNew;
                    cellDetails[i-1][j].g = gNew;
                    cellDetails[i-1][j].h = hNew;
                    cellDetails[i-1][j].parent_i = i;
                    cellDetails[i-1][j].parent_j = j;
                }
            }
        }

        //----------- 2nd Successor (South) ------------

        // Only process this cell if this is a valid one
        if (isValid(i+1, j) == true)
        {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i+1, j, dest) == true)
            {
                // Set the Parent of the destination cell
                cellDetails[i+1][j].parent_i = i;
                cellDetails[i+1][j].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);

                foundDest = true;
                return;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i+1][j] == false &&
                     isUnBlocked(grid, i+1, j) == true)
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i+1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i+1][j].f == FLT_MAX ||
                        cellDetails[i+1][j].f > fNew)
                {
                    openList.insert( make_pair (fNew, make_pair (i+1, j)));
                    // Update the details of this cell
                    cellDetails[i+1][j].f = fNew;
                    cellDetails[i+1][j].g = gNew;
                    cellDetails[i+1][j].h = hNew;
                    cellDetails[i+1][j].parent_i = i;
                    cellDetails[i+1][j].parent_j = j;
                }
            }
        }

        //----------- 3rd Successor (East) ------------

        // Only process this cell if this is a valid one
        if (isValid (i, j+1) == true)
        {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j+1, dest) == true)
            {
                // Set the Parent of the destination cell
                cellDetails[i][j+1].parent_i = i;
                cellDetails[i][j+1].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i][j+1] == false &&
                     isUnBlocked (grid, i, j+1) == true)
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue (i, j+1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cellDetails[i][j+1].f == FLT_MAX ||
                        cellDetails[i][j+1].f > fNew)
                {
                    openList.insert( make_pair(fNew,
                                        make_pair (i, j+1)));

                    // Update the details of this cell
                    cellDetails[i][j+1].f = fNew;
                    cellDetails[i][j+1].g = gNew;
                    cellDetails[i][j+1].h = hNew;
                    cellDetails[i][j+1].parent_i = i;
                    cellDetails[i][j+1].parent_j = j;
                }
            }
        }

        //----------- 4th Successor (West) ------------

        // Only process this cell if this is a valid one
        if (isValid(i, j-1) == true)
        {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j-1, dest) == true)
            {
                // Set the Parent of the destination cell
                cellDetails[i][j-1].parent_i = i;
                cellDetails[i][j-1].parent_j = j;
                printf("The destination cell is found\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i][j-1] == false &&
                     isUnBlocked(grid, i, j-1) == true)
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j-1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i][j-1].f == FLT_MAX ||
                        cellDetails[i][j-1].f > fNew)
                {
                    openList.insert( make_pair (fNew,
                                          make_pair (i, j-1)));

                    // Update the details of this cell
                    cellDetails[i][j-1].f = fNew;
                    cellDetails[i][j-1].g = gNew;
                    cellDetails[i][j-1].h = hNew;
                    cellDetails[i][j-1].parent_i = i;
                    cellDetails[i][j-1].parent_j = j;
                }
            }
        }


 }
    if (foundDest == false)
        printf("Failed to find the Destination Cell\n");

    return;
}


void show()
{
    for(int i=0;i<col;i++)
        {for(int j=0;j<row;j++)
           {
               cout<<_grid[i][j]<<" ";
}
cout<<endl;
}
cout<<endl;
}




void LocationChecker()
{
               cout<<" Player                  "<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
               cout<<" Arrow                   "<<P->getArrowLoc().x<<"     "<<P->getArrowLoc().y<<endl;

      for(int i=0;i<10;i++)
            {
                cout<<"Enemy number = "<<i<<" ";
                cout<<" Enemy  "<<E[i].getEnemyLoc().x<<"    "<<E[i].getEnemyLoc().y<<endl;

            }


      //   for(int i=0;i<100;i++)
     //       {
      //          cout<<"Wall number = "<<i<<endl;
      //          cout<<"  Wall  "  <<W[i].GetWallLoc.x<<"    "<<W[i].GetWallLoc.y<<endl;
      //      }
}


void solidsearch(){




}
void collisionDetector()
{
    //LocationChecker();



            int count1=0,x1,x2,x3,x4;
            for(int i=0;i<row;i++)
            {
                for(int j=0;j<col;j++)
                {
                    if(_grid[i][j]==20 || _grid[i][j]==21 || _grid[i][j]==22||_grid[i][j]==23 ||
                        _grid[i][j]==24 || _grid[i][j]==25 || _grid[i][j]==26 || _grid[i][j]==27 || _grid[i][j]==28 || _grid[i][j]==29 )
                    {
                        x1=E[count1].getEnemyLoc().x;
                        x2=E[count1].getEnemyLoc().y;
                        x3=P->getPlayerLoc().x;
                        x4=P->getPlayerLoc().y;

                    cout<<"eNEMY  "<<x1<<" "<<x2<<endl;
                    cout<<"PlAYER "<<x3<<" "<<x4<<endl;
                      Pair src = make_pair(x1, x2);
                      Pair dest = make_pair(x3, x4);

                    aStarSearch(_grid, src, dest,count1);
               // cout<<endl;
              //  cout<<"Enemy E"<<count1<<":"<<moves.at(2)<<" "<<moves.at(3)<<endl;
              //  cout<<"Enemy E"<<count1<<":"<<x1<<"   "<<x2<<endl;




               if(x1-1==moves.at(2)  &&  x2==moves.at(3))
               E[count1].moveEnemy("left");
               else if(x1+1==moves.at(2) && x2==moves.at(3))
              E[count1].moveEnemy("right");
               else if(x1==moves.at(2) && x2-1==moves.at(3))
                E[count1].moveEnemy("down");
               else if(x1==moves.at(2) && x2+1==moves.at(3))
                 E[count1].moveEnemy("up");

                initialize();
                        moves.clear();
                         count1++;
                    }
                }
            }


             for(int i=0;i<10;i++)
                {

                       if (E[i].getEnemyLoc().x == P->getPlayerLoc().x && E[i].getEnemyLoc().y == P->getPlayerLoc().y)
                        {
                            LocationChecker();
                            cout<<" collision with Enemy "<<endl;
                            state=2;
                            P->livePlayer=false;

                        }
            }



             for(int i=0;i<10;i++)
                {

                       if (E[i].getEnemyLoc().x == P->getArrowLoc().x && E[i].getEnemyLoc().y == P->getArrowLoc().y)
                        {
                            LocationChecker();
                            cout<<" Enemy dead "<<endl;

                            E[i].live = false;

                        }
            }




            if (M->GetChestLoc().x == P->getPlayerLoc().x && M->GetChestLoc().y ==P->getPlayerLoc().y)
            {
                state =3;
                cout<<"won"<<endl;
                M->liveChest = false;
            }


            if (M->GetStArrwsLoc().x == P->getPlayerLoc().x && M->GetStArrwsLoc().y ==P->getPlayerLoc().y)
            {
                P->arrowStatus = true;
                M->liveSetOfArrws = false;
            }

           //  cout<<"Number of Enemies:"<<count1<<endl;


}

//====================================================================================================================================================
//====================================================================================================================================================
//====================================================================================================================================================

void Placer(){

   // FILE * f1;
    fstream f1;
    f1.open(FileName);
    //f1.open("Level/Level1.txt");
    if (!f1.is_open())
    {
        cout<<"Unable to open"<<endl;
    }
    else
    {
        f1>>row>>col;
       for(int i=0;i<row;i++)
       {
           for(int j=0;j<col;j++)
           {
               int s;
               f1>>s;
            _grid[i][j]=s;
           }
       }

    }

    show();

   for(int i=0;i<row;i++)
        {for(int j=0;j<col;j++)
           {
               if(_grid[i][j] == 1)         // 1 is for wall
               {
                //   cout<<" "<<i<<"  "<<j<<"  "<<wallCount<<endl;
                            W[wallCount].wallInit(M->getGridSize(),"images/wall.png");// Load walls
                            W[wallCount].placeWall(i,j);                              // place each brick
                                wallCount++;

               }
               else if (_grid[i][j] == 3)
               {
                     P->initPlayer(M->getGridSize(),"images/p.png",6);   // initialize player pass grid size,image and number of frames
                    P->placePlayer(i,j);
               }
                else if (_grid[i][j] == 4)
                {
                        M->loadChestImage("images/chest.png");              // load chest image
                        M->placeChest(i,j);                                 // place chest in a grid

                }
                else if (_grid[i][j] == 5)
                {
                            M->loadSetOfArrowsImage("images/arrwset.png");      // load set of arrows image
                            M->placeStArrws(i,j);                               // place set of arrows

                }
                else if (_grid[i][j] >=20 && _grid[i][j] <=29)
                {
                            E[enemyCount].initEnm(M->getGridSize(),4,"images/e.png"); //Load enemy image
                            E[enemyCount].placeEnemy(i,j);
                            enemyCount++;
                }




}

}

}

void init()
{
    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0, wWidth, 0, wHeight);

    T0->Start();                                        // set timer to 0

    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




    M->loadBackgroundImage("images/bak.jpg");           // Load maze background image
    P->loadArrowImage("images/arr.png");                // Load arrow image

}


void MenuState()
{
           menu->loadBackgroundImage("images/mainmenu.jpg");

      cout<<"Main Menu "<<endl;

      glClear (GL_COLOR_BUFFER_BIT);        // clear display screen


            glPushMatrix();
            menu->drawBackground();
            glPopMatrix();


        glutSwapBuffers();
}

void Gamestate()
{

glClear (GL_COLOR_BUFFER_BIT);        // clear display screen


        glPushMatrix();
         M->drawBackground();
        glPopMatrix();


        for(int i=0; i<100;i++)
        {
           W[i].drawWall();
        }

        glPushMatrix();
            M->drawGrid();
        glPopMatrix();

        glPushMatrix();
            P->drawplayer();
        glPopMatrix();

        for(int i=0; i<10;i++)
        {
        E[i].drawEnemy();
        }

        glPushMatrix();
            P->drawArrow();
        glPopMatrix();

         glPushMatrix();
           M->drawChest();
        glPopMatrix();

        glPushMatrix();
           M->drawArrows();
        glPopMatrix();
        glutSwapBuffers();
}


void GameOver()
{

       menu->loadBackgroundImage("images/gameover.jpg");
      //  menu->loadLevelOptionImage("images/mainMenu.png");

      cout<<"Game Over"<<endl;

      glClear (GL_COLOR_BUFFER_BIT);        // clear display screen


            glPushMatrix();
            menu->drawBackground();
            glPopMatrix();


        glutSwapBuffers();
}
void Won()
{
        menu->loadBackgroundImage("images/won.jpg");
      //  menu->loadLevelOptionImage("images/mainMenu.png");

      cout<<"Won"<<endl;

      glClear (GL_COLOR_BUFFER_BIT);        // clear display screen


            glPushMatrix();
            menu->drawBackground();
            glPopMatrix();


        glutSwapBuffers();

}

void display(void)
{

    if (state ==0)
    {

        MenuState();

    }
    else if(state ==1)
        {

            Gamestate();           // Game State
        }
    else if(state ==2)
    {
        GameOver();
    }
    else if (state==3)
    {
        Won();
    }


}



void key(unsigned char key, int x, int y)
{
     int x1=P->getPlayerLoc().x;
    int y1=P->getPlayerLoc().y;

    switch (key)
    {
        //case ' ':
        //     P->shootArrow();
        break;
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
        case 119:


           // LocationChecker();

         if(_grid[x1][y1+1]!=1)
            P->movePlayer("up",P->frames);

              solidsearch();
              collisionDetector();

    break;

    case 115:
           //  LocationChecker();
           collisionDetector();
        if(_grid[x1][y1-1]!=1)
        P->movePlayer("down",P->frames);

      solidsearch();
       collisionDetector();

    break;

    case 65:
    case 'a':
        // LocationChecker();

          if(_grid[x1-1][y1]!=1)
         P->movePlayer("left",P->frames);
          solidsearch();
           collisionDetector();

        break;

    case 100:
       //  LocationChecker();

         if(_grid[x1+1][y1]!=1)
        P->movePlayer("right",P->frames);
         solidsearch();
          collisionDetector();

            break;

    case '1':
        if (state==0)
        {
            FileName = "Level/Level1.txt";
            state=1;
            Placer();
        }

    break;
    case '2':
            FileName = "Level/Level2.txt";
            state=1;
            Placer();
        break;
    case '3':
            FileName = "Level/Level3.txt";
            state=1;
            Placer();
        break;
    case '4':
            FileName = "Level/Level4.txt";
            state=1;
            Placer();
        break;
    case 'b':
        if (state == 2||state ==3)
        {

            state =0;
        }
    }

    glutPostRedisplay();
}


 void GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    xPos =posX ;
    yPos =posY ;
}

 void idle(void)
{

    //Your Code here


    glutPostRedisplay();
}


void mouse(int btn, int state, int x, int y){

    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;


      case GLUT_RIGHT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;
    }
     glutPostRedisplay();
};



void Specialkeys(int key, int x, int y)
{
      switch(key)
    {
    case GLUT_KEY_UP:
        P->shootArrow("up");
    break;

    case GLUT_KEY_DOWN:
     P->shootArrow("down");

    break;

    case GLUT_KEY_LEFT:
     P->shootArrow("left");


    break;

    case GLUT_KEY_RIGHT:
         P->shootArrow("right");
    break;

   }


  glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

   glutInitWindowSize (800, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Maze");                    //program title
   init();



    glutDisplayFunc(display);                     //callback function for display
    glutReshapeFunc(resize);                      //callback for reshape
    glutKeyboardFunc(key);                        //callback function for keyboard
    glutSpecialFunc(Specialkeys);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutMainLoop();

   return EXIT_SUCCESS;
}
