#include "stdafx.h"

#ifndef GENE_H
#define GENE_H

#include "globals.h"
#include "inputEvent.h"

typedef list<InputEvent> EventList;
typedef list<InputEvent>::iterator EventListIter;
typedef list<InputEvent>::const_iterator ConstEventListIter;

// Represents an event list (e.g. a DEMO FILE)

class Gene {
	private:
		EventList events;
		int fitness;

		// Time at which the EXIT event occurs
		unsigned long time_at_end;

		// clean up the gene into a valid sequence for
		// playing back a demo
		void PatchUp();

	public:
		bool WriteToFile(const CString file) const;
		bool ReadFromFile(const CString file);

		void SetFitness(const int f) {fitness=f;};
		int GetFitness() const {return fitness;};

		void SetTimeAtEnd(unsigned long time) {time_at_end = time;};
		unsigned long GetTimeAtEnd() {return time_at_end;};

		Gene DoCrossoverSingle(	int start, const Gene& parent) const;

		// not yet.
		// Gene DoCrossoverMulti(	int start, int end, const Gene& parent) const;

		void Mutate(float mutation_rate);
		void Clear();
	
		// Create a random sequence (used for initial generation)
		// time is the length of time (in frames) to allow this demo
		// to run for in the game (e.g. 1000 means a demo for about 30 seconds)
		void CreateRandom(int time);
		
		Gene();
		~Gene();
};

#endif // GENE_H
