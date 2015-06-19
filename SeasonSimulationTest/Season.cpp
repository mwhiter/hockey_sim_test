#include "stdafx.h"
#include "Season.h"


Season::Season(int num_games):
	num_games(num_games)
{
}


Season::~Season()
{
}

int Season::GetNumOfGames() const
{
	return num_games;
}