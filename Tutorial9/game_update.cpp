#include "game.h"

#include <Windows.h>

void																refreshPosFromDeltas			( SCharacter* character )								{
	SVector2																*charDeltas						= &character->PositionDeltas; // get pointer to deltas
	
	// Now we increase cell units and decrease deltas until the deltas are between 0 and 0.9999999999999999999999
	// Update X coordinate
	int																		deltaX							= (int)charDeltas->x;
	character->x														+= deltaX;
	charDeltas->x														-= deltaX;
	if(charDeltas->x < 0) {
		character->x														-= 1;
		charDeltas->x														+= 1;
	}
	
	// Update Y coordinate
	int																		deltaY							= (int)charDeltas->y;
	character	->z														+= deltaY;
	charDeltas	->y														-= deltaY;
	if(charDeltas->y < 0) {
		character	->z														-= 1;
		charDeltas	->y														+= 1;
	}
}

void																shoot								( SGame* gameObject, SPoint2 origin, float direction )								{
	SShot																	newShot;
	newShot.Damage														= 10; // 10 HP
	newShot.Speed														= 10.0f; // 10 tiles per second
	newShot.PositionDeltas												= origin;
	newShot.Direction													= direction;
	newShot.x															= 
	newShot.z															= 0;
	refreshPosFromDeltas( &newShot ); // update tile coordinates

	gameObject->Shots.push_back( newShot );
}

// Use this function to update the map tiles
void																updateMap							( SGame* /*gameObject*/, float /*fLastFrameTime*/ )									{}

// Use this function to update the player
void																updatePlayer						( SGame* gameObject, float fLastFrameTime  )										{
	// Query key states
	bool																	_keyUP								= GetAsyncKeyState('W') ? true : false,
																			_keyDOWN							= GetAsyncKeyState('S') ? true : false,
																			_keyLEFT							= GetAsyncKeyState('A') ? true : false,
																			_keyRIGHT							= GetAsyncKeyState('D') ? true : false;

	SVector2																dir									= {0,0};
	// set target direction from keys pressed
	if(_keyRIGHT)
		++dir.x; //dir.x = 1;
	if(_keyLEFT)
		--dir.x; //dir.x = -1;
	if(_keyUP)
		--dir.y; //dir.y = -1;
	if(_keyDOWN)
		++dir.y; //dir.y = 1;

	dir.Normalize();	// normalize the new direction vector (make it a unit length so we can multiply it by speed to get velocity vector).

	bool																	noKeysPressed						= !_keyRIGHT && !_keyLEFT && !_keyUP && !_keyDOWN;
	if( noKeysPressed )
		gameObject->Player.Action										= ACTION_STAND;
	else {																
		gameObject->Player.Action										= ACTION_WALK;
		gameObject->Player.Direction									= SVector2(1,0).AngleWith(dir);
		if( dir.y < 0 )
			gameObject->Player.Direction									*= -1; 
	}

	// Increase 50% speed if left shift pressed.
	float																	fSpeed								= GetAsyncKeyState(VK_LSHIFT) ? gameObject->Player.Speed*1.5f : gameObject->Player.Speed;

	if( gameObject->Player.Action == ACTION_WALK ) {
		dir.Scale(fSpeed*fLastFrameTime);
		// integrate our calculated displacement
		gameObject->Player.PositionDeltas									+= dir;
	}

	// refresh tile coords now that we have accumulated the distance walked this frame
	refreshPosFromDeltas( &gameObject->Player );

	if(GetAsyncKeyState(VK_SPACE)) {
		SVector2																origin								= { gameObject->Player.x+gameObject->Player.PositionDeltas.x, gameObject->Player.z+gameObject->Player.PositionDeltas.y };
		shoot( gameObject, origin, gameObject->Player.Direction );
	}
}

