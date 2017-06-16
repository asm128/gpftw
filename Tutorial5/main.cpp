// Tip: Hold Left ALT + SHIFT while tapping or holding the arrow keys in order to select multiple columns and write on them at once. 
//		Also useful for copy & paste operations in which you need to copy a bunch of variable or function names and you can't afford the time of copying them one by one.
//
#include <stdio.h>		// for printf()
#include <vector>		// for std::vector
#include <windows.h>	// for interacting with Windows

#define		TILE_GRASS			0  // define some values to represent our terrain tiles
#define		TILE_WALL			1
#define		TILE_WATER			2
#define		TILE_LAVA			3

#define		MAX_MAP_WIDTH		256 // define a maximum size to our game map
#define		MAX_MAP_DEPTH		256   

struct SMap {	// The struct is a block of variables to be used to store our map information
    int														Width, Depth; // Declare Width and Depth variables which will hold the active map size
    int														FloorCells[MAX_MAP_DEPTH][MAX_MAP_WIDTH]; // 2-Dimensional array of integers which can be accessed as FloorCells[y][x] and will hold values for representing the terrain
    int														EnemyCells[MAX_MAP_DEPTH][MAX_MAP_WIDTH]; // 2-Dimensional array which holds indices to the enemy list.
};

#define INVALID_ENEMY -1									

struct SCharacterPoints {
	int														HP
		,													MP
		,													XP
		; // health, mana and experience/score
};

struct SCharacter { // holds character data
	SCharacterPoints										MaxPoints;		
	SCharacterPoints										CurrentPoints;	

	int														x, z;	// coordinates in map
};

struct SGame { // holds the game data
    SMap													Map		; // declare a variable of type SMap
	SCharacter												Player	; // Declare a variable of type SCharacter for the player
	std::vector<SCharacter>									Enemy	;	// and another SCharacter instance for the enemy
};

void													setup										( SGame* gameObject ); // Use this function to setup our game data
void													update										( SGame* gameObject ); // Use this function to update our game data
void													draw										( const SGame* gameObject ); // Use this function to draw our game data

int														main										()															{	// The application starts from here
	SGame														gameInstance;
	setup( &gameInstance ); // call setup() and send the address of our data as parameters

	int															frameCounter								= 0; // declare a variable to keep track of the frame number
	while( true ) { // execute block {} while what's inside () is true
		printf("Current frame number: %i\n", frameCounter);
		
		update	( &gameInstance ); // update frame, send game data address to update() call
		draw	( &gameInstance ); // render game data
		
		if(GetAsyncKeyState(VK_ESCAPE)) // check for escape key pressed
			break; // exit while()

		Sleep(100); // wait some time to give visual stability to the frame and to give the game a playable speed.

		frameCounter++;	// increase frame counter
	}
	return 0; // return an int
}

// Use this function to setup enemy list at level startup.
void													setupEnemies								( SGame* gameObject )										{
#define INITIAL_ENEMY_COUNT 4
	for( int iEnemy=0; iEnemy < INITIAL_ENEMY_COUNT; iEnemy++ ) {
		SCharacter													newEnemy;
		newEnemy.MaxPoints										= { 100, 50, 1000000 }; // HP, MP and XP
		newEnemy.CurrentPoints									= { 100, 50, 0 };
		newEnemy.x												= rand() % gameObject->Map.Width;
		newEnemy.z												= rand() % gameObject->Map.Depth;

		gameObject->Enemy.push_back( newEnemy ); // copy the new enemy as a new element at the end of our enemy list.
	}

}

// Use this function to setup our game data
void													setup										( SGame* gameObject )										{	// Accepts an address pointing to an SGame instance
	printf("- setup() called.\n");

	gameObject->Map.Width									= 32; // Set a proper width for our map, which has to be less than MAX_MAP_WIDTH
	gameObject->Map.Depth									= 19; // Same for map depth   

	for( int z = 0; z< gameObject->Map.Depth; z = z+1 )	// iterate over every row
		for( int x=0; x< gameObject->Map.Width; ++x ) {		// iterate over every column for the z row
			gameObject->Map.FloorCells[z][x]						= TILE_GRASS;		// initialize the (x,z) map cell to "grass"
			gameObject->Map.EnemyCells[z][x]						= INVALID_ENEMY;	// initialize the cell to an invalid enemy
		}

	// set a wall border
	for( int x=0; x < gameObject->Map.Width; x++ ) {
		gameObject->Map.FloorCells[0][x]						= TILE_WALL; // set all cells in the first row [0]   
		gameObject->Map.FloorCells[gameObject->Map.Depth-1][x]	= TILE_WALL; // set all cells in the last row [depth-1]
	}
	for( int z=0; z < gameObject->Map.Depth; z++ ) {
		gameObject->Map.FloorCells[z][0]						= TILE_WALL; // set all cells in the first column [0]   
		gameObject->Map.FloorCells[z][gameObject->Map.Width-1]	= TILE_WALL; // set all cells in the last column [width-1]
	}

	// set some initial configuration to the game characters
	gameObject->Player.CurrentPoints.HP						= 100; 
	gameObject->Player.x									= 5;
	gameObject->Player.z									= 5;
	
	setupEnemies(gameObject); // setup enemies in list
}

