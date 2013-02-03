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
lua_State *L;

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
	L = lua;
	
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
	lua_newtable(L);
	lua_setfield(L, LUA_REGISTRYINDEX, "wart");
	message("buttons/up/", "PRESSED");
	message("one/", "message in the table one");
	cout << read("buttons/up/")  << endl;
	cue = this;
}

Status Cue::update() {
	lua_getfield(L, LUA_REGISTRYINDEX, "wart");
	lua_pushnil(L);
	lua_setfield(L, -2, "tmp");
	lua_pop(L, 1);
	return LIVE;
}

static bool callclone(int iter, int narg, int nres) {
	lua_pushvalue(L, iter);				//	clone iterator
	lua_pcall(L, narg, nres, 0);			//	call it
	if(lua_isnil(L, -1)) return false;
	return true;
}

void Cue::message(string path, string msg) {
	opentable(path);
	if(lua_isstring(L, -1)) {
		lua_pushstring(L, msg.c_str());
		lua_setfield(L, 1, lua_tostring(L, -2));
	}
	lua_settop(L, 0);
}

string Cue::read(string path) {
	opentable(path);				//	I am aware that if you feed a path to read made up of tables that don't exist it will make them
	string s = "";					//	I have decided to ignore this for ease of implementation - I don't need to babygate myself from this "feature" with a flag.
	if(!lua_isnil(L, -1)) {
		lua_getfield(L, 1, lua_tostring(L, -1));
		if (const char *c = lua_tostring(L, -1)) s = c;
	}
	lua_settop(L, 0);
	return s;
}

void Cue::opentable(string path) {
	#define OPEN_DIR 1
	#define GMATCH 2

	lua_getfield(L, LUA_REGISTRYINDEX, "wart");	//	crack open the wart namespace
	lua_getglobal(L, "string");			//	open the string lib
	lua_getfield(L, -1, "gmatch");			//	and get me gmatch
	lua_pushstring(L, path.c_str());
	lua_pushstring(L, "([^/$]-)/");			//	every table ends with a slash
	lua_pcall(L, 2, 1, 0);
	lua_replace(L, GMATCH);

	while(callclone(GMATCH, 0, 1)) continue;
	stage->stackdump("pushed bits: ");

	for(int i = GMATCH + 1; !lua_isnil(L, i);  i++) {
		string s = lua_tostring(L, i);
		lua_getfield(L, OPEN_DIR, lua_tostring(L, i));
		if(lua_istable(L, -1)) {
			if(lua_isnil(L, i+1)) {	//	can't assign a table to a value
				cout << "wart: Can't open path (" << path << ") since \""  << s << "\" is already a table." << endl;
				lua_pushnil(L);
				lua_replace(L, OPEN_DIR);
				lua_settop(L, 1);
				return;
			}
			else lua_replace(L, OPEN_DIR);			//	crack open this table to write in
		}
		else if(lua_isnil(L, -1) && !lua_isnil(L, i+1)) {	//	create a new table
			cout << "wart: Creating new table \"" << s << "\"" << endl;
			lua_newtable(L);
			lua_setfield(L, OPEN_DIR, s.c_str());
			lua_getfield(L, OPEN_DIR, s.c_str());
			lua_replace(L, OPEN_DIR);
		}
		else {
			lua_settop(L, 1);
			lua_pushstring(L, s.c_str());
			return;
		}	
	}

	#undef OPEN_DIR
	#undef GMATCH 
}
