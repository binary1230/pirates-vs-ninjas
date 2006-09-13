#include "stdafx.h"
#include "geneticAlgorithm.h"
#include "gene.h"

// Genetic algorithm params
#define MUTATION_RATE			0.001f
#define CROSSOVER_RATE		0.7f
#define GENE_RUN_TIME			300

#define POPULATION_SIZE 	50
#define MAX_GENERATIONS 	20

#define FINAL_FILENAME 		"most-fit-gene.demo"

int main(int argc, char* argv[]) {

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

	return 0;
}
