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

#ifndef WART_CAST
#define WART_CAST

/*	Tag is a agnostic handler	*/

typedef int Tag;

/*	What is a Castmember?
	A Castmember is an object that sits on the timeline. Each frame
	every Castmembers' update function is called. */

enum {
	LIVE,
	CUT,
	PAUSE,
	WRAP
};


struct Castmember {
	Tag tag, status;
	Castmember *next;
	Castmember(Tag);
	~Castmember();
	virtual Tag update() = 0;
};

/*	So what is the Cast?
	The Cast is the list of Castmembers to execute each frame. */

struct Cast {
	static bool perform();
private:
	static void purge();
	//static *Castmember find(int tag);
	//static *Castmember member_before(int tag);
	//static *Castmember member_after(int tag);
};
#endif
