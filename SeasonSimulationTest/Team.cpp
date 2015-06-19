#include "stdafx.h"

///////////////////////
// RECORD
///////////////////////

Record::Record():
	uiRegulationWin(0),
	uiRegulationLoss(0),
	uiOvertimeWin(0),
	uiOvertimeLoss(0),
	uiShootoutWin(0),
	uiShootoutLoss(0)
{

}

void Record::AwardWin(Game::GameType eGameType, Period::PeriodType ePeriodType)
{
	switch (eGameType)
	{
	case Game::GAME_TYPE_PRESEASON:
	case Game::GAME_TYPE_REGULAR_SEASON:
		switch (ePeriodType)
		{
		case Period::PERIOD_TYPE_REGULATION:
			uiRegulationWin++;
			break;
		case Period::PERIOD_TYPE_OVERTIME:
			uiOvertimeWin++;
			break;
		case Period::PERIOD_TYPE_SHOOTOUT:
			uiShootoutWin++;
			break;
		}
		break;
	// In the playoffs, ignore Period Type - only wins and losses
	case Game::GAME_TYPE_PLAYOFFS:
		uiRegulationWin++;
		break;
	}
}

void Record::AwardLoss(Game::GameType eGameType, Period::PeriodType ePeriodType)
{
	switch (eGameType)
	{
	case Game::GAME_TYPE_PRESEASON:
	case Game::GAME_TYPE_REGULAR_SEASON:
		switch (ePeriodType)
		{
		case Period::PERIOD_TYPE_REGULATION:
			uiRegulationLoss++;
			break;
		case Period::PERIOD_TYPE_OVERTIME:
			uiOvertimeLoss++;
			break;
		case Period::PERIOD_TYPE_SHOOTOUT:
			uiShootoutLoss++;
			break;
		}
		break;
		// In the playoffs, ignore Period Type - only wins and losses
	case Game::GAME_TYPE_PLAYOFFS:
		uiRegulationLoss++;
		break;
	}
}

// Returns all W's
uint_8 Record::GetWins() const
{
	return (uiRegulationWin + uiOvertimeWin + uiShootoutWin);
}

// Returns all L's (but not OTL or SOL's)
uint_8 Record::GetLosses() const
{
	return uiRegulationLoss;
}

// Returns all OTL's and SOL's (but not L's)
uint_8 Record::GetOvertimeLosses() const
{
	return (uiOvertimeLoss + uiShootoutLoss);
}

// Returns all RW's and OTW's (but not SOW's)
uint_8 Record::GetROW() const
{
	return (uiRegulationWin + uiOvertimeWin);
}

///////////////////////
// TEAM
///////////////////////

Team::Team(TeamInfo* pTeamInfo):
	m_pTeamInfo(pTeamInfo),
	m_SeasonStats(),
	m_PreSeasonRecord(),
	m_RegularSeasonRecord(),
	m_PlayoffSeasonRecord(),
	m_pRoster(new Roster(this))
{
}

Team::~Team()
{
	delete m_pRoster;
}

Record& Team::GetRegularSeasonRecord()
{
	return m_RegularSeasonRecord;
}

TeamInfo* Team::GetInfo()
{
	return m_pTeamInfo;
}

Roster* Team::GetRoster() const
{
	return m_pRoster;
}

SeasonTeamStats& Team::GetSeasonStats()
{
	return m_SeasonStats;
}