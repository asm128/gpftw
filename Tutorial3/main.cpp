// Tip: Hold Left ALT + SHIFT while tapping or holding the arrow keys in order to select multiple columns and write on them at once. 
//		Also useful for copy & paste operations in which you need to copy a bunch of variable or function names and you can't afford the time of copying them one by one.
//
#include <stdio.h>      // for printf()
#include <windows.h>    // for interacting with Windows

#define TILE_GRASS		0	// define some values to represent our terrain tiles. #define is a preprocessor directive that allows to replace the defined identifier with a constant value or text. 
#define TILE_WALL		1	// We use it to put some name to the values we're going to use to represent the map tiles.
#define TILE_WATER		2
#define TILE_LAVA		3

#define MAX_MAP_WIDTH	256 // define some constant values for setting a maximum size to our game map
#define MAX_MAP_DEPTH	256   

struct SMap // The struct is a block of variables to be used to store our map information. 
			// We can create instances of a structure whenever we need such variables instead of declaring each variable every time
{	// Here we begin the block of the struct definition.
	int														Width; // Declare Width and Depth variables which will hold the active map size
	int														Depth;   
	int														FloorCells[MAX_MAP_DEPTH][MAX_MAP_WIDTH]; // 2-Dimensional array of integers which can be accessed as FloorCells[y][x] and will hold values for representing the terrain
};	// struct definitions need to end with a semicolon as with any other statement. This is because declaring a struct can be part of a larger statement.

// ------ We need to declare the functions here so we can use it from main(). However we define then later because we will eventually move them to another file where main() won't have access to.
void													setup											( SMap* activeMap );	// Accepts as parameter an address pointing to an SMap
void													update											( SMap* activeMap );	// Accepts as parameter an address pointing to an SMap
void													draw											( SMap someMap );		// Accepts as parameter a copy of the contents of an SMap

int														main											()						{
	// The program starts from here
	SMap														gameMap;      // declare a variable of type SMap. This is almost the same as defining each variable of SMap but shorter, and we can acess to the members with a dot, as in gameMap.Width or gameMap.FloorCells[z][x]

	setup( &gameMap ); // call setup() and send the address of our gameMap as parameter

	int															frameCounter									= 0; // declare a variable for keeping track of the number of frame since execution began, where "int" stands for "integer"

	while( true ) {	// execute code block {} while the condition that's inside parenthesis () is true
		printf("Current frame number: %i\n", frameCounter);

		update	( &gameMap );	// update frame, send map address to update() call
		draw	( gameMap );	// render frame, send copy of the data to be displayed by draw()

		if(GetAsyncKeyState(VK_ESCAPE)) // check for escape key pressed
			break;	// exit while(). If the code that executes when the if() evaluates to true doesn't have braces, then the block that executes when the condition evaluates to true ends in the next semicolon (;). The same applies for "for()" and "while()".

		Sleep(50);	// Wait some time to give visual stability to the frame (printf() is really ugly for displaying realtime games but for now it will do fine enough).
		
		frameCounter											= frameCounter + 1;	// Increment our frame counter
	}
  
    return 0; // return an int
}

void													setup											( SMap* activeMap )		{ // Accepts an address pointing to an SMap
	printf("- setup() called.\n");
	
	activeMap->Width										= 32; // Set a proper width for our map, which has to be less than MAX_MAP_WIDTH
	activeMap->Depth										= 21; // Same for map depth   
	
	// The "for" statement allows us to iterate over the indices or cells of our map. The syntax of a for() statement is something as follows:	
	//
	//	for( [initial statement]; [conditionToContinueIterating]; [statement to execute after each iteration]) 
	//	{ 
	//		[your code block]; 
	//	}

	for( int z = 0; z < activeMap->Depth; z = z+1 ) // iterate over every row. This loop ends when z is equal to the map depth. At the end of each iteration, z is incremented by one.
	{
		for( int x = 0; x < activeMap->Width; x++ ) // iterate over every column for the x row. This loop ends when x is equal to the map width
		{
			activeMap->FloorCells[z][x]								= TILE_GRASS; // initialize the (x,z) map cell to "grass"
		}
	}

	// set a wall border
	for( int x=0; x< activeMap->Width; x++ )   
		activeMap->FloorCells[0][x]								= TILE_WALL; // set all cells in the first row [0]   
	
	for( int x=0; x< activeMap->Width; x++ )       
		activeMap->FloorCells[activeMap->Depth-1][x]			= TILE_WALL; // set all cells in the last row [depth-1]   
	
	for( int z=0; z< activeMap->Depth; z++ )   
		activeMap->FloorCells[z][0]								= TILE_WALL; // set all cells in the first column [0]   
	
	for( int z=0; z< activeMap->Depth; z++ )   
		activeMap->FloorCells[z][activeMap->Width-1]			= TILE_WALL; // set all cells in the last column [width-1]   
}

// Currently our update() function still does nothing.
void													update											( SMap* activeMap )		{ (void)activeMap; printf("- update() called.\n"); }	// Accepts an address pointing to an SMap

// The draw() function will display our generated map in the console screen.
void													draw											( SMap someMap ) // Accepts a copy of the contents of an SMap
{
    printf("- draw() called.\n");
    for( int z=0; z< someMap.Depth; z++ ) // iterate over every row
    {
        for( int x=0; x< someMap.Width; x++ ) // iterate over every column for the z row
        {
            printf( "%c", someMap.FloorCells[z][x] ); // draw the contents of the cell at (x,z) as an ascii character
        }

        printf( "\n" ); // \n is the code character for "new line" inside a text. We use it to display the next cells in the next row of the console screen.
    }
};
