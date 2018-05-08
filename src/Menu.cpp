#include "Menu.h"



Menu::Menu()
{
    //ctor
        int gSize = 10;
     gridSize = gSize;

    unitWidth = (float)2/gridSize;
    liveChest = true;
    liveSetOfArrws=true;
}


Menu::~Menu()
{
    //dtor
}
void Menu::loadBackgroundImage(char* FileName)
{
    bakTex = TextureLoader(FileName);
}

void Menu::loadLevelOptionImage(char* FileName)
{
    ArrBnchTex =  TextureLoader(FileName);
}

void Menu::loadChestImage(char*)
{

}

void Menu::drawBackground()
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

void Menu::drawGrid()
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

void Menu::drawLevelOption()
{

glColor3f(1.0,1.0,1.0);

    glTranslatef(setlevelOptionLoc.x,setlevelOptionLoc.y,1.0);
    glRotated(-spin,0,0,1);

    glBindTexture(GL_TEXTURE_2D,ArrBnchTex);
    glScaled(2.0/(float)(gridSize),2.0/(float)(gridSize),2.0);

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

void Menu::drawChest()
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

int Menu::getGridSize()
{
  return gridSize;
}

void Menu::placeChest(int, int)
{

}

GridLoc Menu::GetChestLoc()
{

}

void Menu::PlaceLevelOption(int x, int y)
{
   setlevelOptionLoc.x =  converter(x,y).x;
   setlevelOptionLoc.y =  converter(x,y).y;
}

GridLoc Menu::GetLevelOptionLoc()
{
 GridLoc val;

   val.x = (int)(ceil((setlevelOptionLoc.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((setlevelOptionLoc.y +(1-unitWidth))/unitWidth));

   return val;
}

loc Menu::converter(int x , int y)
{
      loc val;

        x+=1;
        y+=1;
       val.x = -1-unitWidth/2+(unitWidth)*x;
       val.y = -1-unitWidth/2+(unitWidth)*y;
       return val;
}