// Use this function to update the enemies
void																updateEnemies						( SGame* gameObject, float fLastFrameTime )											{
	for( int z = 0; z < gameObject->Map.Depth; z++ ) // clear all enemy rows to refresh the enemy map layer
		memset( gameObject->Map.EnemyCells[z], INVALID_ENEMY, sizeof(int)*gameObject->Map.Width );

	// An iterator is required for std::vector<SCharacter>::erase().
	std::vector<SCharacter>::iterator										iEnemy								= gameObject->Enemy.begin(); // 
	short																	indexEnemy							= 0; // keep track of enemy index
	while( iEnemy != gameObject->Enemy.end() ) {
		SCharacter																* currentEnemy						= &gameObject->Enemy[indexEnemy]; // get the address of the current enemy at [iEnemy] index
		SVector2																* enemyDeltas						= &currentEnemy->PositionDeltas;

		if( currentEnemy->CurrentPoints.HP <= 0 ) {		// remove enemy if zero health
			gameObject->Player.CurrentPoints.XP									+= iEnemy->MaxPoints.HP;
			iEnemy																= gameObject->Enemy.erase( iEnemy );	// 
			continue; // keep at the current index of the list 
		}

		float																	fEnemySpeed							= currentEnemy->Speed;
		
		if( gameObject->Player.x < currentEnemy->x )
			enemyDeltas->x														-= fEnemySpeed*fLastFrameTime;	// decrease x 
		else if( gameObject->Player.x > currentEnemy->x )
			enemyDeltas->x														+= fEnemySpeed*fLastFrameTime;	// increase x 
	
		if( gameObject->Player.z < currentEnemy->z )
			enemyDeltas->y														-= fEnemySpeed*fLastFrameTime;	// decrease z 
		else if( gameObject->Player.z > currentEnemy->z )
			enemyDeltas->y														+= fEnemySpeed*fLastFrameTime;	// increase z 

		// refresh tile coordinates now that we have accumulated the distance walked
		refreshPosFromDeltas( currentEnemy );

		if( gameObject->Player.z == currentEnemy->z 
		 && gameObject->Player.x == currentEnemy->x 
		 ) { // Decrease player life if enemy position matches player position
			gameObject->Player.CurrentPoints.HP									-= max( 1, currentEnemy->CurrentPoints.HP/3 );
			currentEnemy->x														= rand()%gameObject->Map.Width;
			currentEnemy->z														= rand()%gameObject->Map.Depth;	// set a random position for the enemy so it has to walk again in order to hit the player
		}
		gameObject->Map.EnemyCells[currentEnemy->z][currentEnemy->x]		= indexEnemy; // assign enemy index to the cell corresponding to this enemy
		++indexEnemy;
		++iEnemy;
	}
}

void																updateShots							( SGame* gameObject, float fLastFrameTime  )										{
	for( int z = 0; z < gameObject->Map.Depth; z++ ) // clear all shot rows to refresh the map layer	
		memset( gameObject->Map.ShotsCells[z], INVALID_SHOT, sizeof(int)*gameObject->Map.Width );

	std::vector<SShot>::iterator											iShot								= gameObject->Shots.begin(); // 
	short 																	indexShot							= 0;
	while( iShot != gameObject->Shots.end() ) {
		SShot																	* currentShot						= &gameObject->Shots[indexShot]; 
		SVector2																* shotDeltas						= &currentShot->PositionDeltas;

		SVector2																dirVector							= SVector2(1, 0).Rotate( currentShot->Direction );
		shotDeltas->x														+= currentShot->Speed*fLastFrameTime*dirVector.x; // add speed*time*direction to our coordinates 
		shotDeltas->y														+= currentShot->Speed*fLastFrameTime*dirVector.y; // add speed*time*direction to our coordinates 

		refreshPosFromDeltas( currentShot );	// refresh cell coordinates now that we have accumulated the distance

		if( currentShot->x < 0 
		 || currentShot->z < 0 
		 || currentShot->x >= gameObject->Map.Width 
		 || currentShot->z >= gameObject->Map.Depth
		 ) { // remove shot if reached the end of the screen
			iShot																= gameObject->Shots.erase( iShot );
			continue;
		}
		//
		if( gameObject->Map.EnemyCells[currentShot->z][currentShot->x] != INVALID_ENEMY ) { // damage enemy and remove shot
			int																		enemyIndex							= gameObject->Map.EnemyCells[currentShot->z][currentShot->x];
			SCharacterPoints														& currentEnemyPoints				= gameObject->Enemy[enemyIndex].CurrentPoints;	// get a reference in order to reduce verbosity of our code
			currentEnemyPoints.HP												-= iShot->Damage; // Decrease player life if enemy position matches player position
			gameObject->Player.CurrentPoints.XP									+= iShot->Damage;
			iShot																= gameObject->Shots.erase( iShot ); 
			continue;
		}
		gameObject->Map.ShotsCells[currentShot->z][currentShot->x]			= indexShot; // assign enemy index to the cell corresponding to this enemy
		++iShot;
		++indexShot;
	}
}
