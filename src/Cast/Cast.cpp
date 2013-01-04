#include "../wart.h"

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
