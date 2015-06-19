#include "stdafx.h"
#include <functional>

Roster::Roster(Team* pTeam) :
	pTeam(pTeam),
	skaters_(),
	goalies_(),
	forwardLines(),
	defenseLines(),
	goalieLines(),
	num_active_skaters_(0),
	num_active_goalies_(0)
{
}


Roster::~Roster()
{

}

// Adds a player to the roster, if space is available.
bool Roster::AddPlayer(Skater* pSkater)
{
	if (pSkater == NULL) return false;
	
	// If the roster is full don't add the player
	if (GetSize() >= MAX_ROSTER_SIZE) return false;

	if (num_active_skaters_ >= NUM_ACTIVE_ROSTER_SKATERS)
	{
		// Add the player, but don't dress him
		skaters_.push_back(std::pair<Skater*, bool>(pSkater, false));
	}
	else
	{
		// Add the player and dress him
		skaters_.push_back(std::pair<Skater*, bool>(pSkater, true));
		num_active_skaters_++;
	}

	return true;
}

bool Roster::AddPlayer(Goalie* pGoalie)
{
	if (pGoalie == NULL) return false;

	// If the roster is full don't add the player
	if (GetSize() >= MAX_ROSTER_SIZE) return false;

	// If we try to add a goalie but we have more than the allowed number of goalies, don't add it
	if (GetNumActiveGoalies() >= NUM_ACTIVE_ROSTER_GOALIES) return false;

	goalies_.push_back(std::pair<Goalie*, bool>(pGoalie, true));
	num_active_goalies_++;

	return true;
}

// Removes a skater (by ID) from the roster, if he exists
void Roster::RemoveSkater(int id)
{
	// Can't use the find function here otherwise we'd do a double traversal. Keep it smart.
	std::list<std::pair<Skater*, bool>>::iterator it = skaters_.begin();
	while (it != skaters_.end())
	{
		Skater* player = (*it).first;
		if (player != NULL)
		{
			BaseInfo* info = player->GetInfo();
			if (info != NULL)
			{
				if (info->GetID() == id)
				{
					// Increment the iterator (move it one step ahead) and call erase on the now-previous iterator (the value returned from i++)
					skaters_.erase(it++);
				}
			}
		}

		// Increment the iterator here, no matter what! (otherwise infinite loop)
		it++;
	}
}

// Removes a goalie (by ID) from the roster, if he exists
void Roster::RemoveGoalie(int id)
{
	// Can't use the find function here otherwise we'd do a double traversal. Keep it smart.
	std::list<std::pair<Goalie*, bool>>::iterator it = goalies_.begin();
	while (it != goalies_.end())
	{
		Goalie* player = (*it).first;
		if (player != NULL)
		{
			BaseInfo* info = player->GetInfo();
			if (info != NULL)
			{
				if (info->GetID() == id)
				{
					// Increment the iterator (move it one step ahead) and call erase on the now-previous iterator (the value returned from i++)
					goalies_.erase(it++);
				}
			}
		}

		// Increment the iterator here, no matter what! (otherwise infinite loop)
		it++;
	}
}

// Find a player on the roster by their ID. Returns NULL if no player exists.
std::pair<Skater*, bool>* Roster::FindSkater(int id)
{
	for (std::list<std::pair<Skater*, bool>>::iterator it = skaters_.begin(); it != skaters_.end(); it++)
	{
		Skater* skater = (*it).first;
		if (skater == NULL) continue;

		BaseInfo* info = skater->GetInfo();
		if (info)
		{
			if (info->GetID() == id)
				return &(*it);
		}
	}

	return NULL;
}

// Find a player on the roster by their ID. Returns NULL if no player exists.
std::pair<Goalie*, bool>* Roster::FindGoalie(int id)
{
	for (std::list<std::pair<Goalie*, bool>>::iterator it = goalies_.begin(); it != goalies_.end(); it++)
	{
		Goalie* goalie = (*it).first;
		if (goalie == NULL) continue;

		BaseInfo* info = goalie->GetInfo();
		if (info)
		{
			if (info->GetID() == id)
				return &(*it);
		}
	}

	return NULL;
}

