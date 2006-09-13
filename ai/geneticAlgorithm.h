#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "stdafx.h"
#include "globals.h"
#include "gene.h"

// class Gene;

typedef vector<Gene> GeneList;

class GeneticAlgorithm {
	protected:
		GeneList currentGeneration;

		// Genetic algorithm paramaters
		float mutation_rate;
		float crossover_rate;
		int population_size;
		int gene_run_time; 		// time in frames to run the simulation for

		int total_fitness;
	
		// compute the fitness scores of the current generation by
		// running the game and recording the fitness scores
		bool UpdateFitnessScores();

		const Gene& RouletteWheelSelect() const;
		void DoCrossover(	const Gene& mom, const Gene& dad, 
											Gene& kid1, Gene& kid2) const;

		void CreateNextGeneration();
		void Mutate();

		// computes the next generation from an existing one
		void ComputeNextGeneration();

		// creates a random initial generation based on the
		// paramaters which are already set
		void CreateRandomGeneration();

		int RunGame(const Gene &gene, int demo_seq_number) const;

	public:
		inline void SetMutationRate(float r) {mutation_rate = r;};
		inline float GetMutationRate() {return mutation_rate;};
		
		inline void SetCrossoverRate(float r) {crossover_rate = r;};
		inline float GetCrossoverRate() {return crossover_rate;};
		
		inline void SetGeneRunTime(int _time) {gene_run_time = _time;};
		inline int GetGeneRunTime() const {return gene_run_time;};

		// Population size must be an even number, or we will make it one.
		inline void SetPopulationSize(int s) {population_size = (s%2==1)?s+1:s;};
		inline int GetPopulationSize() {return population_size;};

		// Load an initial random generation, be sure to set the 
		// genetic algorithm params first.
		void LoadRandomGeneration();

		void LoadGenerationFromFiles(CString basename, int count);

		// Return the most fit gene
		Gene GetMostFitGene() const;

		// Run the genetic algorithm for the specified count
		bool Run(int generation_count);

		void Init(  float _mutation_rate,
								float _crossover_rate,
								int _population_size,
								int _gene_run_time);

		void Shutdown();
};

#endif // GENETIC_ALGORITHM_H
