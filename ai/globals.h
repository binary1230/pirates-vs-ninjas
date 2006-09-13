#ifndef GLOBALS_H
#define GLOBALS_H

// generate a random float between 0 and 1
#define RandomFloat() \
((float) random() / (float) 0x7fffffff)

// generate a random int in range upper to lower
#define Rand(lower, upper) \
((rand() % (upper - lower + 1)) + lower)

// The total count of gamekeys that we can use in the gene
#define GAMEKEY_COUNT 10

// The gamekey which exits the simulation
#define GAMEKEY_EXIT 10

#endif // GLOBALS_H
