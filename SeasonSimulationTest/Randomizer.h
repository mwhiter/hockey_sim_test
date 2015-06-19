#pragma once

#include <random>

class Randomizer
{
public:
	Randomizer();
	~Randomizer();

	int WeightedRandom(int* weights, int size);

	bool GetRandBool();
	int GetRandInt(int upper);
	int GetRandInt(int lower, int upper);

	bool GetRandBool(std::string msg);
	int GetRandInt(int upper, std::string msg);
	int GetRandInt(int lower, int upper, std::string msg);
private:
	std::mt19937 engine;
};