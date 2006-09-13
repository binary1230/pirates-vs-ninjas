#ifndef GLOBALS_H
#define GLOBALS_H

// generate a random float between 0 and 1
#define RandomFloat() \
((float) random() / (float) 0x7fffffff)

// generate a random int in range upper to lower
#define Rand(lower, upper) \
((rand() % (upper - lower + 1)) + lower)

// The total count of gamekeys that we can use in the gene
#define GAMEKEY_COUNT 5

// The gamekey which exits the simulation
#define GAMEKEY_EXIT 10

// The random seed we are going to use for the demo file
// This must be the same for all demo files
#define RANDOM_SEED_FOR_ENGINE 42

#endif // GLOBALS_H
