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

void Player::refresh() {
	for(bool *i = &up; i <= &select; i++) *i = false;
}

Player::Player(Tag t) : Castmember(t) {
	refresh();
}

Tag Player::update() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				return WRAP;
			break;
		}
	}
	key = SDL_GetKeyState(NULL);
	
	if(key[SDLK_UP]) up = true;
	if(key[SDLK_DOWN]) down = true;
	if(key[SDLK_LEFT]) left = true;
	if(key[SDLK_RIGHT]) right = true;

	return LIVE;
}

bool Player::pressing(Tag t) {
	return *(&up + t);	/* C programmer, bitch ;)	*/
}
