#pragma once

///////////////////////////
// TEAM STATS
///////////////////////////

// Period Stats for one team
struct PeriodTeamStats
{
	PeriodTeamStats() : iGoals(0), iTimeOnAttack(0), iShots(0), iShotAttempts(0), iHits(0), iFaceoffs(0), iBlockedShots(0), iPIM(0) {}
	PeriodTeamStats& operator += (const PeriodTeamStats& other);

	uint_8 iGoals;
	uint_16 iTimeOnAttack;
	uint_8 iShots;
	uint_8 iShotAttempts;
	uint_8 iHits;
	uint_8 iFaceoffs;
	uint_8 iBlockedShots;
	uint_16 iPIM;
};

// Game Stats for one team
struct GameTeamStats
{
	GameTeamStats() : iGoals(0), iTimeOnAttack(0), iShots(0), iShotAttempts(0), iHits(0), iFaceoffs(0), iBlockedShots(0), iPIM(0) {}

	friend GameTeamStats operator+ (GameTeamStats& lhs, const PeriodTeamStats& rhs);
	GameTeamStats& operator += (const PeriodTeamStats& other);

	uint_8 iGoals;
	uint_16 iTimeOnAttack;
	uint_8 iShots;
	uint_8 iShotAttempts;
	uint_8 iHits;
	uint_8 iFaceoffs;
	uint_8 iBlockedShots;
	uint_16 iPIM;
};

// Season Stats for one team
struct SeasonTeamStats
{
	SeasonTeamStats() : iGoals(0), iTimeOnAttack(0), iShots(0), iShotAttempts(0), iHits(0), iFaceoffs(0), iBlockedShots(0), iPIM(0) {}

	friend SeasonTeamStats operator+ (SeasonTeamStats& lhs, const GameTeamStats& rhs);
	SeasonTeamStats& operator += (const GameTeamStats& other);

	uint_16 iGoals;
	uint_16 iTimeOnAttack;
	uint_16 iShots;
	uint_16 iShotAttempts;
	uint_16 iHits;
	uint_16 iFaceoffs;
	uint_16 iBlockedShots;
	uint_16 iPIM;
};

///////////////////////////
// INDIVIDUAL STATS
///////////////////////////

// Period stats for one skater
struct PeriodSkaterStats
{
	PeriodSkaterStats() : iGoals(0), iAssists(0), iShots(0), iShotAttempts(0), iPlusMinus(0), iPIM(0), iHits(0), iBlocks(0), iFaceoffs(0), iTOI(0) { }

	uint_8 iGoals;
	uint_8 iAssists;
	uint_8 iShots;
	uint_8 iShotAttempts;
	int_8 iPlusMinus;		// can be negative
	uint_16 iPIM;
	uint_16 iHits;
	uint_16 iBlocks;
	uint_16 iFaceoffs;
	uint_16 iTOI;
};

// Game stats for one skater
struct GameSkaterStats
{
	GameSkaterStats() : iGoals(0), iAssists(0), iShots(0), iShotAttempts(0), iPlusMinus(0), iPIM(0), iHits(0), iBlocks(0), iFaceoffs(0), iTOI(0) { }

	friend GameSkaterStats operator+ (GameSkaterStats& lhs, const PeriodSkaterStats& rhs);
	GameSkaterStats& operator += (const PeriodSkaterStats& other);

	uint_8 iGoals;
	uint_8 iAssists;
	uint_8 iShots;
	uint_8 iShotAttempts;
	int_8 iPlusMinus;		// can be negative
	uint_16 iPIM;
	uint_16 iHits;
	uint_16 iBlocks;
	uint_16 iFaceoffs;
	uint_16 iTOI;
};

// Season stats for one skater
struct SeasonSkaterStats
{
	SeasonSkaterStats() : iGoals(0), iAssists(0), iShots(0), iShotAttempts(0), iPlusMinus(0), iPIM(0), iHits(0), iBlocks(0), iFaceoffs(0), iTOI(0) { }

	friend SeasonSkaterStats operator+ (SeasonSkaterStats& lhs, const GameSkaterStats& rhs);
	SeasonSkaterStats& operator += (const GameSkaterStats& other);

	uint_16 iGoals;
	uint_16 iAssists;
	uint_16 iShots;
	uint_16 iShotAttempts;
	int_16 iPlusMinus;		// can be negative, larger than others to protect against unnecessarily long 
	uint_16 iPIM;
	uint_16 iHits;
	uint_16 iBlocks;
	uint_16 iFaceoffs;
	uint iTOI;				// regular int, because seasons can be long and TOI can potentially be huge
};