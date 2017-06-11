#include "point.h"		// for SVector2
#include <vector>		// for std::vector

#define MAX_MAP_WIDTH    256 // define a maximum size to our game map
#define MAX_MAP_DEPTH    256   

#define TILE_GRASS   0  // define some values to represent our terrain tiles
#define TILE_WALL    1
#define TILE_WATER   2
#define TILE_LAVA    3

#define INVALID_ENEMY -1
#define INVALID_SHOT -1

#define GAME_PI 3.141592653589793238462643383279
#define GAME_2PI (2*GAME_PI)
#define GAME_EPSILON 0.00001f

struct SMap // The struct is a block of variables to be used to store our map information
{
    int												Width
		,											Depth
		; // Declare Width and Depth variables which will hold the active map size
    int												FloorCells[MAX_MAP_DEPTH][MAX_MAP_WIDTH]; // 2-Dimensional array of integers which can be accessed as FloorCells[y][x] and will hold values for representing the terrain
    int												EnemyCells[MAX_MAP_DEPTH][MAX_MAP_WIDTH]; // 2-Dimensional array which holds indices to the enemy list.
    int												ShotsCells[MAX_MAP_DEPTH][MAX_MAP_WIDTH]; // 2-Dimensional array which holds indices to the shot list.
};

struct SCharacterPoints
{
	int												HP	// health, mana and experience/score
		,											MP
		,											XP
		;	
};

struct SMovingObject {
	int												x	// Coordinates in tile map
		,											z
		;												
	SVector2										PositionDeltas;	// The position inside the tile
	float											Direction;		// rotation angle in radians
	float											Speed;			// cells/second
};

enum ACTION_TYPE	// this is pretty much the same as "typedef int ACTION_TYPE;"
	{	ACTION_WALK	// means the character has to apply movement during update
	,	ACTION_STAND	// means the character is stopped and shouldn't walk on frame update
	};

// holds character data, inherits from SMovingObject for storing position and physics information
struct SCharacter : public SMovingObject {
	SCharacterPoints								MaxPoints;	// the maximum game points that can be set to a particular character
	SCharacterPoints								CurrentPoints;	// the current value of the character points, updated on game events
	ACTION_TYPE										Action;
};

// holds character data, inherits from SMovingObject for storing position and physics information
struct SShot : public SMovingObject {
	int												Damage;	// Value that will be substracted from the character's health points 
};

struct SGame { // holds the game data
    SMap											Map; // declare a variable of type SMap
	SCharacter										Player; // Declare a variable of type SCharacter for the player
	std::vector<SCharacter>							Enemy;	// Enemy list
	std::vector<SShot>								Shots;		// Shot list
};

// -- game functions			
void											setupMap					( SGame* gameObject );	// initialize map layers
void											setupPlayer					( SGame* gameObject );	// initialize player
void											setupEnemies				( SGame* gameObject );	// initialize enemy list

void											updateMap					( SGame* gameObject, float fTimeElapsed );	// update tiles
void											updatePlayer				( SGame* gameObject, float fTimeElapsed );	// update player
void											updateShots					( SGame* gameObject, float fTimeElapsed );	// update shots
void											updateEnemies				( SGame* gameObject, float fTimeElapsed );	// update enemy AI and refresh enemy layer

void											drawASCIIMap				( const SGame* gameObject );	// take the map data and print it on the console
void											drawASCIIGameInfo			( const SGame* gameObject );	// print the player and misc info
