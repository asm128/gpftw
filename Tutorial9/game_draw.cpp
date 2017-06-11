#include "game.h"
#include <stdio.h>

// This function prints the game map on the console
void																drawASCIIMap						( const SGame* gameObject )															{
	char																	imageMap[4096]						= {};	// We're going to draw our map in this array. the assignment of empty brackets = {} initializes all chars in the array to 0
	int																		nImageCursor						= 0;	// The position where we shuold position our next character

	for( int z=0; z< gameObject->Map.Depth; z++ ) { // iterate over every row
		for( int x=0; x< gameObject->Map.Width; x++ ) { // iterate over every column for the z row
			if( gameObject->Player.x == x && gameObject->Player.z == z )
				imageMap[nImageCursor++]											= 'P'; // draw the player as an ascii character
			else if( gameObject->Map.EnemyCells[z][x] != INVALID_ENEMY ) {
				if( gameObject->Map.ShotsCells[z][x] != INVALID_SHOT )
					imageMap[nImageCursor++]											= '@'; // draw the enemy as an ascii character
				else
					imageMap[nImageCursor++]											= 'E'+gameObject->Map.EnemyCells[z][x]; // draw the enemy as an ascii character
			}
			else if( gameObject->Map.ShotsCells[z][x] != INVALID_SHOT ) {
				const SShot																* shot								= &gameObject->Shots[gameObject->Map.ShotsCells[z][x]];
				SVector2																dirVector							= SVector2(1, 0).Rotate( shot->Direction );

				if( ( dirVector.x < (-GAME_EPSILON) && dirVector.y < (-GAME_EPSILON) )
					|| ( dirVector.x > GAME_EPSILON && dirVector.y > GAME_EPSILON )
					)
					imageMap[nImageCursor++]											= '\\'; // draw the enemy as an ascii character
				else if( ( dirVector.x < (-GAME_EPSILON) && dirVector.y > GAME_EPSILON )
					|| ( dirVector.x > GAME_EPSILON && dirVector.y < (-GAME_EPSILON) )
					)
					imageMap[nImageCursor++]											= '/'; // draw the enemy as an ascii character
				else if( dirVector.x < (-GAME_EPSILON) || dirVector.x > GAME_EPSILON )
					imageMap[nImageCursor++]											= '-'; // draw the enemy as an ascii character
				else 
					imageMap[nImageCursor++]											= '|'; // draw the enemy as an ascii character
			}
			else
				imageMap[nImageCursor++]											= gameObject->Map.FloorCells[z][x] ? gameObject->Map.FloorCells[z][x] : ' '; // draw the tile as an ascii character
        }
        imageMap[nImageCursor++]											= '\n'; // \n is the code character for "new line" inside a text. We use it to display the next cells in the next row.
    }
    imageMap[nImageCursor]												= 0; // a null char indicates the end of the character string or text.

	printf( "%s", imageMap );
}

// This function prints miscelaneous game info
void																drawASCIIGameInfo					( const SGame* gameObject )															{
	// Calculate another ways of representing the direction
	SVector2																dirVector							= SVector2(1, 0).Rotate(gameObject->Player.Direction);
	float																	degrees								= gameObject->Player.Direction/(float)GAME_2PI*360;

	printf( "- Player health: %i, Mana: %i, Experience: %i\n"
			"- Player position: (%i, %i), deltas: (%f, %f)\n"
			"- Player direction: (%f, %f)\n- Player angle: %f radians or %f degrees\n"
			"- Enemy count: %i, Shot count: %i\n"
			"Shoot with Space key. Run by holding LEFT SHIFT while moving.\nMove (P)layer by pressing the arrow keys to prevent being touched by enemies E, F, G and H."
		, gameObject->Player.CurrentPoints.HP
		, gameObject->Player.CurrentPoints.MP
		, gameObject->Player.CurrentPoints.XP
		, gameObject->Player.x
		, gameObject->Player.z
		, gameObject->Player.PositionDeltas.x
		, gameObject->Player.PositionDeltas.y
		, dirVector.x
		, dirVector.y
		, gameObject->Player.Direction
		, degrees
		, gameObject->Enemy.size()
		, gameObject->Shots.size() 
	);
};