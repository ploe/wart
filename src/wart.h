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

#endif
