#pragma once

#include "DatabaseHelper.h"

class BaseInfo
{
public:
	BaseInfo();
	~BaseInfo();

	bool CacheResults(Database::DatabaseResults* pResults);

	TeamType GetTeam() const;
	int GetID() const;
	std::string GetLastName() const;
	std::string GetFirstName() const;
protected:
	// Personal info
	std::string m_szLast;
	std::string m_szFirst;
	
	int id;

	// Team info
	TeamType teamtype_;
};

class SkaterInfo : public BaseInfo
{
public:
	SkaterInfo();
	~SkaterInfo();

	bool CacheResults(Database::DatabaseResults* pResults);

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
	PositionType position_;

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

class GoalieInfo : public BaseInfo
{
public:
	GoalieInfo();
	~GoalieInfo();

	bool CacheResults(Database::DatabaseResults* pResults);

	// Reflexes
	int GetGloveHigh() const;
	int GetGloveLow() const;
	int GetStickHigh() const;
	int GetStickLow() const;
	int GetFiveHole() const;
	int GetPositioning() const;

	// Puck Control
	int GetPassing() const;
	int GetPokeCheck() const;
	int GetReboundControl() const;
	int GetShotRecovery() const;
	int GetStickHandling() const;

	// Physical
	int GetAggressiveness() const;
	int GetAgility() const;
	int GetDurability() const;
	int GetEndurance() const;
	int GetFlexibility() const;
	int GetVision() const;

private:
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

class TeamInfo
{
public:
	TeamInfo();
	~TeamInfo();

	bool CacheResults(Database::DatabaseResults* pResults);

	int GetID() const;
	std::string GetCity() const;
	std::string GetName() const;
	std::string GetAdjective() const;
	std::string GetAbbreviation() const;
private:
	int id;
	std::string city;
	std::string name;
	std::string adjective;
	std::string abbreviation;
};