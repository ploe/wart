/*

wart - Copyright (c) Myke Atkinson 2012

This software is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely. An acknowledgment in derived products or their documentation would be 
appreciated, but is not required.

*/

#ifndef WART
#define WART

	/*	Libraries, standard or otherwise - for when I couldn't be arsed
		reinventing the wheel.	*/

	#include <cstdlib>
	#include <iostream>

	#include "SDL/SDL.h"
	#include "lua.hpp"
	
	/*	The modules that compose wart. Like lime it follows a 
		theatre/film analogy.
		
		Cast 	The events to be called each frame
		Stage	The window/screen, the visual/aural output
		Player	Mouse, keyboard and joypad i.e. input */

	#include "Cast/Cast.h"
	#include "Player/Player.h"

#endif
