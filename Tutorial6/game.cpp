#include "game.h"
#include <Windows.h>
#include <stdio.h>

void setupMap( SGame* gameObject ) 	// initialize game map
{
    gameObject->Map.Width = 32; // Set a proper width for our map, which has to be less than MAX_MAP_WIDTH
    gameObject->Map.Depth = 15; // Same for map depth   

    for( int z=0; z< gameObject->Map.Depth; z = z+1 ) // iterate over every row
    {
        for( int x=0; x< gameObject->Map.Width; x++ ) // iterate over every column for the z row
        {
            gameObject->Map.FloorCells[z][x] = TILE_GRASS; // initialize the (x,z) map cell to "grass"
			gameObject->Map.EnemyCells[z][x] = INVALID_ENEMY;	// initialize the cell to an invalid enemy
        }
    }

    // set a wall border
    for( int x=0; x < gameObject->Map.Width; x++ )   
	{
        gameObject->Map.FloorCells[0][x] = TILE_WALL; // set all cells in the first row [0]   
        gameObject->Map.FloorCells[gameObject->Map.Depth-1][x] = TILE_WALL; // set all cells in the last row [depth-1]
	}
    for( int z=0; z < gameObject->Map.Depth; z++ )   
	{
        gameObject->Map.FloorCells[z][0] = TILE_WALL; // set all cells for the first column [0]   
        gameObject->Map.FloorCells[z][gameObject->Map.Width-1] = TILE_WALL; // set all cells for the last column [width-1]
	}
};

// Use this function to setup player at level startup.
void setupPlayer( SGame* gameObject )
{
	// set some initial configuration to the game characters
	gameObject->Player.CurrentPoints.HP				= 100; 
	gameObject->Player.x = 5, gameObject->Player.z	= 5;
}

// Use this function to setup enemy list at level startup.
void setupEnemies( SGame* gameObject )
{
#define INITIAL_ENEMY_COUNT 4
	for( int iEnemy=0; iEnemy < INITIAL_ENEMY_COUNT; iEnemy++ )
	{
		SCharacter newEnemy;
		newEnemy.MaxPoints		= { 100, 50, 1000000 }; // HP, MP and XP
		newEnemy.CurrentPoints	= { 100, 50, 0 };
		newEnemy.x = rand() % gameObject->Map.Width;
		newEnemy.z = rand() % gameObject->Map.Depth;

		gameObject->Enemy.push_back( newEnemy ); // copy the new enemy as a new element at the end of our enemy list.
	}
}

// Use this function to update the map tiles
void updateMap( SGame* gameObject, float fLastFrameTime  ) 
{
}

// Use this function to update the player
void updatePlayer( SGame* gameObject, float fLastFrameTime  ) 
{
	if(GetAsyncKeyState(VK_UP))
		gameObject->Player.z -= 1; // decrease by 1
	if(GetAsyncKeyState(VK_DOWN))
		gameObject->Player.z += 1; // increase by 1
	if(GetAsyncKeyState(VK_RIGHT))
		gameObject->Player.x ++; // increase by 1
	if(GetAsyncKeyState(VK_LEFT))
		gameObject->Player.x --; // decrease by 1 
}

// Use this function to update the enemies
void updateEnemies( SGame* gameObject, float fLastFrameTime ) 
{
	for( int z = 0; z < gameObject->Map.Depth; z++ ) // clear all enemy rows to refresh the enemy map layer
		memset( gameObject->Map.EnemyCells[z], INVALID_ENEMY, sizeof(int)*gameObject->Map.Width );

	for( unsigned int iEnemy=0; iEnemy < gameObject->Enemy.size(); iEnemy++ )
	{
		SCharacter* currentEnemy = &gameObject->Enemy[iEnemy]; // get the address of the current enemy at [iEnemy] index

		if( gameObject->Player.x < currentEnemy->x )
			currentEnemy->x -= 1;	// decrease enemy x if player is in that direction
		else if( gameObject->Player.x > currentEnemy->x )
			currentEnemy->x += 1;	// increase enemy x if player is in that direction
	
		if( gameObject->Player.z < currentEnemy->z )
			currentEnemy->z --;	// decrease enemy z if player is in that direction
		else if( gameObject->Player.z > currentEnemy->z )
			currentEnemy->z ++;	// increase enemy z if player is in that direction

		if( gameObject->Player.z == currentEnemy->z 
			&& gameObject->Player.x == currentEnemy->x )
		{
			gameObject->Player.CurrentPoints.HP -= 1; // Decrease player life if enemy position matches player position
			currentEnemy->x = rand()%gameObject->Map.Width;
			currentEnemy->z = rand()%gameObject->Map.Depth;	// set a random position for the enemy so it has to walk again in order to hit the player
		}
		gameObject->Map.EnemyCells[currentEnemy->z][currentEnemy->x] = iEnemy; // assign enemy index to the cell corresponding to this enemy
	}
}

// This function prints the game map on the console
void drawASCIIMap( const SGame* gameObject )
{
	char imageMap[4096]={};	// We're going to draw our map in this array. the assignment of empty brackets = {} initializes all chars in the array to 0
	int nImageCursor = 0;	// The position where we shuold position our next character

	for( int z=0; z< gameObject->Map.Depth; z++ ) // iterate over every row
    {
        for( int x=0; x< gameObject->Map.Width; x++ ) // iterate over every column for the z row
        {
			if( gameObject->Player.x == x && gameObject->Player.z == z )
				imageMap[nImageCursor++] = 'P'; // draw the player as an ascii character
			else if( gameObject->Map.EnemyCells[z][x] != INVALID_ENEMY )
				imageMap[nImageCursor++] = 'E'+gameObject->Map.EnemyCells[z][x]; // draw the enemy as an ascii character
			else
				imageMap[nImageCursor++] = gameObject->Map.FloorCells[z][x] ? gameObject->Map.FloorCells[z][x] : ' '; // draw the tile as an ascii character
        }
        imageMap[nImageCursor++] = '\n'; // \n is the code character for "new line" inside a text. We use it to display the next cells in the next row.
    }
    imageMap[nImageCursor] = 0; // a null char indicates the end of the character string or text.

	printf( "%s", imageMap );
}

// This function prints miscelaneous game info
void drawASCIIGameInfo( const SGame* gameObject )
{
    printf( "- Player health: %i\n"
			"- Player position: (%i, %i)\n"
			"- Enemy count: %i\n"
			"Move (P)layer by pressing the arrow keys to prevent being touched by enemies E, F, G and H.\n", 
			gameObject->Player.CurrentPoints.HP, gameObject->Player.x, gameObject->Player.z, gameObject->Enemy.size() );
}