// Use this function to update our game data
void													update										( SGame* gameObject )										{	// Accepts an address of an SGame instance
	printf("- update() called.\n");
	
	if(GetAsyncKeyState(VK_UP))
		gameObject->Player.z									= gameObject->Player.z - 1; // decrease by 1
	if(GetAsyncKeyState(VK_DOWN))
		gameObject->Player.z									+= 1; // increase by 1
	if(GetAsyncKeyState(VK_RIGHT))
		++gameObject->Player.x;		// increase by 1
	if(GetAsyncKeyState(VK_LEFT))
		gameObject->Player.x--;		// decrease by 1 

	for( int z = 0; z < gameObject->Map.Depth; z++ ) // clear all enemy rows to refresh the enemy map layer
		memset( gameObject->Map.EnemyCells[z], INVALID_ENEMY, sizeof(int)*gameObject->Map.Width );

	for( unsigned int iEnemy=0; iEnemy < gameObject->Enemy.size(); ++iEnemy ) {
		SCharacter													* currentEnemy								= &gameObject->Enemy[iEnemy]; // get the address of the current enemy at [iEnemy] index

		if( gameObject->Player.x < currentEnemy->x )
			currentEnemy->x											-= 1;	// decrease enemy x if player is in that direction
		else if( gameObject->Player.x > currentEnemy->x )
			currentEnemy->x											+= 1;	// increase enemy x if player is in that direction
	
		if( gameObject->Player.z < currentEnemy->z )
			--currentEnemy->z;	// decrease enemy z if player is in that direction
		else if( gameObject->Player.z > currentEnemy->z )
			++currentEnemy->z;	// increase enemy z if player is in that direction

		if( gameObject->Player.z == currentEnemy->z 
		 && gameObject->Player.x == currentEnemy->x 
		 )
		{
			gameObject->Player.CurrentPoints.HP						-= 1; // Decrease player life if enemy position matches player position
			currentEnemy->x											= rand() % gameObject->Map.Width;
			currentEnemy->z											= rand() % gameObject->Map.Depth;	// set a random position for the enemy so it has to walk again in order to hit the player
		}
		gameObject->Map.EnemyCells[currentEnemy->z][currentEnemy->x] = iEnemy; // assign enemy index to the cell corresponding to this enemy
	}
}

// Use this function to draw our game data and to display the results on the console screen
void													draw										( const SGame* gameObject )									{	// Accepts an address of an SGame instance
	printf("- draw() called.\n");
	
	char														imageMap[4096]								= {};	// We're going to draw our map in this array. the assignment of empty brackets = {} initializes all chars in the array to 0
	int															nImageCursor								= 0;	// The position where we shuold position our next character

	for( int z=0; z< gameObject->Map.Depth; z++ ) {				// iterate over every row
		for( int x=0; x< gameObject->Map.Width; x++ ) {				// iterate over every column for the z row
				 if( gameObject->Player.x == x && gameObject->Player.z == z )	imageMap[nImageCursor++]	= 'P'; // draw the player as an ascii character
			else if( gameObject->Map.EnemyCells[z][x] != INVALID_ENEMY )		imageMap[nImageCursor++]	= char('E' + gameObject->Map.EnemyCells[z][x]); // draw the enemy as an ascii character
			else																imageMap[nImageCursor++]	= char(gameObject->Map.FloorCells[z][x] ? gameObject->Map.FloorCells[z][x] : ' '); // draw the tile as an ascii character
		}
		imageMap[nImageCursor++]								= '\n'; // \n is the code character for "new line" inside a text. We use it to display the next cells in the next row.
	}
	imageMap[nImageCursor]									= 0; // a null char indicates the end of the character string or text.

	printf( "%s", imageMap );
	printf( "- Player health: %i\n"
			"- Player position: (%i, %i)\n"
			"- Enemy count: %i\n"
			"Move (P)layer by pressing the arrow keys to prevent being touched by enemies E, F, G and H.\n", 
			gameObject->Player.CurrentPoints.HP, gameObject->Player.x, gameObject->Player.z, gameObject->Enemy.size() );
}
