#include "gene.h"

void Gene::PatchUp() {

}

bool Gene::WriteToFile(CString file) const {

}

bool Gene::ReadFromFile(CString file) {

}

Gene Gene::DoCrossoverSingle(int start, const Gene& parent) const {

}

Gene Gene::DoCrossoverMulti(int start, int end, const Gene& parent) const {

}

void Gene::Mutate(float rate) {
}

void Gene::Clear() {

}

void Gene::CreateRandom(int time) {

}
		
Gene::Gene() {
	Clear();
}

Gene::~Gene() {
	Clear();
}

