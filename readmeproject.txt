Harkaranjeet Singh
Parth Thacker

Instructor – Proff. H. Cecotti
Course - CSCI 115
Project Report

TO compile:
-The project must be compiled in codeblocks IDE to initiate the game. Main file must be compiled and run.
-Other alternative to it is to click on the .exe file named Maze.

-This project consist of turn based game in which we have one user and upto ten enemies.
-Objective is to either kill all the enemies with the arrow or get to the treasure chest.
-Initially the player doesn't have any arrow, he collects arrow by walking into the box containing arrows.
-If the enemy is able to reach till the player, the player dies and the game ends.  
-If you want to check other classes, they are located in the src folder.


-The implementation of player, enemy, maze, menu and wall can be found in this folder.
-Apart from this there is a folder named level which contains the text files representing the multiple levels of the game.
-Image folder contains all the images of enemy, users, wall, background.

-In main class based on the level selected from the menu, the text file representing the grid is loaded and displayed on the screen.
-The file data is stored in a 2 X 2 array named _grid[row][col]. So, the A* algorithm is used to calculate the shortest path between each enemy and the player,
 based on the enemy and player location in _grid array. Once the player makes a move, the A* algorithm calculate the shortest distance between each enemy and the player,
 and make the enmy move one step towards the shortest path. 


-Initially the player has zero arrows and he collects arrows by moving over the grid containing the arrow.
-After collecting arow, the player can shoot and kill the enemy.
-The game ends if the player reachs the treasure chest.
-Game begins with the menu where user is able to select a difficulty level and proceed into the game. The player walks in the game using the keys-W,A,S,D
 and shoots arrows with the arrow keys.
