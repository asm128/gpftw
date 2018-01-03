// Tip: Hold Left ALT + SHIFT while tapping or holding the arrow keys in order to select multiple columns and write on them at once. 
//		Also useful for copy & paste operations in which you need to copy a bunch of variable or function names and you can't afford the time of copying them one by one.
//
#include <stdio.h>      // for printf()
#include <windows.h>    // for Sleep() and GetAsyncKeyState()

#define TILE_GRASS   0  // define some values to represent our terrain tiles
#define TILE_WALL    1
#define TILE_WATER   2
#define TILE_LAVA    3

#define MAX_MAP_WIDTH    64 // define a maximum size to our game map
#define MAX_MAP_DEPTH    64  

struct SMap // The struct is a block of variables to be used to store our map information
{
	int												Width; // Declare Width and Depth variables which will hold the active map size
	short int										Depth;  
	short int										FloorCells[MAX_MAP_DEPTH][MAX_MAP_WIDTH]; // 2-Dimensional array of integers which can be accessed as FloorCells[y][x] and will hold values for representing the terrain
};

struct SCharacter { // holds character data	
	int												HP;		// health points
	int												x, z;	// coordinates in map
};

void											setup										(SMap* activeMap	, SCharacter*	player, SCharacter*	enemy);	// Accepts addresses pointing to SMap and SCharacter data
void											update										(SMap* activeMap	, SCharacter*	player, SCharacter*	enemy);	// We can now modify our player and enemy data from these functions
void											draw										(SMap someMap		, SCharacter	player, SCharacter	enemy);	// Accepts a copy of the data found in the game structures.

int												main										()																	{	// Application starts from here
	SMap												gameMap;			// declare a variable of type SMap. This is almost the same as defining each variable of SMap but shorter, and we can acess to the members with a dot, as in gameMap.Width or gameMap.FloorCells[z][x]
	SCharacter											playerCharacter;	// Declare a variable of type SCharacter for the player
	SCharacter											enemyCharacter;		// and another SCharacter instance for the enemy

	setup( &gameMap, &playerCharacter, &enemyCharacter );				// call setup() and send the address of our instances as parameters

	int													frameCounter								= 0; // declare a variable to keep track of the frame number

	while( true ) { // execute block {} while what's inside () is true
		printf	("Current frame number: %i\n", frameCounter);
		
		update	(&gameMap, &playerCharacter, &enemyCharacter);	// update frame, send map and player addresses to update() call
		draw	(gameMap, playerCharacter, enemyCharacter);		// render frame, send copy of the data to be displayed by draw()
		
		if(GetAsyncKeyState(VK_ESCAPE)) // check for escape key pressed
			break; // exit while() loop
		
		Sleep(150); // wait some time to give visual stability to the frame

		frameCounter++; // Increment frame count
	} // while() block ends here

	return 0; // return an (int)eger
}

// Accepts an address pointing to an SMap
void											setup										( SMap* activeMap, SCharacter* player, SCharacter* enemy )			{ 
	printf("- setup() called.\n");

	activeMap->Width								= 32; // Set a proper width for our map, which has to be less than MAX_MAP_WIDTH
	activeMap->Depth								= 20; // Same for map depth  

	for( int z = 0; z < activeMap->Depth; ++z )		// iterate over every row
		for( int x = 0; x < activeMap->Width; ++x )		// iterate over every column for the z row
			activeMap->FloorCells[z][x]						= TILE_GRASS;	// initialize the (x,z) map cell to "grass"

	// set a wall border
	for( int x = 0; x < activeMap->Width; ++x )  {
		activeMap->FloorCells[0][x]						= TILE_WALL; // set cell in the first row [0]  
		activeMap->FloorCells[activeMap->Depth-1][x]	= TILE_WALL; // set cell in the last row [depth-1]
	}
	for( int z=0; z < activeMap->Depth; ++z ) {
		activeMap->FloorCells[z][0]						= TILE_WALL; // set cell in the first column [0]  
		activeMap->FloorCells[z][activeMap->Width-1]	= TILE_WALL; // set cell in the last column [width-1]
	}

    // set some initial configuration to the game characters
	player->HP										= 100; 
	enemy->HP										= 100; 

    player->x										= 5;
	player->z										= 5;
    enemy->x										= 25;
	enemy->z										= 15;
}

void											update										( SMap* activeMap, SCharacter* player, SCharacter* enemy )			{	// Accepts an address pointing to an SMap
    printf("- update() called.\n");

	if( player->x < enemy->x )
		enemy->x										= enemy->x - 1; // decrease enemy x if player in that direction
	else if( player->x > enemy->x )
		enemy->x										= enemy->x + 1; // increase enemy x if player in that direction

	if( player->z < enemy->z )
		enemy->z										= enemy->z - 1; // decrease enemy z if player in that direction
	else if( player->z > enemy->z )
		enemy->z										= enemy->z + 1; // increase enemy z if player in that direction

	if( player->z == enemy->z
	 && player->x == enemy->x 
	 )
	{
		player->HP										-= 1; // Decrease player life if enemy position matches player position
		
		// set a random position for the enemy so it has to walk again to reach the player
		enemy->x										= rand() % activeMap->Width;
		enemy->z										= rand() % activeMap->Depth; 
	}
}

void											draw										( SMap someMap, SCharacter player, SCharacter enemy ) { // Accepts a copy of the contents of an SMap
	printf("- draw() called.\n");
	for( int z = 0; z < someMap.Depth; ++z ) { // iterate over every row
		for( int x = 0; x < someMap.Width; ++x ) { // iterate over every column for the z row
			if( player.x == x && player.z == z )
				printf( "%c", 'P' ); // draw the contents of the cell at (x,z) as an ascii character
			else if( enemy.x == x && enemy.z == z )
				printf( "%c", 'E' ); // draw the contents of the cell at (x,z) as an ascii character
			else
				printf( "%c", someMap.FloorCells[z][x] ); // draw the contents of the cell at (x,z) as an ascii character
		}
		printf( "\n" ); // \n is the code character for "new line" inside a text. We use it to display the next cells in the next row.
	}
	printf("- Enemy health: %i\n"			, enemy		.HP);
	printf("- Player health: %i\n"			, player	.HP);
	printf("- Enemy position: (%i, %i)\n"	, enemy		.x, enemy	.z);
	printf("- Player position: (%i, %i)\n"	, player	.x, player	.z);
}
