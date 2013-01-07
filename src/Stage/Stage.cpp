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
	lua = NULL;
	lua = luaL_newstate();
	luaL_openlibs(lua);
	
	/*	Make Stage global without static acrobatics	*/
	stage = this;

	/* Other necessary daemons, that require the Stage to be created first	*/
	new Cue("Cue Daemon");

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

void Stage::stackdump(string str) {
	cout << str << "\t";
	for(int i = 1; i <= lua_gettop(lua); i++) {
		switch(int t = lua_type(lua, i)) {
			case LUA_TSTRING: cout << lua_tostring(lua, i); break;
			case LUA_TNUMBER: cout << lua_tonumber(lua, i); break;
			case LUA_TBOOLEAN: cout << (lua_toboolean(lua, i) ? "true" : "false");
			default: cout << lua_typename(lua, t); break; 
		}
		cout << "\t";
	}
	cout << endl;
}

Cue *cue;

Cue::Cue(string t) : Castmember(t) {
	lua_newtable(stage->lua);
	
	/*	message table within cue	*/
	lua_newtable(stage->lua);
	lua_setfield(stage->lua, -2,"message");
	/*	and finally the persist table	*/
	lua_newtable(stage->lua);
	lua_setfield(stage->lua, -2,"persist");
	lua_setfield(stage->lua, LUA_REGISTRYINDEX, "wart");
	message("test");
	cue = this;
}

Status Cue::update() {
	lua_getfield(stage->lua, LUA_REGISTRYINDEX, "wart");
	lua_newtable(stage->lua);
	lua_setfield(stage->lua, -2, "message");
	lua_settop(stage->lua, 0);

	/*
	lua_pushnil(stage->lua);
	while(lua_next(stage->lua, 1)) {
		if(lua_isstring(stage->lua, -2)) 
			cout << lua_tostring(stage->lua, -2) << endl;
		lua_pop(stage->lua, 1);	
	}
	lua_pop(stage->lua, -1);
	*/

	return LIVE;
}

void Cue::message(string message) {
	lua_getfield(stage->lua, LUA_REGISTRYINDEX, "wart");
	lua_getfield(stage->lua, -1, "message");
	lua_pushboolean(stage->lua, 1);
	lua_setfield(stage->lua, -2, message.c_str());
	lua_getfield(stage->lua, -1, message.c_str());
	stage->stackdump("Message Pushed: ");
	lua_settop(stage->lua, 0);
}