// First scratch the entire roster
void Roster::BestLines()
{
	// Scratch skaters
	for (std::list<std::pair<Skater*, bool>>::iterator it = skaters_.begin(); it != skaters_.end(); it++)
	{
		DressSkater(it->first->GetInfo()->GetID(), false);
	}

	// Scratch goalies
	for (std::list<std::pair<Goalie*, bool>>::iterator it = goalies_.begin(); it != goalies_.end(); it++)
	{
		DressGoalie(it->first->GetInfo()->GetID(), false);
	}

	// Let's do this tomorrow
	/* 
		Code in here is going to fill out the lines.
		1. Assign a score to each player based on where he might fit best
		2. Centers - Prioritize playmakers
		3. Wingers - Prioritize snipers in the top 6. Physical guys on the 3rd and 4th lines
		4. Defenseman - Sort by overalls, try to pair offensive defenseman with defenseman defenseman whenever possible
		5. Goalies - Simple sort by overalls
	*/

	// Throw roster skaters into three separate vectors based on their position type
	std::vector<Skater*> wingers, centers, defenseman;
	for (std::list<std::pair<Skater*, bool>>::iterator it = skaters_.begin(); it != skaters_.end(); it++)
	{
		Skater* pSkater = it->first;
		switch (it->first->GetPosition())
		{
		case POSITION_TYPE_CENTER:
			centers.push_back(pSkater);
			break;
		case POSITION_TYPE_LEFT_WING:
		case POSITION_TYPE_RIGHT_WING:
			wingers.push_back(pSkater);
			break;
		case POSITION_TYPE_DEFENSE:
			defenseman.push_back(pSkater);
			break;
		default:
			std::cout << "Error building lines!" << std::endl;
			break;
		}
	}

	std::vector<Goalie*> goalies;
	for (std::list<std::pair<Goalie*, bool>>::iterator it = goalies_.begin(); it != goalies_.end(); it++)
	{
		goalies.push_back(it->first);
	}

	std::sort(centers.begin(), centers.end(), PlayerSorter<Skater>(&Skater::GetPuckSkillsOverall));
	std::sort(wingers.begin(), wingers.end(), PlayerSorter<Skater>(&Skater::GetShootingOverall));

	// First line - we want scoring potential
	forwardLines[0].SetLeftWing(wingers.back());
	wingers.pop_back();
	forwardLines[0].SetCenter(centers.back());
	centers.pop_back();
	forwardLines[0].SetRightWing(wingers.back());
	wingers.pop_back();

	// Second line
	forwardLines[1].SetLeftWing(wingers.back());
	wingers.pop_back();
	forwardLines[1].SetCenter(centers.back());
	centers.pop_back();
	forwardLines[1].SetRightWing(wingers.back());
	wingers.pop_back();

	std::sort(centers.begin(), centers.end(), PlayerSorter<Skater>(&Skater::GetOverall));
	std::sort(wingers.begin(), wingers.end(), PlayerSorter<Skater>(&Skater::GetOverall));

	// Third line
	forwardLines[2].SetLeftWing(wingers.back());
	wingers.pop_back();
	forwardLines[2].SetCenter(centers.back());
	centers.pop_back();
	forwardLines[2].SetRightWing(wingers.back());
	wingers.pop_back();

	std::sort(centers.begin(), centers.end(), PlayerSorter<Skater>(&Skater::GetDefenseOverall));
	std::sort(wingers.begin(), wingers.end(), PlayerSorter<Skater>(&Skater::GetPhysicalOverall));

	// Fourth line
	forwardLines[3].SetLeftWing(wingers.back());
	wingers.pop_back();
	forwardLines[3].SetCenter(centers.back());
	centers.pop_back();
	forwardLines[3].SetRightWing(wingers.back());
	wingers.pop_back();

	// Defenseman
	std::sort(defenseman.begin(), defenseman.end(), PlayerSorter<Skater>(&Skater::GetOverall));
	std::sort(defenseman.begin(), defenseman.end(), PlayerSorter<Skater>(&Skater::GetOverall));

	defenseLines[0].SetLeftDefenseman(defenseman.back());
	defenseman.pop_back();
	defenseLines[0].SetRightDefenseman(defenseman.back());
	defenseman.pop_back();

	defenseLines[1].SetLeftDefenseman(defenseman.back());
	defenseman.pop_back();
	defenseLines[1].SetRightDefenseman(defenseman.back());
	defenseman.pop_back();

	defenseLines[2].SetLeftDefenseman(defenseman.back());
	defenseman.pop_back();
	defenseLines[2].SetRightDefenseman(defenseman.back());
	defenseman.pop_back();

	std::sort(goalies.begin(), goalies.end(), PlayerSorter<Goalie>(&Goalie::GetOverall));

	goalieLines[0].SetGoalie(goalies.back());
	goalies.pop_back();
	goalieLines[1].SetGoalie(goalies.back());
	goalies.pop_back();

	/*
	std::cout << "-----------------------------------------" << std::endl;
	std::cout << "Team: " << pTeam->GetInfo()->GetCity() << " " << pTeam->GetInfo()->GetName() << std::endl << std::endl;

	// Print out forward lines
	for (int i = 0; i < NUM_LINES_FORWARDS; i++)
	{
		std::cout << 
			(forwardLines[i].GetLeftWing() == NULL	? "NULL" : forwardLines[i].GetLeftWing()->GetInfo()->GetLastName())		<< "(" << forwardLines[i].GetLeftWing()->GetOverall()	<< ") " <<
			(forwardLines[i].GetCenter() == NULL	? "NULL" : forwardLines[i].GetCenter()->GetInfo()->GetLastName())		<< "(" << forwardLines[i].GetCenter()->GetOverall()		<< ") " <<
			(forwardLines[i].GetRightWing() == NULL ? "NULL" : forwardLines[i].GetRightWing()->GetInfo()->GetLastName())	<< "(" << forwardLines[i].GetRightWing()->GetOverall()	<< ") " <<
		std::endl;
	}
	std::cout << std::endl;

	// Print out defenseman lines
	for (int i = 0; i < NUM_LINES_DEFENSEMEN; i++)
	{
		std::cout <<
			(defenseLines[i].GetLeftDefenseman() == NULL ? "NULL"	: defenseLines[i].GetLeftDefenseman()->GetInfo()->GetLastName())	<< "(" << defenseLines[i].GetLeftDefenseman()->GetOverall()		<< ") " <<
			(defenseLines[i].GetRightDefenseman() == NULL ? "NULL"	: defenseLines[i].GetRightDefenseman()->GetInfo()->GetLastName())	<< "(" << defenseLines[i].GetRightDefenseman()->GetOverall()	<< ") " <<
		std::endl;
	}

	std::cout << std::endl;

	// Print out goalie lines
	for (int i = 0; i < NUM_ACTIVE_ROSTER_GOALIES; i++)
	{
		std::cout << (goalieLines[i].GetGoalie() == NULL ? "NULL" : goalieLines[i].GetGoalie()->GetInfo()->GetLastName()) << "(" << goalieLines[i].GetGoalie()->GetOverall() << ") " << std::endl;
	}

	std::cout << std::endl;
	*/
}

