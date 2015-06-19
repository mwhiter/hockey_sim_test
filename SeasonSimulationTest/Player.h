#pragma once

#include "Infos.h"
#include "Team.h"
#include "Shot.h"
#include <string>
#include "Stats.h"

class Player {
public:
	Player(BaseInfo* pInfo);
	~Player();

	Team* GetTeam() const;

	void resetStats();

	virtual int GetOverall() const = 0;
	virtual void CalculateOverall() = 0;
	virtual BaseInfo* GetInfo() const = 0;
	virtual bool IsSkater() const = 0;
	virtual bool IsGoalie() const = 0;
private:
	Team* team_;

	int m_iOverall;
};

class Skater : public Player {
public:
	Skater(SkaterInfo* pSkaterInfo);
	~Skater();

	bool IsSkater() const;
	bool IsGoalie() const;

	SeasonSkaterStats& GetSeasonStats();

	BaseInfo* GetInfo() const;

	int GetOverall() const;
	int GetSkatingOverall() const;
	int GetShootingOverall() const;
	int GetPhysicalOverall() const;
	int GetPuckSkillsOverall() const;
	int GetDefenseOverall() const;

	void CalculateOverall();
	int CalculateSkatingOverall();
	int CalculateShootingOverall();
	int CalculatePhysicalOverall();
	int CalculatePuckSkillsOverall();
	int CalculateDefenseOverall();

	PositionType GetPosition() const;

	// Skating
	int GetAcceleration() const;
	int GetAgility() const;
	int GetBalance() const;
	int GetEndurance() const;
	int GetSpeed() const;

	// Shooting
	int GetAccuracyBackhand() const;
	int GetAccuracyForehand() const;
	int GetAccuracySlapshot() const;
	int GetPowerBackhand() const;
	int GetPowerForehand() const;
	int GetPowerSlapshot() const;
	int GetShotConfidence() const;

	// Physical
	int GetAggressiveness() const;
	int GetBodyChecking() const;
	int GetDurability() const;
	int GetFighting() const;
	int GetStrength() const;

	// Puck Skills
	int GetDeking() const;
	int GetHandEye() const;
	int GetPassing() const;
	int GetPuckControl() const;
	int GetVision() const;

	// Defense
	int GetDiscipline() const;
	int GetFaceoff() const;
	int GetPositioning() const;
	int GetShotBlocking() const;
	int GetStickChecking() const;
private:
	SkaterInfo* pSkaterInfo;

	// Season stats for skaters - todo: this needs to be a vector of all seasons
	SeasonSkaterStats m_SeasonStats;

	int m_iOverall;
	PositionType position_;

	// Skater Overalls
	int m_iShootingOverall;
	int m_iSkatingOverall;
	int m_iPhysicalOverall;
	int m_iPuckSkillsOverall;
	int m_iDefenseOverall;

	// Skating
	int m_iAcceleration;
	int m_iAgility;
	int m_iBalance;
	int m_iEndurance;
	int m_iSpeed;

	// Shooting
	int m_iAccuracyBackhand;
	int m_iAccuracyForehand;
	int m_iAccuracySlapshot;
	int m_iPowerBackhand;
	int m_iPowerForehand;
	int m_iPowerSlapshot;
	int m_iShotConfidence;

	// Physical
	int m_iAggressiveness;
	int m_iBodyChecking;
	int m_iDurability;
	int m_iFighting;
	int m_iStrength;

	// Puck Skills
	int m_iDeking;
	int m_iHandEye;
	int m_iPassing;
	int m_iPuckControl;
	int m_iVision;

	// Defense
	int m_iDiscipline;
	int m_iFaceoff;
	int m_iPositioning;
	int m_iShotBlocking;
	int m_iStickChecking;
};

class Goalie : public Player {
public:
	Goalie(GoalieInfo* pGoalieInfo);
	~Goalie();

	BaseInfo* GetInfo() const;

	int GetOverall() const;
	int GetReflexesOverall() const;
	int GetPuckControlOverall() const;
	int GetPhysicalOverall() const;

	void CalculateOverall();
	int CalculateReflexesOverall();
	int CalculatePuckControlOverall();
	int CalculatePhysicalOverall();

