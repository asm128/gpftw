#include "game.h"

void																setupMap							( SGame* gameObject ) 																{	// initialize game map
	gameObject->Map.Width												= 32; // Set a proper width for our map, which has to be less than MAX_MAP_WIDTH
	gameObject->Map.Depth												= 17; // Same for map depth   

	for( int z=0; z< gameObject->Map.Depth; ++z ) {		// iterate over every row
		for( int x=0; x< gameObject->Map.Width; ++x ) {		// iterate over every column for the z row
			gameObject->Map.FloorCells[z][x]									= TILE_GRASS;	
			gameObject->Map.EnemyCells[z][x]									= INVALID_ENEMY;
			gameObject->Map.ShotsCells[z][x]									= INVALID_SHOT;
		}
	}

	// set a wall border
	for( int x=0; x < gameObject->Map.Width; x++ ) {
		gameObject->Map.FloorCells[0][x]									= TILE_WALL; // set all cells in the first row [0]   
		gameObject->Map.FloorCells[gameObject->Map.Depth-1][x]				= TILE_WALL; // set all cells in the last row [depth-1]
	}
	for( int z=0; z < gameObject->Map.Depth; z++ ) {
		gameObject->Map.FloorCells[z][0] = TILE_WALL; // set all cells for the first column [0]   
		gameObject->Map.FloorCells[z][gameObject->Map.Width-1]				= TILE_WALL; // set all cells for the last column [width-1]
	}
};

// Use this function to setup player at level startup.
void																setupPlayer							( SGame* gameObject )																{
	// set some initial configuration to the player character
	gameObject->Player.CurrentPoints									= { 100, 20, 0 }; 

	gameObject->Player.x												= 5;
	gameObject->Player.z												= 5;
	gameObject->Player.PositionDeltas									= {0,0};
	gameObject->Player.Direction										= 0.0f;
	gameObject->Player.Speed											= 5.5f;
	gameObject->Player.Action											= ACTION_STAND;
}

// Use this function to setup enemy list at level startup.
void																setupEnemies						( SGame* gameObject )																{
#define INITIAL_ENEMY_COUNT 5
	for( int iEnemy=0; iEnemy < INITIAL_ENEMY_COUNT; iEnemy++ ) {
		SCharacter																newEnemy;
		newEnemy.MaxPoints													= { 10+10*iEnemy, 50, 1000000 }; // HP, MP and XP
		newEnemy.CurrentPoints												= { 10+10*iEnemy, 50, 0 };
		newEnemy.x															= rand() % gameObject->Map.Width;
		newEnemy.z															= rand() % gameObject->Map.Depth;
		newEnemy.PositionDeltas												= {0, 0};
		newEnemy.Direction													= 0.0f;
		newEnemy.Speed														= float(iEnemy); // generate the speed from the enemy index
		newEnemy.Action														= ACTION_WALK;

		gameObject->Enemy.push_back( newEnemy ); // copy the new enemy as a new element at the end of our enemy list.
	}
}
