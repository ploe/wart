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

/* The Player class is the key listener, at the moment it's written to set private members when a key
 is depressed. But for the sake of simplicity I think I'llpush keypresses in to the Cue system I'm
 presently working on */

	struct Player : Castmember {
		virtual Tag update();
		Player(string);
		enum {
			UP, DOWN, LEFT, RIGHT, A, B, L, R, START, SELECT 
		};
		bool pressing(Tag t);
		void setkey(Tag, SDLKey);
	private:
		Uint8 *key;
		void refresh();
		bool up, down, left, right, a, b, l, r, start, select;
	};

/* KeyCaptor is a dumb little class that intercepts the Player and captures keypresses, and maps those
keys to buttons in the player using the setkey member function. */

	struct KeyCaptor : Castmember  {
		virtual Tag update();
		KeyCaptor(string);	
	};

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
