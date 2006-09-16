#include "stdafx.h"
#include "geneticAlgorithm.h"
#include "gene.h"

// Genetic algorithm params
#define MUTATION_RATE			0.001f
#define CROSSOVER_RATE		0.7f
#define GENE_RUN_TIME			300

#define POPULATION_SIZE 	3
#define MAX_GENERATIONS 	2

#define FINAL_FILENAME 		"most-fit-gene.demo"

int main(int argc, char* argv[]) {

	Gene g1, g2, g3, g4;

	g1.CreateRandom(40);
	g2.CreateRandom(40);

	g3 = g1.DoCrossoverSingle(2, g2);
	g4 = g2.DoCrossoverSingle(2, g1);

	//g3.Mutate(0.7);
	// g4.Mutate(0.7);

	g1.WriteToFile("test1");
	g2.WriteToFile("test2");
	g3.WriteToFile("test3");
	g4.WriteToFile("test4");

/*
	GeneticAlgorithm ga;

	ga.Init(	MUTATION_RATE,
						CROSSOVER_RATE,
						POPULATION_SIZE,
						GENE_RUN_TIME );

	fprintf(stderr, "Starting genetic algorithm: This will take a while.\n");

	if (!ga.Run(MAX_GENERATIONS)) {
		fprintf(stderr, "ERROR: Error running genetic algorithm.\n");
		return -1;
	}

	Gene gene = ga.GetMostFitGene();

	fprintf(stderr, "FINISHED! Most fit gene: (fitness score = %i)\n"
									"          Output to: '%s'\n", 
									gene.GetFitness(), FINAL_FILENAME);

	ga.Shutdown();
*/
	return 0;
}
