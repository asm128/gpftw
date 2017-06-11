// Tip: Hold Left ALT + SHIFT while tapping or holding the arrow keys in order to select multiple columns and write on them at once. 
//		Also useful for copy & paste operations in which you need to copy a bunch of variable or function names and you can't afford the time of copying them one by one.
//
#include <stdio.h>      // for printf()
#include <windows.h>    // for interacting with Windows

// Define some functions to use from main(). These functions will contain our game code.
void						setup								()				{ printf("- setup() called.\n"	); }
void						update								()				{ printf("- update() called.\n"	); }
void						draw								()				{ printf("- draw() called.\n"	); }

int							main								()				{
	setup(); // Call setup()
	int								frameCounter						= 0; // declare and initialize a variable of (int)eger type for keeping track of the number of frame since execution began.
	
	while( true ) { // Execute code between braces while the condition inside () evaluates to true.
		printf("Current frame number: %i\n", frameCounter); // Print the frame count.

		update	();		// Update frame.
		draw	();		// Render frame.
		if(GetAsyncKeyState(VK_ESCAPE)) // Check for escape key pressed.
		{
		    break; // Exit while() loop.
		}
		Sleep(50);	// Wait 50 milliseconds then continue executing
		frameCounter				= frameCounter + 1; // increase our frame counter by 1
	}
	return 0; // Exit from the function returning an (int)eger.
}
