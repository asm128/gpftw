#include "point.h"		// for SVector2
#include <vector>		// for std::vector

#define MAX_MAP_WIDTH		48 // define a maximum size to our game map
#define MAX_MAP_DEPTH		48   

#define TILE_GRASS			0  // define some values to represent our terrain tiles
#define TILE_WALL			1
#define TILE_WATER			2
#define TILE_LAVA			3

#define INVALID_ENEMY		-1

#define GAME_PI				3.141592653589793238462643383279
#define GAME_2PI			(2*GAME_PI)
#define GAME_EPSILON		0.00001f

struct SMap { // The struct is a block of variables to be used to store our map information
	int									Width
		,								Depth
		; // Declare Width and Depth variables which will hold the active map size
	short								FloorCells[MAX_MAP_DEPTH][MAX_MAP_WIDTH]; // 2-Dimensional array of integers which can be accessed as FloorCells[y][x] and will hold values for representing the terrain
	short								EnemyCells[MAX_MAP_DEPTH][MAX_MAP_WIDTH]; // 2-Dimensional array which holds indices to the enemy list.
};

struct SCharacterPoints {
	int									HP
		,								MP
		,								XP
		; // health, mana and experience/score
};

struct SCharacter { // holds character data
	SCharacterPoints					MaxPoints		;
	SCharacterPoints					CurrentPoints	;

	int									x	// Coordinates in tile map
		,								z
		; 
	SVector2							PositionDeltas	;
	float								Direction		;	// rotation angle in radians
	float								Speed			;	// cells/second
};

struct SGame { // holds the game data
    SMap								Map; // declare a variable of type SMap
	SCharacter							Player; // Declare a variable of type SCharacter for the player
	::std::vector<SCharacter>			Enemy;	// and another SCharacter instance for the enemy
};

// -- game functions
void								setupMap								( SGame* gameObject );	// initialize map layers
void								setupPlayer								( SGame* gameObject );	// initialize player
void								setupEnemies							( SGame* gameObject );	// initialize enemy list

void								updateMap								( SGame* gameObject, float fTimeElapsed );	// update tiles
void								updatePlayer							( SGame* gameObject, float fTimeElapsed );	// update player
void								updateEnemies							( SGame* gameObject, float fTimeElapsed );	// update enemy AI and refresh enemy layer

void								drawASCIIMap							( const SGame* gameObject );	// take the map data and print it on the console
void								drawASCIIGameInfo						( const SGame* gameObject );	// print the player and misc info
