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

#include "../wart.h"

/* With the Stage as a Castmember (how poetic/pretentious does that sound?) we can set up constructors
and destructors for the window itself, thereby allowing us to wrap all our lib behaviour up nice and tight
and we don't have to add anything special or unusual for its de/allocation, when we WRAP it's purged like 
everything else and destroyed appropriately.

Its update function throttles the framerate, and flips the screen. */

Stage *stage;

Stage::Stage(string t) : Castmember (t) {

	/*	Set window size	*/
	viewport.w = 800; viewport.h = 600;
	update_viewport(0, 0);

	/*	Drag SDL in	*/
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(viewport.w, viewport.h, 16, SDL_HWSURFACE);
	start = SDL_GetTicks();

	/*	Then Lua	*/
	lua = lua_open();

	/*	Make Stage global without static acrobatics	*/
	stage = this;
}

Stage::~Stage() {
	lua_close(lua);
	SDL_FreeSurface(screen);
	SDL_Quit();
}

#define TICKS 1000
#define JIFFY TICKS/25

Tag Stage::update() {
	if(JIFFY > (SDL_GetTicks() - start)) SDL_Delay(JIFFY - (SDL_GetTicks() - start));
	SDL_Flip(screen);
	start = SDL_GetTicks();
	return LIVE;
}

#undef TICKS
#undef JIFFY

void Stage::update_viewport(int x, int y) {
	viewport.x = x; viewport.y = y;
}
