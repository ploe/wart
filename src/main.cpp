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

#include "SDL/SDL.h"
#include "lua.hpp"


#include "wart.h"


/*	The Stage is the window	*/

enum {
	LIVE,
	CUT,
	PAUSE,
	WRAP
};

Castmember *timeline = NULL;

Castmember::Castmember(int t) {
	tag = t;
	next = timeline;
	timeline = this;
}

/*	Castmember destructor 
	Links up all the nodes before freeing the Castmember. */

Castmember::~Castmember() {
	Castmember *prev = NULL;
	for(Castmember *c = timeline; c != NULL; c = c->next) {
		if(c == this) {
			if(this->next) {
				if(prev) prev->next = this->next;	/*	middle		*/
				else timeline = this->next;		/*	top		*/
			} 
			else {
				if(prev) prev->next = NULL;		/*	bottom		*/
				else timeline = NULL;			/*	last		*/
			}
			return;
		}
		prev = c;
	}
}


/*	Cast::perform is the "heartbeat" of the program
	It is the function which is called every frame, the one which
	executes each Castmember's action, depending on its status.
	
	If its status is LIVE it is executed
	If CUT it is removed from the stack
	If PAUSE do nothing
	If ENCORE execute the same action again (could be deprecated)
	If WRAP then the program closes */

/*	FPS is the amount of frames per second,
	A JIFFY is the minimum length of a frame.	*/

#define FPS 30
#define JIFFY 30/1000

bool Cast::perform() {
	Castmember *c = timeline;
	int start = SDL_GetTicks();
	while(c != NULL) {
		if(c->status == LIVE) c->status = c->update();
		
		if(c->status == CUT) {
			Castmember *next = c->next;
			delete c;
			if(!next) break;
			c = next;
		}
		else if(c->status == WRAP) {
			purge();
			return false;
		}
		else c = c->next;
	}
	if(JIFFY > (SDL_GetTicks() - start)) SDL_Delay(JIFFY - (SDL_GetTicks() - start));
	return true;
}

/*	Cast::purge is called to clear the stack	*/

void Cast::purge() {
	Castmember *c = timeline;
	while(c) {
		Castmember *next = c->next;
		delete c;
		c = next;
		std::cout << "Another one bites the dust" << std::endl;
	}
}

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
	lua_State *L = lua_open();
	if(luaL_loadfile(L, "Scenes/test.lua") || lua_pcall(L, 0, 0, 0)) std::cout << "cannot run config file: " << lua_tostring(L, -1) << std::endl;
	lua_getglobal(L, "name");
	lua_getglobal(L, "age");
	std::cout << "Hello they call me, "  << lua_tostring(L, -2) << " and I'm " << lua_tostring(L, -1) << " years old." << std::endl;
	lua_close(L);
	//Stage::set();
	//while(Cast::perform()) continue;
	return 0;
}
