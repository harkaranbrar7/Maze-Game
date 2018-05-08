/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <Maze.h>


Maze::Maze()
{

}

Maze::Maze(int gSize)                                        // Constructor
{
    //ctor
    gridSize =gSize;

    unitWidth = (float)2/gridSize;
    liveChest = true;
    liveSetOfArrws=true;
}

Maze::~Maze()
{
    //dtor
}
void Maze::loadChestImage(char* FileName)
{
    chestTex = TextureLoader(FileName);
}

void Maze::loadBackgroundImage(char* FileName)
{
   bakTex = TextureLoader(FileName);
}

void Maze::loadSetOfArrowsImage(char* FileName)
{
    ArrBnchTex =  TextureLoader(FileName);
}

void Maze::placeChest(int x, int y)
{
   chestLoc.x =  converter(x,y).x;
   chestLoc.y =  converter(x,y).y;
}

void Maze::placeStArrws(int x, int y)
{
   setOfArrsLoc.x =  converter(x,y).x;
   setOfArrsLoc.y =  converter(x,y).y;
}



GridLoc Maze::GetChestLoc()
{
   GridLoc val;

   val.x = (int)(ceil((chestLoc.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((chestLoc.y +(1-unitWidth))/unitWidth));

   return val;
}



GridLoc Maze::GetStArrwsLoc()
{
    GridLoc val;

   val.x = (int)(ceil((setOfArrsLoc.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((setOfArrsLoc.y +(1-unitWidth))/unitWidth));

   return val;
}


int Maze::getGridSize()
{
  return gridSize;
}


void Maze::drawBackground()
{
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,bakTex);

    glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);

       glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);
     glEnd();
}


void Maze::drawGrid()
{
   float a;
   glColor4f(1.0,1.0,1.0,0.2);
   glDisable(GL_TEXTURE_2D);
   glPointSize(1);
   glBegin(GL_LINES);

   for(int i =0;i<gridSize;i++)
    {
         a = -1+(float)unitWidth*i;
         glVertex3f(a,1,-0.4);
         glVertex3f(a,-1,-0.4);
         glVertex3f(-1,a,0.4);
         glVertex3f(1,a,0.4);
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void Maze::drawArrows()
{
    if(liveSetOfArrws)
    {

    glColor3f(1.0,1.0,1.0);

    glTranslatef(setOfArrsLoc.x,setOfArrsLoc.y,1.0);
    glRotated(-spin,0,0,1);

    glBindTexture(GL_TEXTURE_2D,ArrBnchTex);
    glScaled(1.0/(float)(gridSize),1.0/(float)(gridSize),1.0);

    glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);

       glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);
     glEnd();
    }
}

void Maze::drawChest()
{
    if(liveChest){
    glColor3f(1.0,1.0,1.0);

    glTranslatef(chestLoc.x,chestLoc.y,1.0);

    glBindTexture(GL_TEXTURE_2D,chestTex);
    glScaled(1.0/(float)(gridSize+5),1.0/(float)(gridSize+5),1.0);
    glRotated(spin,0,0,1);

    glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);

       glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);
     glEnd();

     spin +=0.5;
    }
}

loc Maze::converter(int x, int y)
{
       loc val;

        x+=1;
        y+=1;
       val.x = -1-unitWidth/2+(unitWidth)*x;
       val.y = -1-unitWidth/2+(unitWidth)*y;
       return val;
}