// Score a skater as a potential center
int Roster::ScoreCenter(Skater* skater) const
{
	int score = 0;

	return score;
}

// Dresses/Scratches a player, based on the second bool. Returns false if a dress/scratch was unsuccessful
bool Roster::DressSkater(int id, bool dressed)
{
	std::pair<Skater*, bool>* skater = FindSkater(id);
	if (skater)
	{
		// Attempt to dress/scratch the player, if he's not already of that status
		if (skater->second != dressed)
		{
			skater->second = dressed;
			// If the player is now dressed, add to the active skaters or goalies depending on his type
			if (skater->second)
			{
				num_active_skaters_++;
			}
			// Opposite if he's now scratched
			else
			{
				num_active_skaters_--;
			}
			return true;
		}
		else
			return false;
	}

	return false;
}

bool Roster::DressGoalie(int id, bool dressed)
{
	std::pair<Goalie*, bool>* goalie = FindGoalie(id);
	if (goalie)
	{
		// Attempt to dress/scratch the player, if he's not already of that status
		if (goalie->second != dressed)
		{
			goalie->second = dressed;
			// If the player is now dressed, add to the active skaters or goalies depending on his type
			if (goalie->second)
			{
				num_active_goalies_++;
			}
			// Opposite if he's now scratched
			else
			{
				num_active_goalies_--;
			}
			return true;
		}
		else
			return false;
	}

	return false;
}

// Is this a valid roster for a game?
bool Roster::IsValid() const
{
	if (GetSize() >= MAX_ROSTER_SIZE) return false;
	if (num_active_skaters_ != NUM_ACTIVE_ROSTER_SKATERS) return false;
	if (num_active_goalies_ != NUM_ACTIVE_ROSTER_GOALIES) return false;
	if (GetSize() < NUM_ACTIVE_ROSTER_GOALIES + NUM_ACTIVE_ROSTER_SKATERS) return false;

	return true;
}

// Returns the forward line, line 1 = 0, line 2 = 1, etc.
ForwardLine& Roster::GetForwardLine(int line)
{
	return forwardLines[line];
}
// Returns the defense line, line 1 = 0, line 2 = 1, etc.
DefenseLine& Roster::GetDefenseLine(int line)
{
	return defenseLines[line];
}

// Returns the goalie line, starter = 0, backup = 1
GoalieLine& Roster::GetGoalieLine(int line)
{
	return goalieLines[line];
}

