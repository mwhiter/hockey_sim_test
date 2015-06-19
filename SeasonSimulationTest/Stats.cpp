#include "stdafx.h"
#include "Stats.h"

PeriodTeamStats& PeriodTeamStats::operator+=(const PeriodTeamStats& other)
{
	PeriodTeamStats stats;
	this->iGoals += other.iGoals;
	this->iTimeOnAttack += other.iTimeOnAttack;
	this->iShots += other.iShots;
	this->iShotAttempts += other.iShotAttempts;
	this->iHits += other.iHits;
	this->iFaceoffs += other.iFaceoffs;
	this->iBlockedShots += other.iBlockedShots;
	this->iPIM += other.iPIM;
	return *this;
}

// Add period stats for one team to game stats for one team
GameTeamStats operator+(GameTeamStats& lhs, const PeriodTeamStats& rhs)
{
	lhs += rhs;
	return lhs;
}

// Add period stats for one team to game stats for one team
GameTeamStats& GameTeamStats::operator+=(const PeriodTeamStats& other)
{
	GameTeamStats stats;
	this->iGoals += other.iGoals;
	this->iTimeOnAttack += other.iTimeOnAttack;
	this->iShots += other.iShots;
	this->iShotAttempts += other.iShotAttempts;
	this->iHits += other.iHits;
	this->iFaceoffs += other.iFaceoffs;
	this->iBlockedShots += other.iBlockedShots;
	this->iPIM += other.iPIM;
	return *this;
}

// Add period stats for one team to game stats for one team
SeasonTeamStats operator+(SeasonTeamStats& lhs, const GameTeamStats& rhs)
{
	lhs += rhs;
	return lhs;
}

// Add period stats for one team to game stats for one team
SeasonTeamStats& SeasonTeamStats::operator+=(const GameTeamStats& other)
{
	SeasonTeamStats stats;
	this->iGoals += other.iGoals;
	this->iTimeOnAttack += other.iTimeOnAttack;
	this->iShots += other.iShots;
	this->iShotAttempts += other.iShotAttempts;
	this->iHits += other.iHits;
	this->iFaceoffs += other.iFaceoffs;
	this->iBlockedShots += other.iBlockedShots;
	this->iPIM += other.iPIM;
	return *this;
}

// Add period stats for one player to game stats
GameSkaterStats operator+ (GameSkaterStats& lhs, const PeriodSkaterStats& rhs)
{
	lhs += rhs;
	return lhs;
}

// Add period stats for one player to game stats
GameSkaterStats& GameSkaterStats::operator += (const PeriodSkaterStats& other)
{
	this->iGoals += other.iGoals;
	this->iAssists += other.iAssists;
	this->iShots += other.iShots;
	this->iShotAttempts += other.iShotAttempts;
	this->iPlusMinus += other.iPlusMinus;
	this->iPIM += other.iPIM;
	this->iHits += other.iHits;
	this->iBlocks += other.iBlocks;
	this->iFaceoffs += other.iFaceoffs;
	this->iTOI += other.iTOI;
	return *this;
}

// Add game stats for one player to season stats
SeasonSkaterStats operator+ (SeasonSkaterStats& lhs, const GameSkaterStats& rhs)
{
	lhs += rhs;
	return lhs;
}

// Add game stats for one player to season stats
SeasonSkaterStats& SeasonSkaterStats::operator += (const GameSkaterStats& other)
{
	this->iGoals += other.iGoals;
	this->iAssists += other.iAssists;
	this->iShots += other.iShots;
	this->iShotAttempts += other.iShotAttempts;
	this->iPlusMinus += other.iPlusMinus;
	this->iPIM += other.iPIM;
	this->iHits += other.iHits;
	this->iBlocks += other.iBlocks;
	this->iFaceoffs += other.iFaceoffs;
	this->iTOI += other.iTOI;
	return *this;
}