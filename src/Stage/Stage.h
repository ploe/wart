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
		Stage(string);
		~Stage();
		lua_State *lua;
		void update_viewport(int, int);
		void stackdump(string);
	private:
		SDL_Surface *screen;
		SDL_Rect viewport;
		int start;
};

extern Stage *stage;

/* The Cue system will be the simple message passing infrastructure. A Cue can either exist
till the end of the frame, pushed using the message function, or forever using the persist function.
If you want rid of a Cue you wipe it. Zero, one, infinity rule ;) */

	struct Cue : Castmember {
		void wipe(string);
		void message(string);
		void persist(string);
		virtual Status update();
		Cue(string);
	private:
		int ref;
	};

#endif
