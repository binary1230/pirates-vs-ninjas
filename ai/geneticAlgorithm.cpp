#include "geneticAlgorithm.h"
#include "gene.h"

#define NINJAS_ENGINE_EXE_FILE "src/ninjas"
#define AI_RESULTS_FILE "AI-results.txt"

// UnDefine these to show the demo file being played back
// #define RUN_DEMO_AT_NORMAL_SPEED
// #define RUN_DEMO_AND_SHOW_GRAPHICS

// run the game and determine the fitness score of this individual
// returns -1 on error
//
// demo_seq_number - 	the sequence number of the demo file
// 										this is used to determine the demo filename
int GeneticAlgorithm::RunGame(const Gene &gene, int demo_seq_number) const {

	// generate the filename
	CString demoFilename;
	demoFilename.Format("gene%i.demo", demo_seq_number);

	fprintf(stderr, "\n\n---------Starting '%s'----------\n\n", 
									demoFilename.c_str());

	// write the gene's contents to the demo file
	if (!gene.WriteToFile(demoFilename)) {
		fprintf(stderr, "ERROR: Can't write to file '%s'\n", demoFilename.c_str());
		return -1;
	}
	
	// Fork off and run the game
	pid_t pid;
	int status;

	if ( (pid = fork()) ) {
		// we're the parent
		
		// wait for the child to finish
		waitpid(pid, &status, 0);

		// make sure everything's ok
		if (WIFEXITED(status)) {
			if (WEXITSTATUS(status) != 0) {
				fprintf(stderr, "ERROR: Child process terminated abnormally!\n");
				return -1;
			}
		}
		
		// open the results file, return fitness score
		FILE* f = fopen(AI_RESULTS_FILE, "r");
		char buf[255];
		int fitness;

		if (!f) {
				fprintf(stderr, 
								"ERROR: Can't open results file '%s'!\n", 
								AI_RESULTS_FILE	);	
				return -1;
		}

		if ( fscanf(f, "%i", &fitness) != 1 ) {
			fprintf(stderr, 
							"ERROR: Can't read fitness score from results file '%s'.\n",
							AI_RESULTS_FILE );
			fitness = -1;
		}
		
		fclose(f);

		if (fitness > 0) {
			fprintf(stderr, "\n\n---------Finished: '%s' Score: %i----------\n\n", 
											demoFilename.c_str(), fitness);
		}

		return fitness;

	} else {
		// we're the child
		
		const char* demoFile = demoFilename.c_str();
		
		// start the game
		execl(	NINJAS_ENGINE_EXE_FILE, 
						NINJAS_ENGINE_EXE_FILE, 

#ifndef RUN_DEMO_AT_NORMAL_SPEED
						"-8",
#endif

#ifndef RUN_DEMO_AND_SHOW_GRAPHICS
						"-9",
#endif
						"-d", demoFile,
						NULL
						);

		char* error = strerror(errno);

		fprintf(stderr, "ERROR: Failed to execute: %s: %s\n", 
										NINJAS_ENGINE_EXE_FILE, error);
		_exit(-1);
	}
}

// Update the fitness score of the specified generation
bool GeneticAlgorithm::UpdateFitnessScores() {
	int i, new_fitness, max = currentGeneration.size();

	total_fitness = 0;

	for (int i = 0; i < max; i++) {
		new_fitness = RunGame(currentGeneration[i], i);

		if (new_fitness == -1) {
			total_fitness = -1;
			return false;
		}

		currentGeneration[i].SetFitness(new_fitness);
		total_fitness += new_fitness;
	}
}

const Gene& GeneticAlgorithm::RouletteWheelSelect() const {
	int slice = int(RandomFloat() * (float)total_fitness);

	int total;
	int selected_gene = 0;

	for (int i = 0; i < population_size; i++) {
		total += currentGeneration[i].GetFitness();
		
		if (total > slice) {
			selected_gene = i;
			break;
		}
	}

	return currentGeneration[selected_gene];
}

void GeneticAlgorithm::DoCrossover(	const Gene& mom, const Gene& dad, 
																		Gene& kid1, Gene& kid2) const {
	
	float random_num = RandomFloat();

	if (random_num > crossover_rate) {
		kid1 = mom;
		kid2 = dad;
		return;
	}

	int start = Rand(0, gene_run_time - 1);
	kid1 = mom.DoCrossoverSingle(start, dad);
	kid2 = dad.DoCrossoverSingle(start, mom);
}

void GeneticAlgorithm::CreateRandomGeneration() {
	currentGeneration.resize(population_size);

	for (int i = 0; i < population_size; i++) {
		currentGeneration[i].CreateRandom(gene_run_time);
	}
}

void GeneticAlgorithm::CreateNextGeneration() {
	
	fprintf(stderr, "::Creating next generation.. ");

	GeneList nextGeneration;
	nextGeneration.resize(population_size);

	for (int i = 0; i < population_size; i++) {	
		// select 2 'good' parents
		Gene mom = RouletteWheelSelect();
		Gene dad = RouletteWheelSelect();

		// do crossover, make 2 kids
		Gene kid1, kid2;
		DoCrossover(mom, dad, kid1, kid2);
	
		// mutate the kids
		kid1.Mutate(mutation_rate);
		kid2.Mutate(mutation_rate);
	
		// add the kids in
		nextGeneration[i] 		= kid1;
		nextGeneration[i+1] 	= kid2;
	}

	currentGeneration = nextGeneration;
	
	fprintf(stderr, "::Done creating next generation.. ");
}

Gene GeneticAlgorithm::GetMostFitGene() const {
	int topscore = 0;
	int selected = 0;

	for (int i=0; i<population_size; i++) {
		if (currentGeneration[i].GetFitness() > topscore) {
			topscore = currentGeneration[i].GetFitness();
			selected = i;
		}
	}

	return currentGeneration[selected];
}

// The meat and potatoes of the genetic algorithm
bool GeneticAlgorithm::Run(int generation_count) {
	int generation_num = 0;

	// start off with a random generation
	CreateRandomGeneration();
	
	// update their fitness scores
	if (!UpdateFitnessScores()) {
		fprintf(stderr, "ERROR: Failed to update fitness scores.\n");		
		return false;
	}

	// keep making new generatins until we get to the end
	while (generation_num < generation_count) {
		generation_num++;

		CreateNextGeneration();

		if (!UpdateFitnessScores()) {
			fprintf(stderr, "ERROR: Failed to update fitness scores.\n");		
			return false;
		}

		fprintf(stderr, "Generation %i complete: Best score: %i\n", 
										generation_num, GetMostFitGene().GetFitness());
	}
}

void GeneticAlgorithm::Init(	float _mutation_rate, 
															float _crossover_rate, 
															int _population_size,
															int _gene_run_time) {
	mutation_rate = _mutation_rate;
	crossover_rate = _crossover_rate;
	SetPopulationSize(_population_size);
	gene_run_time = _gene_run_time;
}

void GeneticAlgorithm::Shutdown() {

}