	bool IsSkater() const;
	bool IsGoalie() const;

	// Reflexes
	int GetGloveHigh();
	int GetGloveLow();
	int GetStickHigh();
	int GetStickLow();
	int GetFiveHole();
	int GetPositioning();

	// Puck Control
	int GetPassing();
	int GetPokeCheck();
	int GetReboundControl();
	int GetShotRecovery();
	int GetStickHandling();

	// Physical
	int GetAggressiveness();
	int GetAgility();
	int GetDurability();
	int GetEndurance();
	int GetFlexibility();
	int GetVision();
private:
	GoalieInfo* pGoalieInfo;

	int m_iOverall;

	// Goalie Overalls
	int m_iReflexesOverall;
	int m_iPuckControlOverall;
	int m_iPhysicalOverall;

	// Reflexes
	int m_iGloveHigh;
	int m_iGloveLow;
	int m_iStickHigh;
	int m_iStickLow;
	int m_iFiveHole;
	int m_iPositioning;

	// Puck Control
	int m_iPassing;
	int m_iPokeCheck;
	int m_iReboundControl;
	int m_iShotRecovery;
	int m_iStickHandling;

	// Physical
	int m_iAggressiveness;
	int m_iAgility;
	int m_iDurability;
	int m_iEndurance;
	int m_iFlexibility;
	int m_iVision;
};

// A version of a player that is only a game
class GameSkater
{
public:
	GameSkater(Skater* pSkater);
	~GameSkater();

	int GetStamina() const;
	void ChangeStamina(int iAmount);
	
	GameSkaterStats* GetGameStats() const;

	Shot* TakeShot(int iLength, int iWidth, Goalie* pGoalie);

	bool IsOnIce() const;
	void SetOnIce(bool bValue);
	
	int GetShiftTime() const;
	void SetShiftTime(int iValue);
	void ChangeShiftTime(int iChange);

	// Skating
	int GetAcceleration() const;
	int GetAgility() const;
	int GetBalance() const;
	int GetEndurance() const;
	int GetSpeed() const;

	// Shooting
	int GetAccuracyBackhand() const;
	int GetAccuracyForehand() const;
	int GetAccuracySlapshot() const;
	int GetPowerBackhand() const;
	int GetPowerForehand() const;
	int GetPowerSlapshot() const;
	int GetShotConfidence() const;

	// Physical
	int GetAggressiveness() const;
	int GetBodyChecking() const;
	int GetDurability() const;
	int GetFighting() const;
	int GetStrength() const;

	// Puck Skills
	int GetDeking() const;
	int GetHandEye() const;
	int GetPassing() const;
	int GetPuckControl() const;
	int GetVision() const;

	// Defense
	int GetDiscipline() const;
	int GetFaceoff() const;
	int GetPositioning() const;
	int GetShotBlocking() const;
	int GetStickChecking() const;

	Skater* GetSkater() const;

private:
	// Skating
	int m_iAccelerationMod;
	int m_iAgilityMod;
	int m_iBalanceMod;
	int m_iEnduranceMod;
	int m_iSpeedMod;

	// Shooting
	int m_iAccuracyBackhandMod;
	int m_iAccuracyForehandMod;
	int m_iAccuracySlapshotMod;
	int m_iPowerBackhandMod;
	int m_iPowerForehandMod;
	int m_iPowerSlapshotMod;
	int m_iShotConfidenceMod;

	// Physical
	int m_iAggressivenessMod;
	int m_iBodyCheckingMod;
	int m_iDurabilityMod;
	int m_iFightingMod;
	int m_iStrengthMod;

	// Puck Skills
	int m_iDekingMod;
	int m_iHandEyeMod;
	int m_iPassingMod;
	int m_iPuckControlMod;
	int m_iVisionMod;

	// Defense
	int m_iDisciplineMod;
	int m_iFaceoffMod;
	int m_iPositioningMod;
	int m_iShotBlockingMod;
	int m_iStickCheckingMod;

	// Game Stuff
	GameSkaterStats* pGameStats;
	int m_iStamina;
	int m_iShiftTime;
	bool m_bOnIce;

	Skater* pSkater;
};