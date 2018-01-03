// Tip: Hold Left ALT + SHIFT while tapping or holding the arrow keys in order to select multiple columns and write on them at once. 
//		Also useful for copy & paste operations in which you need to copy a bunch of variable or function names and you can't afford the time of copying them one by one.
//
#include "timer.h"	// for our STimer struct

#include <stdio.h>		// for printf()
#include <windows.h>	// for interacting with Windows

int						main							()													{	// Our application application starts from here
	STimer						timer;

    int							frameCounter					= 0; // declare a variable to keep track of the frame number
    while( true ) { // iterate {} while what's inside () is true
		timer.Frame(); // query time elapsed since last Frame() call

		printf("Current frame number: %i\n" "Last frame time: %f\n", frameCounter, timer.LastTime );

        if(GetAsyncKeyState(VK_ESCAPE)) // check for escape key pressed
            break;	// exit while()
        
		Sleep(50);		// wait some time to give visual stability to the frame
        ++frameCounter;	// increase frame counter
    }
  
    return 0; // return an int
}
