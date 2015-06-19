#pragma once
#include "DatabaseHelper.h"
#include "Randomizer.h"
#include "Logger.h"

class Core
{
public:
	Core();
	~Core();

	void InitDatabase();
	void CreateTeams();
	void CreatePlayers();

	void StartSeason();

	std::vector<Team*>& GetTeams();
	Team* GetTeam(int id) const;
	
	std::vector<Skater*>& GetSkaters();
	Skater* GetSkater(int id) const;

	std::vector<Goalie*>& GetGoalies();
	Goalie* GetGoalie(int id) const;

	std::vector<SkaterInfo*>& GetSkaterInfo();
	std::vector<GoalieInfo*>& GetGoalieInfo();
	std::vector<TeamInfo*>& GetTeamInfo();
	
	std::string GetTimeString(int seconds);

	Logger& GetLogger();
	Randomizer& GetRandom();
	Database::DatabaseUtility GetDatabase() const;
private:
	Database::DatabaseUtility db;

	Randomizer rnd;
	Logger log;

	std::vector<Team*> teams_;
	std::vector<Skater*> skaters_;
	std::vector<Goalie*> goalies_;

	std::vector<TeamInfo*> teaminfo_;
	std::vector<SkaterInfo*> skaterinfo_;
	std::vector<GoalieInfo*> goalieinfo_;
};

extern Core core;
#define CORE core