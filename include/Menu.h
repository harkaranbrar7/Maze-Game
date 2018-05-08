#ifndef MENU_H
#define MENU_H

#include<CommonThings.h>
#include<string.h>
#include<gl/gl.h>
#include<GL/glut.h>
#include<SOIL.h>
#include<iostream>
#include <Timer.h>
#include <math.h>       /* ceil */

using namespace std;


class Menu
{
    public:
        Menu();
        virtual ~Menu();
      void loadBackgroundImage(char *);   // set an image for the background
      void loadLevelOptionImage(char *);  // set an image for the level Option
      void loadChestImage(char *);        // set an image for the Arrow

      void drawBackground();              // Draw background image
      void drawGrid();                    // Draw Grid
      void drawLevelOption();             // Draw level Option
      void drawChest();                   // Draw Chest

      int getGridSize();                  // return grid size

      void placeChest(int,int);           // Place Chest
      GridLoc GetChestLoc();              // Return grid location of the chest

      void PlaceLevelOption(int,int);         // Place set of Leveloption
      GridLoc GetLevelOptionLoc();            // Return grid location of the level option

      float spin;                         // for spin chest
      bool liveChest;                     // status of the chest (collected or not)
      bool liveSetOfArrws;                // status of the arrows(collected or not)


    protected:

      int gridSize;                       // grid size
      float unitWidth;                    // Unit width of the grid

    private:

      GLuint bakTex;                       // Handler for background texture
      GLuint ArrBnchTex;                   // Handler for set of arrows texture
      GLuint chestTex;                     // Handler for Chest texture
      loc chestLoc;                        // chest Location
      loc setlevelOptionLoc;                    // level option Location
      loc converter(int, int);             // convert grid size to viewport
};

#endif // MENU_H
