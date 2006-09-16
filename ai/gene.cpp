#include "gene.h"
#include "globals.h"

void Gene::PatchUp() {
	/*bool lastSeen[GAMEKEY_COUNT];
	bzero(lastSeend, GAMEKEY_COUNT * sizeof(bool));

	// go through all the events, making sure that no two events
	// are back to back with the same button and state
	// e.g. we don't want 5 BUTTON_UP OFF messages back to back
	// that'd be pointless
	
	for (EventListIter iter = list.begin(); iter != list.end(); iter++) {
		if (lastSeen[(*iter).button] != (*iter).state) {
			
		}
	}*/

	// Not needed. Who cares, the engine should ignore it anyway.
}

bool Gene::WriteToFile(CString file) const {

	FILE* f = fopen(file.c_str(), "w");
	long last_time = -1;
	InputEvent event;

	if (!f) {
		char* err = strerror(errno);
		fprintf(stderr, "ERROR opening file '%s': %s\n", file.c_str(), err);
		return false;
	}

	// write the header
	fprintf(f, 	"DEMO:ninja-engine GA generated demo file"
							" '%s':%s:AI\n", file.c_str(), VERSION);
	
	// write the random seed
	fprintf(f, "%i", RANDOM_SEED_FOR_ENGINE);	
	
	// write the events to the file
	for (ConstEventListIter iter = events.begin(); iter != events.end(); iter++) {
		event = *iter;

		// sanity check
		assert(event.time >= last_time && "! NEW Event BEFORE our current time!");

		// make sure it isn't at or past the end
		// if it is, don't use it.
		if (event.time >= time_at_end) {
			break;
		}

		// if this event isn't at the same time as the last, output
		// the new time on a new line
		if (event.time != last_time) {
			fprintf(f, "\n%lu", event.time);
		}

		// print the new event
		fprintf(f, " %u %u", event.button, event.state);

		last_time = event.time;
	}

	// write the exit event to the file
	fprintf(f, "\n%lu %u 1", time_at_end, GAMEKEY_EXIT);
	
	fclose(f);
	
	return true;
}

bool Gene::ReadFromFile(CString file) {
	fprintf(stderr, "ERROR: Can't read from files, yet.\n");
	return false;
}

// XXX need to redo this. it kinda sucks from a GA perspective
// XXX also NOT WORKING!
Gene Gene::DoCrossoverSingle(int start, const Gene& parent) const {
	
	// ineffecient
	// make a copy of US
	/*Gene kid = *this;
	// kid.events = events;
	
	// jump to the middle somewhere
	// GOT TO BE A BETTER WAY...
	EventListIter kid_iter = kid.events.begin();
	ConstEventListIter parent_iter = parent.events.begin();
	for (int i = 0; i < start; i++) {
		kid_iter++;
		parent_iter++;
	}

	// put buttons and states but not times from other parent into KID
	// (this probably won't work too well...)
	while (kid_iter != kid.events.end() && parent_iter != parent.events.end()) {
		(*kid_iter).button 	= (*parent_iter).button;
		(*kid_iter).state 	= (*parent_iter).state;
		kid_iter++;
		parent_iter++;
	}*/

	Gene kid;
	kid.Clear();
	kid.time_at_end = time_at_end;

	ConstEventListIter p1_iter = parent.events.begin();
	ConstEventListIter p2_iter = this->events.begin();
	InputEvent event;

	int position = 0;
	unsigned long time_fudge = 0;

	// first copy from the other parent
	while (p1_iter != parent.events.end() && position < start) {
		
		event.time 		= (*p1_iter).time;
		event.button 	= (*p1_iter).button;
		event.state 	=	(*p1_iter).state;

		kid.events.push_front(event);

		p1_iter++;	p2_iter++;
		position++;
	}

	// don't go on if there's nothing else to copy
	if (p2_iter == this->events.end()) {
		return kid;
	}

	// TAKE INTO ACCOUNT TIME DIFFERENCES
	if (position > 0) {

		// see if the next event occurs in the past
		// if so we need to move its time further ahead
		if (event.time > (*p2_iter).time) {
			time_fudge = event.time - (*p2_iter).time + 1;
		}
	}
	
	// then copy the rest from us
	while (p2_iter != this->events.end()) {
		event.time 		= (*p2_iter).time;
		event.button 	= (*p2_iter).button;
		event.state 	=	(*p2_iter).state;

		kid.events.push_front(event);

		p2_iter++;
		position++;
	}

	return kid;
}

/*Gene Gene::DoCrossoverMulti(int start, int end, const Gene& parent) const {
	return parent;
}*/

// XXX: Mess with TIMING!
void Gene::Mutate(float mutation_rate) {
	for (EventListIter iter = events.begin(); iter != events.end(); iter++) {

		if (RandomFloat() < mutation_rate) {
			
			// decide to flip either the button or the state
			if (Rand(0,1)) {

				// pick a new random button to press
				(*iter).button = Rand(0, GAMEKEY_COUNT-1);

			} else {
				
				// change the button state
				(*iter).state = !(*iter).state;

			}
		}
	}
}

void Gene::Clear() {
	events.clear();
	fitness = 0;
	time_at_end = 0;
}

void Gene::CreateRandom(int max_time) {
	SetTimeAtEnd(max_time);
	int time = 1;

	InputEvent event;

	while (time < max_time - 1) {
		event.time 		= time;
		event.button 	= Rand(0, GAMEKEY_COUNT-1);
		event.state 	= Rand(0, 1);

		events.push_back(event);
		
		time += Rand(0, 20);
	}
}
		
Gene::Gene(const Gene& g) {
	Clear();
	events = g.events;
	fitness = g.fitness;
	time_at_end = g.time_at_end;
}

Gene::Gene() {
	Clear();
}

Gene::~Gene() {
	Clear();
}

