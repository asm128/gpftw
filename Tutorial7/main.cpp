// Tip: Hold Left ALT + SHIFT while tapping or holding the arrow keys in order to select multiple columns and write on them at once. 
//		Also useful for copy & paste operations in which you need to copy a bunch of variable or function names and you can't afford the time of copying them one by one.
//
#include "game.h"	// for our game functions
#include "timer.h"	// for our STimer struct

#include <stdio.h>		// for printf()
#include <windows.h>	// for interacting with Windows

void														setup								( SGame* gameObject ); // Use this function to initialize our game data
void														update								( SGame* gameObject, float fLastFrameTime ); // Use this function to update our game data
void														draw								( const SGame* gameObject ); // Use this function to draw our game data

int															main								()															{		// start application from here
	SGame															gameInstance;
	STimer															timer;

	setup( &gameInstance ); // call setup() and send the address of our data as parameters
	
	int																frameCounter						= 0; // declare a variable to keep track of the frame number
	while( true ) { // iterate {} while what's inside () is true
		timer.Frame(); // query time elapsed since last Frame() call

		printf("Current frame number: %i\n" "Last frame time: %f\n", frameCounter, timer.LastTime );
		update	( &gameInstance, timer.LastTime ); // send frame time to update() 
		draw	( &gameInstance ); // render game data
		
		if(GetAsyncKeyState(VK_ESCAPE)) // check for escape key pressed
			break; // exit while()
		
		Sleep(60); // wait some time to give visual stability to the frame
		++frameCounter;	// increase frame counter
	}
	
	return 0; // return an int
}

// Use this function to setup our game data
void														setup								( SGame* gameObject )										{		// Accepts an address pointing to an SGame instance
	printf("- setup() called.\n");

	// call setup game functions
	setupMap			(gameObject);
	setupPlayer			(gameObject);
	setupEnemies		(gameObject); // setup enemies in list
}

// Use this function to update our game data
void														update								( SGame* gameObject, float fLastFrameTime )					{		// Accepts an address of an SGame instance
    printf("- update() called.\n");
	
	// call update game functions
	updateMap			(gameObject, fLastFrameTime);
	updatePlayer		(gameObject, fLastFrameTime);
	updateEnemies		(gameObject, fLastFrameTime); // update enemies
}

// Use this function to draw our game data
void														draw								( const SGame* gameObject )									{		// Accepts an address of an SGame instance
	printf("- draw() called.\n");

	drawASCIIMap		(gameObject);
	drawASCIIGameInfo	(gameObject);
}
