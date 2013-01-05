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

#ifndef WART_PLAYER
#define WART_PLAYER

	struct Player : Castmember {
		virtual Tag update();
		Player(Tag t);
		enum {
			UP, DOWN, LEFT, RIGHT, A, B, L, R, START, SELECT 
		};
		bool pressing(Tag t);
	private:
		Uint8 *key;
		void refresh();
		bool up, down, left, right, a, b, l, r, start, select;
	};

#endif
