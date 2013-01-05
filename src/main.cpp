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
