#pragma once

#include "Team.h"

class Game;

class Period
{
public:
	enum PeriodType
	{
		PERIOD_TYPE_REGULATION,
		PERIOD_TYPE_OVERTIME,
		PERIOD_TYPE_SHOOTOUT
	};

	Period(Game* game, PeriodType ePeriodType, int minutes);
	~Period();

	void Simulate();

	int GetSecondsLeft() const;
	PeriodType GetPeriodType() const;

	int GetSkillDifference() const;

	std::vector<std::pair<GameSkater*, PeriodSkaterStats>>& GetSkaterStats();
	PeriodTeamStats& GetHomeStats();
	PeriodTeamStats& GetAwayStats();

	Game* GetGame() const;
private:
	// Line Changes / Stamina Tests
	int ChooseNewForwardLine(Team* pTeam) const;
	int ChooseNewDefenseLine(Team* pTeam) const;
	void SetForwardLine(Team* pTeam, int iForwardLine);
	void SetDefenseLine(Team* pTeam, int iDefenseLine);
	void TestForwardLineShift(Team* pTeam);
	void TestDefenseLineShift(Team* pTeam);
	void CalculateStaminaChange(GameSkater* pSkater);
	void UpdateSkillDifference();

	// Various tests that occur each second
	Team* TestPossessionChange();
	ZoneType TestZoneEntry();
	ZoneType TestClear();
	void TestPenalty();

	// Shooting / Scoring Tests
	void TestShot(GameRoster* pAttackerRoster, GameRoster* pDefenderRoster);
	GameSkater* ChooseShooter(GameRoster* pAttackerRoster);
	GameSkater* ChooseAssist(GameSkater* pShooter, GameSkater* pFirstAssist, GameRoster* pAttackerRoster);

	// Faceoffs
	Team* SimulateFaceoff(FaceoffZoneType eFaceoffZone, Team* pIcingTeam = NULL);

	bool IsHomeTeam(Team* pTeam) const;

	ZoneType m_eCurrentZone;
	Team* m_pCurrentPossessingTeam;

	PeriodType ePeriodType;
	
	// Lines that are on the ice at the moment
	int iHomeForwardLine;
	int iHomeDefenseLine;
	int iAwayForwardLine;
	int iAwayDefenseLine;

	int m_iSkillDifference;

	std::vector<std::pair<GameSkater*, PeriodSkaterStats>> SkaterStats;

	PeriodTeamStats HomeStats;
	PeriodTeamStats AwayStats;

	int m_iSecondsLeft;

	Game* m_pGame;
};

class Game
{
public:
	enum OvertimeType
	{
		NO_OVERTIME,					// No overtime; game ends in a tie
		OVERTIME_FIVE_OT_SHOOTOUT,		// Five minute OT period, then a shootout
		OVERTIME_CONTINUOUS				// Continuous overtime
	};

	enum GameType
	{
		GAME_TYPE_PRESEASON,			// Preseason game
		GAME_TYPE_REGULAR_SEASON,		// Regular season game
		GAME_TYPE_PLAYOFFS				// Playoff game
	};

	Game(Team* home, Team* away, GameType eGameType, OvertimeType overtime_rules);
	~Game();

	void Simulate();

	GameType GetGameType() const;
	OvertimeType GetOvertimeType() const;

	std::vector<GameSkater*> GetGameSkaters() const;
	GameTeamStats& GetHomeStats();
	GameTeamStats& GetAwayStats();
	
	Team* GetAwayTeam() const;
	Team* GetHomeTeam() const;
	Team* GetWinner() const;

	GameRoster* GetHomeRoster() const;
	GameRoster* GetAwayRoster() const;

	bool IsPlayingInGame(Team* pTeam) const;
	bool IsFinal() const;
	uint GetID() const;
private:
	void Finalize();
	void DetermineWinner(const Period& kLastPeriod);

	static uint uiGameIDs;		// a static uint that increments when each new game is created
	uint uiId;

	GameRoster* pHomeRoster;
	GameRoster* pAwayRoster;

	std::vector<GameSkater*> paGameSkaters;

	GameTeamStats HomeStats;
	GameTeamStats AwayStats;

	bool bIsFinal;
	
	std::list<Period> periods;

	GameType eGameType;
	OvertimeType eOvertimeType;

	Team* winner_;

	Team* pAwayTeam;
	Team* pHomeTeam;
};

