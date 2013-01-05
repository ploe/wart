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

struct Stage {
	static void set();
	static void show();
	Stage();
	~Stage();
	//static SDL_Surface *screen;	
private:
	static SDL_Surface *screen;
};

SDL_Surface *Stage::screen = NULL;

void Stage::set() {
	screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
}

void Stage::show() {
	Stage::set();
	while(Cast::perform()) SDL_Flip(screen);
}



/*	A bunch of example cast members
	each implements different behaviour for the sake of testing. */

//	Suicidal is a Castmember who kills itself.

struct Suicidal : Castmember {
	Suicidal(Tag t) : Castmember(t) {};
	virtual Tag update();
};

Tag Suicidal::update() {
	std::cout << "Goodbye cruel world." << std::endl;
	return CUT;
}

// Greeter says Hello, every single frame.

struct Greeter : Castmember {
	Greeter(Tag t) : Castmember(t) {i = 0;};
	virtual Tag update();
private:
	int i;
};

Tag Greeter::update() {
	if(i++) std::cout << "Hello. I've seen you " << i << " times now." << std::endl;
	else std::cout << "Hello" << std::endl;
	return LIVE;
}

// The Director kills the whole program

struct Director : Castmember {
	Director(Tag t) : Castmember(t) {i = 0;};
	virtual Tag update();
private:
	int i;
};

Tag Director::update() {
	if(++i > 10)  {
		std::cout << "That's a wrap!" << std::endl;
		return WRAP;
	}
	return LIVE;
}

enum {
	MYKE,
	VIX,
	TINNY,
	GUYBRUSH
};


int main(int argc, char *argv[]) {
	//lua_State *L = lua_open();
	//if(luaL_loadfile(L, "Scenes/test.lua") || lua_pcall(L, 0, 0, 0)) std::cout << "cannot run config file: " << lua_tostring(L, -1) << std::endl;
	//lua_getglobal(L, "name");
	//lua_getglobal(L, "age");
	//std::cout << "Hello they call me, "  << lua_tostring(L, -2) << " and I'm " << lua_tostring(L, -1) << " years old." << std::endl;
	//lua_close(L);
	new Player(GUYBRUSH);
	Stage::set();
	while(Cast::perform()) continue;
	return 0;
}
