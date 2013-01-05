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

#include "wart.h"

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

Stage::Stage(Tag t) : Castmember (t) {
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
	start = SDL_GetTicks();
	lua = lua_open();
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

enum {
	MYKE,
	VIX,
	TINNY,
	GUYBRUSH
};

lua_State *L;

Stage *stage;

int main(int argc, char *argv[]) {
	L = lua_open();
	//if(luaL_loadfile(L, "Scenes/test.lua") || lua_pcall(L, 0, 0, 0)) std::cout << "cannot run config file: " << lua_tostring(L, -1) << std::endl;
	//lua_getglobal(L, "name");
	//lua_getglobal(L, "age");
	//std::cout << "Hello they call me, "  << lua_tostring(L, -2) << " and I'm " << lua_tostring(L, -1) << " years old." << std::endl;
	//lua_close(L);
	new Player(GUYBRUSH);
	new Stage(MYKE);
	while(Cast::perform()) continue;
	return 0;
}