// Number of active skaters on this roster
int Roster::GetNumActiveSkaters() const
{
	return num_active_skaters_;
}

// Number of active goalies on this roster
int Roster::GetNumActiveGoalies() const
{
	return num_active_goalies_;
}

// Return the number of players currently on the roster, including both active and scratched players
int Roster::GetSize() const
{
	return skaters_.size() + goalies_.size();
}

const std::list<std::pair<Skater*, bool>>& Roster::GetSkaters()
{
	return skaters_;
}

//////////////////////////////
// GAME ROSTER
//////////////////////////////

GameRoster::GameRoster(Team* pTeam):
	pTeam(pTeam),
	pRoster(pTeam->GetRoster())
{
	// Forwards
	for (int iForwardLine = 0; iForwardLine < NUM_LINES_FORWARDS; iForwardLine++)
	{
		ForwardLine kRosterForwardLines = pRoster->GetForwardLine(iForwardLine);
		GameForwardLine* pNewForwardLine = new GameForwardLine();
		for (int iForward = 0; iForward < NUM_FORWARDS_PER_LINE; iForward++)
		{
			// Implicit copy constructor should be good enoug here - there are no arrays in Skater class
			GameSkater* pForwardCopy = new GameSkater(kRosterForwardLines.GetForward(iForward));
			pNewForwardLine->SetForward(iForward, pForwardCopy);
		}
		paForwardLines[iForwardLine] = pNewForwardLine;
	}

	// Defensemen
	for (int iDefenseLine = 0; iDefenseLine < NUM_LINES_DEFENSEMEN; iDefenseLine++)
	{
		DefenseLine kRosterDefenseLines = pRoster->GetDefenseLine(iDefenseLine);
		GameDefenseLine* pNewDefenseLine = new GameDefenseLine();
		for (int iDefense = 0; iDefense < NUM_DEFENSEMEN_PER_LINE; iDefense++)
		{
			// Implicit copy constructor should be good enoug here - there are no arrays in Skater classy
			GameSkater* pSkaterCopy = new GameSkater(kRosterDefenseLines.GetDefenseman(iDefense));
			pNewDefenseLine->SetDefenseman(iDefense, pSkaterCopy);
		}
		paDefenseLines[iDefenseLine] = pNewDefenseLine;
	}

	// Goalies
	for (int iGoalieLine = 0; iGoalieLine < NUM_ACTIVE_ROSTER_GOALIES; iGoalieLine++)
	{
		GoalieLine kRosterGoalieLine = pRoster->GetGoalieLine(iGoalieLine);
		// Implicit copy constructor should be good enoug here - there are no arrays in Goalie class
		GoalieLine* pNewGoalieLine = new GoalieLine();
		Goalie* pGoalieCopy = new Goalie(*kRosterGoalieLine.GetGoalie());
		pNewGoalieLine->SetGoalie(pGoalieCopy);
		paGoalieLines[iGoalieLine] = pNewGoalieLine;
	}
}

GameRoster::~GameRoster()
{
	// Clear the memory we allocated for all Forwards
	for (int iForwardLine = 0; iForwardLine < NUM_LINES_FORWARDS; iForwardLine++)
	{
		for (int iForward = 0; iForward < NUM_FORWARDS_PER_LINE; iForward++)
		{
			// Delete the copied Forward here
			delete paForwardLines[iForwardLine]->GetForward(iForward);
		}
		delete paForwardLines[iForwardLine];
	}

	// Clear the memory we allocated for all Defenseman
	for (int iDefenseLine = 0; iDefenseLine < NUM_LINES_DEFENSEMEN; iDefenseLine++)
	{
		for (int iDefense = 0; iDefense < NUM_DEFENSEMEN_PER_LINE; iDefense++)
		{
			// Delete the copied Defenseman here
			delete paDefenseLines[iDefenseLine]->GetDefenseman(iDefense);
		}
		delete paDefenseLines[iDefenseLine];
	}

	// Clear the memory we allocated for all Goalies
	for (int iGoalieLine = 0; iGoalieLine < NUM_ACTIVE_ROSTER_GOALIES; iGoalieLine++)
	{
		delete paGoalieLines[iGoalieLine]->GetGoalie();
	}
}

GameForwardLine* GameRoster::GetForwardLine(int iLine) const
{
	return paForwardLines[iLine];
}

GameDefenseLine* GameRoster::GetDefenseLine(int iLine) const
{
	return paDefenseLines[iLine];
}

GoalieLine* GameRoster::GetGoalie(int iLine) const
{
	return paGoalieLines[iLine];
}

Team* GameRoster::GetTeam() const
{
	return pTeam;
}