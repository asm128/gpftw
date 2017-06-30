#include <stdio.h>	// <--- This line tells the program to include all the source code found in stdio.h as part of our program, so we have access to standard functions that come with the compiler. 
					//		For this tutorial we need to import from stdio.h, a function to print some text on the screen. 
					//		This is because the language (either this or any other programming language) doesn't define a concept of screen and the responsibility of providing a medium for display is left to the Operating System and the compiler libraries.

/// A function is defined as follows, where the text between braces would be what you would replace with your actual code: 
///
/// [returnType]  [functionName]		([functionParameters])		{ [codeBlock]; }
///
/// i.e.:
///
/// void	myFunction()	{ }
///
/// or:
///
/// int	guessMyName()	{ return 0; }
///
/// or:
///
/// int	beCreative(int argc, char* argv)	{ return 0; }
/// ------------------------------------------------------------------------------------------------------------------
int		main	()	/// The program starts from here.
{
	printf("Hello World!\n");  // Print some text with a function taken from <stdio.h>
	return 0;	// Return an (int)eger number.
}
