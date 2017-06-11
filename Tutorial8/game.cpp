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
	// set some initial configuration to the player character
	gameObject->Player.CurrentPoints.HP				= 20; 
	gameObject->Player.x = 5, gameObject->Player.z	= 5;
	gameObject->Player.Direction = 0.0f;

	gameObject->Player.PositionDeltas = {0,0};
	gameObject->Player.Speed	= 5.0f;
}

// Use this function to setup enemy list at level startup.
void setupEnemies( SGame* gameObject )
{
#define INITIAL_ENEMY_COUNT 5
	for( int iEnemy=0; iEnemy < INITIAL_ENEMY_COUNT; iEnemy++ )
	{
		SCharacter newEnemy;
		newEnemy.MaxPoints		= { 100, 50, 1000000 }; // HP, MP and XP
		newEnemy.CurrentPoints	= { 100, 50, 0 };
		newEnemy.x = rand() % gameObject->Map.Width;
		newEnemy.z = rand() % gameObject->Map.Depth;
		newEnemy.PositionDeltas = {0,0};
		newEnemy.Direction = 0.0f;
		newEnemy.Speed = float(iEnemy); // generate the speed from the enemy index

		gameObject->Enemy.push_back( newEnemy ); // copy the new enemy as a new element at the end of our enemy list.
	}
};

void refreshPosFromDeltas( SCharacter* character )
{
	SVector2 *charDeltas = &character->PositionDeltas; // get pointer to deltas
	
	// Now we increase cell units and decrease deltas until the deltas are between 0 and 0.9999999999999999999999
	// Update X coordinate
	while( charDeltas->x >= 1.0f )
	{
		character->x += 1;
		charDeltas->x -= 1.0f;	
	}

	while( charDeltas->x < 0.0f )
	{
		character->x -= 1;
		charDeltas->x += 1.0f;
	}

	// Update Y coordinate
	while( charDeltas->y >= 1.0f )
	{
		character->z += 1;
		charDeltas->y -= 1.0f;
	}

	while( charDeltas->y < 0.0f )
	{
		character->z -= 1;
		charDeltas->y += 1.0f;
	}
}

// Use this function to update the map tiles
void updateMap( SGame* gameObject, float fLastFrameTime  ) 
{
}

// Use this function to update the player
void updatePlayer( SGame* gameObject, float fLastFrameTime  ) 
{
	// Query key states
	bool _keyUP		= GetAsyncKeyState('W') ? true : false,
		_keyDOWN	= GetAsyncKeyState('S') ? true : false,
		_keyLEFT	= GetAsyncKeyState('A') ? true : false,
		_keyRIGHT	= GetAsyncKeyState('D') ? true : false;

	SVector2 dir = {0,0};
	// set target direction from keys pressed
	if(_keyRIGHT)
		dir.x ++; //dir.x = 1;
	if(_keyLEFT)
		dir.x --; //dir.x = -1;
	if(_keyUP)
		dir.y --; //dir.y = -1;
	if(_keyDOWN)
		dir.y ++; //dir.y = 1;

	// normalize the new direction vector (make it a unit length so we can multiply it by speed to get velocity vector)
	dir.Normalize();

	// we want to walk only if a direction key was pressed
	bool dirKeyPressed = _keyRIGHT || _keyLEFT || _keyUP || _keyDOWN;
	if( dirKeyPressed )
	{
		gameObject->Player.Direction = SVector2(1, 0).AngleWith(dir);
		if( dir.y < 0 ) // negate rotation if more than 180º
			gameObject->Player.Direction = gameObject->Player.Direction*-1;

		// Increase 50% speed if left shift pressed.
		float fSpeed = GetAsyncKeyState(VK_LSHIFT) ? gameObject->Player.Speed*1.5f : gameObject->Player.Speed;
		dir.Scale(fSpeed*fLastFrameTime);

		// integrate our calculated displacement
		gameObject->Player.PositionDeltas = gameObject->Player.PositionDeltas + dir;
	}

	// refresh tile coords now that we have accumulated the distance walked this frame
	refreshPosFromDeltas( &gameObject->Player );
}

// Use this function to update the enemies
void updateEnemies( SGame* gameObject, float fLastFrameTime ) 
{
	for( int z = 0; z < gameObject->Map.Depth; z++ ) // clear all enemy rows to refresh the enemy map layer
		memset( gameObject->Map.EnemyCells[z], INVALID_ENEMY, sizeof(int)*gameObject->Map.Width );

	// where iEnemy stands for "i"ndex of enemy.
	for( unsigned int iEnemy=0; iEnemy < gameObject->Enemy.size(); iEnemy++ )
	{
		SCharacter* currentEnemy = &gameObject->Enemy[iEnemy]; // get the address of the current enemy at [iEnemy] index
		SVector2* enemyDeltas = &currentEnemy->PositionDeltas;

		float fEnemySpeed = currentEnemy->Speed;
		
		if( gameObject->Player.x < currentEnemy->x )
			enemyDeltas->x -= fEnemySpeed*fLastFrameTime;	// decrease x 
		else if( gameObject->Player.x > currentEnemy->x )
			enemyDeltas->x += fEnemySpeed*fLastFrameTime;	// increase x 
	
		if( gameObject->Player.z < currentEnemy->z )
			enemyDeltas->y -= fEnemySpeed*fLastFrameTime;	// decrease z 
		else if( gameObject->Player.z > currentEnemy->z )
			enemyDeltas->y += fEnemySpeed*fLastFrameTime;	// increase z 

		// refresh tile coordinates now that we have accumulated the distance walked
		refreshPosFromDeltas( currentEnemy );

		if( gameObject->Player.z == currentEnemy->z 
			&& gameObject->Player.x == currentEnemy->x )
		{	// Decrease player life if enemy position matches player position
			gameObject->Player.CurrentPoints.HP -= 1;
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
};

// This function prints miscelaneous game info
void drawASCIIGameInfo( const SGame* gameObject )
{
	// Calculate another ways of representing the direction
	SVector2 dirVector = SVector2(1, 0).Rotate(gameObject->Player.Direction);
	float degrees = gameObject->Player.Direction/(float)GAME_2PI*360;

	printf( "- Player health: %i\n"
			"- Player position: (%i, %i), deltas: (%f, %f)\n"
			"- Player direction: (%f, %f)\n"
			"- Player angle: %f radians or %f degrees\n"
			"- Enemy count: %i\n"
			"Move (P)layer by pressing the arrow keys to prevent being touched by enemies E, F, G and H.\nRun by holding LEFT SHIFT while moving.", 
			gameObject->Player.CurrentPoints.HP, gameObject->Player.x, gameObject->Player.z, 
			gameObject->Player.PositionDeltas.x, gameObject->Player.PositionDeltas.y, 
			dirVector.x, dirVector.y, gameObject->Player.Direction, degrees,
			gameObject->Enemy.size() );
};

