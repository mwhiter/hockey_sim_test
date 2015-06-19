#include "stdafx.h"
#include "Randomizer.h"
#include <chrono>


Randomizer::Randomizer()
{
	engine.seed(std::chrono::system_clock::now().time_since_epoch().count());
}


Randomizer::~Randomizer()
{
}

// Returns a random index based on the given weights array. Requires size of weights array.
int Randomizer::WeightedRandom(int* weights, int size)
{
	int sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum += weights[i];
	}

	std::uniform_int_distribution<int> distribution(0, sum-1);
	int rnd = distribution(engine);

	for (int i = 0; i < size; i++)
	{
		if (rnd < weights[i])
			return i;
		rnd -= weights[i];
	}

	std::cout << "Error! Weighted random failed! What happened here?" << std::endl;
	return -1;
}

// Returns a random boolean, without logging
bool Randomizer::GetRandBool()
{
	return GetRandInt(0, 2) ? true : false;
}

// Returns a random integer from 0 (inclusive) to upper (exclusive), without logging
int Randomizer::GetRandInt(int upper)
{
	return GetRandInt(0, upper);
}

// Returns a random integer from lower (inclusive) to upper (exclusive), or -1 if lower < upper, without logging
int Randomizer::GetRandInt(int lower, int upper)
{
	if (lower > upper) return -1;
	std::uniform_int_distribution<int> int_dist(lower, upper - 1);
	int number = int_dist(engine);
	return number;
}

// Returns a random boolean, with logging
bool Randomizer::GetRandBool(std::string msg)
{	
	return GetRandInt(0, 2, msg) ? true : false;
}

// Returns a random integer from 0 (inclusive) to upper (exclusive), with logging
int Randomizer::GetRandInt(int upper, std::string msg)
{
	return GetRandInt(0, upper, msg);
}

// Returns a random integer from lower (inclusive) to upper (exclusive), or -1 if lower < upper, with logging
int Randomizer::GetRandInt(int lower, int upper, std::string msg)
{
	if (lower > upper) return -1;
	std::uniform_int_distribution<int> int_dist(lower, upper-1);
	
	int number = int_dist(engine);

	CORE.GetLogger().SetCurrentFile("RandomCalls.csv");
	msg += ", " + std::to_string(number);
	CORE.GetLogger().WriteLogToFile(msg);

	return number;
}