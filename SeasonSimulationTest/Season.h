#pragma once

#include "Player.h"

// Represents a season
class Season
{
public:
	Season(int num_games);
	~Season();

	int GetNumOfGames() const;
private:
	int num_games;
};

