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

#ifndef WART_STAGE
#define WART_STAGE

/*	The Stage is the window	*/

	struct Stage : Castmember {
		virtual Tag update();
		Stage(Tag);
		~Stage();
		lua_State *lua;
	private:
		SDL_Surface *screen;
		int start;
};

#endif
