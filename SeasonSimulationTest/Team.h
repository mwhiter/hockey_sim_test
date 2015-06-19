#pragma once

#include "Roster.h"
#include "Game.h"

// belongs in standings.h or something
class Record
{
public:
	Record();
	
	void AwardWin(Game::GameType eGameType, Period::PeriodType ePeriodType);
	void AwardLoss(Game::GameType eGameType, Period::PeriodType ePeriodType);

	uint_8 GetWins() const;
	uint_8 GetLosses() const;
	uint_8 GetOvertimeLosses() const;
	uint_8 GetROW() const;
private:
	uint_8 uiRegulationWin;
	uint_8 uiRegulationLoss;
	uint_8 uiOvertimeWin;
	uint_8 uiOvertimeLoss;
	uint_8 uiShootoutWin;
	uint_8 uiShootoutLoss;
};

class Team
{
public:
	Team(TeamInfo* pTeamInfo);
	~Team();

	TeamInfo* GetInfo();
	Roster* GetRoster() const;

	Record& GetRegularSeasonRecord();

	SeasonTeamStats& GetSeasonStats();
private:
	SeasonTeamStats m_SeasonStats;
	Record m_PreSeasonRecord;
	Record m_RegularSeasonRecord;
	Record m_PlayoffSeasonRecord;

	TeamInfo* m_pTeamInfo;
	Roster* m_pRoster;
};