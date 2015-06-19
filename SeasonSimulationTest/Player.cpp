#include "stdafx.h"

Player::Player(BaseInfo* pInfo) :
	m_iOverall(0),
	team_(CORE.GetTeams()[pInfo->GetTeam()])
{
}

Player::~Player()
{

}

Team* Player::GetTeam() const
{
	return team_;
}

//////////////////////////////
// SKATER
//////////////////////////////

Skater::Skater(SkaterInfo* pSkaterInfo) :
	m_SeasonStats(),
	position_(pSkaterInfo->GetPosition()),
	pSkaterInfo(pSkaterInfo),
	m_iAcceleration(pSkaterInfo->GetAcceleration()),
	m_iAgility(pSkaterInfo->GetAgility()),
	m_iBalance(pSkaterInfo->GetBalance()),
	m_iEndurance(pSkaterInfo->GetEndurance()),
	m_iSpeed(pSkaterInfo->GetSpeed()),

	m_iAccuracyBackhand(pSkaterInfo->GetAccuracyBackhand()),
	m_iAccuracyForehand(pSkaterInfo->GetAccuracyForehand()),
	m_iAccuracySlapshot(pSkaterInfo->GetAccuracySlapshot()),
	m_iPowerBackhand(pSkaterInfo->GetPowerBackhand()),
	m_iPowerForehand(pSkaterInfo->GetPowerForehand()),
	m_iPowerSlapshot(pSkaterInfo->GetPowerSlapshot()),
	m_iShotConfidence(pSkaterInfo->GetShotConfidence()),

	m_iAggressiveness(pSkaterInfo->GetAggressiveness()),
	m_iBodyChecking(pSkaterInfo->GetBodyChecking()),
	m_iDurability(pSkaterInfo->GetDurability()),
	m_iFighting(pSkaterInfo->GetFighting()),
	m_iStrength(pSkaterInfo->GetStrength()),

	m_iDeking(pSkaterInfo->GetDeking()),
	m_iHandEye(pSkaterInfo->GetHandEye()),
	m_iPassing(pSkaterInfo->GetPassing()),
	m_iPuckControl(pSkaterInfo->GetPuckControl()),
	m_iVision(pSkaterInfo->GetVision()),

	m_iDiscipline(pSkaterInfo->GetDiscipline()),
	m_iFaceoff(pSkaterInfo->GetFaceoff()),
	m_iPositioning(pSkaterInfo->GetPositioning()),
	m_iShotBlocking(pSkaterInfo->GetShotBlocking()),
	m_iStickChecking(pSkaterInfo->GetStickChecking()),
	Player((BaseInfo*)pSkaterInfo)
{
	CalculateOverall();
}


Skater::~Skater()
{
}

bool operator<(Player &p1, Player &p2)
{
	return p1.GetOverall() < p2.GetOverall();
}

// Calculate the skater's overall based on his stats
void Skater::CalculateOverall()
{
	const int NUM_WEIGHT = 5;
	int weights[NUM_WEIGHT];
	if (position_ == POSITION_TYPE_LEFT_WING || position_ == POSITION_TYPE_RIGHT_WING)
	{
		weights[0] = 50;
		weights[1] = 75;
		weights[2] = 50;
		weights[3] = 100;
		weights[4] = 75;
	}
	else  if (position_ == POSITION_TYPE_CENTER)
	{
		weights[0] = 50;
		weights[1] = 50;
		weights[2] = 50;
		weights[3] = 100;
		weights[4] = 80;
	}
	else if (position_ == POSITION_TYPE_DEFENSE)
	{
		weights[0] = 75;
		weights[1] = 25;
		weights[2] = 75;
		weights[3] = 50;
		weights[4] = 100;
	}

	double overall = 0;

	m_iShootingOverall = CalculateSkatingOverall();
	m_iSkatingOverall = CalculateShootingOverall();
	m_iPhysicalOverall = CalculatePhysicalOverall();
	m_iPuckSkillsOverall = CalculatePuckSkillsOverall();
	m_iDefenseOverall = CalculateDefenseOverall();

	overall += GetSkatingOverall() * weights[0];
	overall += GetShootingOverall() * weights[1];
	overall += GetPhysicalOverall() * weights[2];
	overall += GetPuckSkillsOverall() * weights[3];
	overall += GetDefenseOverall() * weights[4];

	int sum_weight = 0;
	for (int i = 0; i < NUM_WEIGHT; i++)
		sum_weight += weights[i];
	overall /= sum_weight;

	m_iOverall = (int)round(overall);
}

int Skater::CalculateSkatingOverall()
{
	const int NUM_WEIGHT = 5;
	int weights[NUM_WEIGHT];
	if (position_ == POSITION_TYPE_LEFT_WING || position_ == POSITION_TYPE_RIGHT_WING)
	{
		weights[0] = 100;
		weights[1] = 75;
		weights[2] = 100;
		weights[3] = 25;
		weights[4] = 75;
	}
	else  if (position_ == POSITION_TYPE_CENTER)
	{
		weights[0] = 75;
		weights[1] = 100;
		weights[2] = 100;
		weights[3] = 25;
		weights[4] = 75;
	}
	else if (position_ == POSITION_TYPE_DEFENSE)
	{
		weights[0] = 75;
		weights[1] = 75;
		weights[2] = 100;
		weights[3] = 25;
		weights[4] = 100;
	}

	double overall = 0;

	overall += m_iAcceleration * weights[0];
	overall += m_iAgility * weights[1];
	overall += m_iBalance * weights[2];
	overall += m_iEndurance * weights[3];
	overall += m_iSpeed * weights[4];

	int sum_weight = 0;
	for (int i = 0; i < NUM_WEIGHT; i++)
		sum_weight += weights[i];
	overall /= sum_weight;

	return (int)round(overall);
}

int Skater::CalculateShootingOverall()
{
	const int NUM_WEIGHT = 7;
	int weights[NUM_WEIGHT];
	if (position_ == POSITION_TYPE_LEFT_WING || position_ == POSITION_TYPE_RIGHT_WING)
	{
		weights[0] = 75;
		weights[1] = 75;
		weights[2] = 100;
		weights[3] = 100;
		weights[4] = 50;
		weights[5] = 50;
		weights[6] = 50;
	}
	else  if (position_ == POSITION_TYPE_CENTER)
	{
		weights[0] = 75;
		weights[1] = 75;
		weights[2] = 100;
		weights[3] = 100;
		weights[4] = 50;
		weights[5] = 50;
		weights[6] = 50;
	}
	else if (position_ == POSITION_TYPE_DEFENSE)
	{
		weights[0] = 50;
		weights[1] = 50;
		weights[2] = 75;
		weights[3] = 75;
		weights[4] = 100;
		weights[5] = 100;
		weights[6] = 50;
	}

	double overall = 0;

	overall += m_iAccuracyBackhand * weights[0];
	overall += m_iAccuracyForehand * weights[1];
	overall += m_iAccuracySlapshot * weights[2];
	overall += m_iPowerBackhand * weights[3];
	overall += m_iPowerForehand * weights[4];
	overall += m_iPowerSlapshot * weights[5];
	overall += m_iShotConfidence * weights[6];

	int sum_weight = 0;
	for (int i = 0; i < NUM_WEIGHT; i++)
		sum_weight += weights[i];
	overall /= sum_weight;

	return (int)round(overall);
}

int Skater::CalculatePhysicalOverall()
{
	const int NUM_WEIGHT = 5;
	int weights[NUM_WEIGHT];
	if (position_ == POSITION_TYPE_LEFT_WING || position_ == POSITION_TYPE_RIGHT_WING)
	{
		weights[0] = 50;
		weights[1] = 75;
		weights[2] = 25;
		weights[3] = 25;
		weights[4] = 100;
	}
	else  if (position_ == POSITION_TYPE_CENTER)
	{
		weights[0] = 50;
		weights[1] = 100;
		weights[2] = 25;
		weights[3] = 10;
		weights[4] = 75;
	}
	else if (position_ == POSITION_TYPE_DEFENSE)
	{
		weights[0] = 75;
		weights[1] = 100;
		weights[2] = 25;
		weights[3] = 25;
		weights[4] = 75;
	}

	double overall = 0;

	overall += m_iAggressiveness * weights[0];
	overall += m_iBodyChecking * weights[1];
	overall += m_iDurability * weights[2];
	overall += m_iFighting * weights[3];
	overall += m_iStrength * weights[4];

	int sum_weight = 0;
	for (int i = 0; i < NUM_WEIGHT; i++)
		sum_weight += weights[i];
	overall /= sum_weight;

	return (int)round(overall);
}

int Skater::CalculatePuckSkillsOverall()
{
	const int NUM_WEIGHT = 5;
	int weights[NUM_WEIGHT];
	if (position_ == POSITION_TYPE_LEFT_WING || position_ == POSITION_TYPE_RIGHT_WING)
	{
		weights[0] = 50;
		weights[1] = 100;
		weights[2] = 75;
		weights[3] = 100;
		weights[4] = 75;
	}
	else  if (position_ == POSITION_TYPE_CENTER)
	{
		weights[0] = 25;
		weights[1] = 75;
		weights[2] = 100;
		weights[3] = 50;
		weights[4] = 100;
	}
	else if (position_ == POSITION_TYPE_DEFENSE)
	{
		weights[0] = 25;
		weights[1] = 50;
		weights[2] = 75;
		weights[3] = 100;
		weights[4] = 75;
	}

	double overall = 0;

	overall += m_iDeking * weights[0];
	overall += m_iHandEye * weights[1];
	overall += m_iPassing * weights[2];
	overall += m_iPuckControl * weights[3];
	overall += m_iVision * weights[4];

	int sum_weight = 0;
	for (int i = 0; i < NUM_WEIGHT; i++)
		sum_weight += weights[i];
	overall /= sum_weight;

	return (int)round(overall);
}

int Skater::CalculateDefenseOverall()
{
	const int NUM_WEIGHT = 5;
	int weights[NUM_WEIGHT];
	if (position_ == POSITION_TYPE_LEFT_WING || position_ == POSITION_TYPE_RIGHT_WING)
	{
		weights[0] = 25;
		weights[1] = 0;
		weights[2] = 75;
		weights[3] = 50;
		weights[4] = 100;
	}
	else  if (position_ == POSITION_TYPE_CENTER)
	{
		weights[0] = 25;
		weights[1] = 100;
		weights[2] = 75;
		weights[3] = 75;
		weights[4] = 75;
	}
	else if (position_ == POSITION_TYPE_DEFENSE)
	{
		weights[0] = 25;
		weights[1] = 0;
		weights[2] = 100;
		weights[3] = 75;
		weights[4] = 100;
	}

	double overall = 0;

	overall += m_iDiscipline * weights[0];
	overall += m_iFaceoff * weights[1];
	overall += m_iPositioning * weights[2];
	overall += m_iShotBlocking * weights[3];
	overall += m_iStickChecking * weights[4];

	int sum_weight = 0;
	for (int i = 0; i < NUM_WEIGHT; i++)
		sum_weight += weights[i];
	overall /= sum_weight;

	return (int)round(overall);
}

// Return the player's Overall
int Skater::GetOverall() const
{
	return m_iOverall;
}

// Return the skater's Skating Overall
int Skater::GetSkatingOverall() const
{
	return m_iShootingOverall;
}

// Return the skater's Shooting Overall
int Skater::GetShootingOverall() const
{
	return m_iSkatingOverall;
}

// Return the skater's Physical Overall
int Skater::GetPhysicalOverall() const
{
	return m_iPhysicalOverall;
}

// Return the skater's Puck Skills Overall
int Skater::GetPuckSkillsOverall() const
{
	return m_iPuckSkillsOverall;
}

// Return the skater's Defense Overall
int Skater::GetDefenseOverall() const
{
	return m_iDefenseOverall;
}

PositionType Skater::GetPosition() const
{
	return position_;
}

BaseInfo* Skater::GetInfo() const
{
	return pSkaterInfo;
}

bool Skater::IsSkater() const
{
	return true;
}

bool Skater::IsGoalie() const
{
	return false;
}

SeasonSkaterStats& Skater::GetSeasonStats()
{
	return m_SeasonStats;
}

// Skating
int Skater::GetAcceleration() const{ return m_iAcceleration; }
int Skater::GetAgility() const{ return m_iAgility; }
int Skater::GetBalance() const{ return m_iBalance; }
int Skater::GetEndurance() const{ return m_iEndurance; }
int Skater::GetSpeed() const{ return m_iSpeed; }

// Shooting
int Skater::GetAccuracyBackhand() const{ return m_iAccuracyBackhand; }
int Skater::GetAccuracyForehand() const{ return m_iAccuracyForehand; }
int Skater::GetAccuracySlapshot() const{ return m_iAccuracySlapshot; }
int Skater::GetPowerBackhand() const{ return m_iPowerBackhand; }
int Skater::GetPowerForehand() const{ return m_iPowerForehand; }
int Skater::GetPowerSlapshot() const{ return m_iPowerSlapshot; }
int Skater::GetShotConfidence() const{ return m_iShotConfidence; }

// Physical
int Skater::GetAggressiveness() const{ return m_iAggressiveness; }
int Skater::GetBodyChecking() const{ return m_iBodyChecking; }
int Skater::GetDurability() const{ return m_iDurability; }
int Skater::GetFighting() const{ return m_iFighting; }
int Skater::GetStrength() const{ return m_iStrength; }

// Puck Skills
int Skater::GetDeking() const{ return m_iDeking; }
int Skater::GetHandEye() const{ return m_iHandEye; }
int Skater::GetPassing() const{ return m_iPassing; }
int Skater::GetPuckControl() const{ return m_iPuckControl; }
int Skater::GetVision() const{ return m_iVision; }

// Defense
int Skater::GetDiscipline() const{ return m_iDiscipline; }
int Skater::GetFaceoff() const{ return m_iFaceoff; }
int Skater::GetPositioning() const{ return m_iPositioning; }
int Skater::GetShotBlocking() const{ return m_iShotBlocking; }
int Skater::GetStickChecking() const{ return m_iStickChecking; }

/////////////////////////////
// GOALIE
/////////////////////////////

Goalie::Goalie(GoalieInfo* pGoalieInfo):
	pGoalieInfo(pGoalieInfo),
	m_iGloveHigh(pGoalieInfo->GetGloveHigh()),
	m_iGloveLow(pGoalieInfo->GetGloveLow()),
	m_iStickHigh(pGoalieInfo->GetStickHigh()),
	m_iStickLow(pGoalieInfo->GetStickHigh()),
	m_iFiveHole(pGoalieInfo->GetFiveHole()),
	m_iPositioning(pGoalieInfo->GetPositioning()),
	m_iPassing(pGoalieInfo->GetPassing()),
	m_iPokeCheck(pGoalieInfo->GetPokeCheck()),
	m_iReboundControl(pGoalieInfo->GetReboundControl()),
	m_iShotRecovery(pGoalieInfo->GetShotRecovery()),
	m_iStickHandling(pGoalieInfo->GetStickHandling()),
	m_iAggressiveness(pGoalieInfo->GetAggressiveness()),
	m_iAgility(pGoalieInfo->GetAgility()),
	m_iDurability(pGoalieInfo->GetDurability()),
	m_iEndurance(pGoalieInfo->GetEndurance()),
	m_iFlexibility(pGoalieInfo->GetFlexibility()),
	m_iVision(pGoalieInfo->GetVision()),
	Player((BaseInfo*)pGoalieInfo)
{
	CalculateOverall();
}

Goalie::~Goalie()
{

}

int Goalie::GetOverall() const
{
	return m_iOverall;
}

int Goalie::GetReflexesOverall() const
{
	return m_iReflexesOverall;
}

int Goalie::GetPuckControlOverall() const
{
	return m_iPuckControlOverall;
}

int Goalie::GetPhysicalOverall() const
{
	return m_iPhysicalOverall;
}

void Goalie::CalculateOverall()
{
	const int NUM_WEIGHT = 3;
	int weights[NUM_WEIGHT] = {100, 80, 85};

	double overall = 0;

	m_iReflexesOverall = CalculateReflexesOverall();
	m_iPuckControlOverall = CalculatePuckControlOverall();
	m_iPhysicalOverall = CalculatePhysicalOverall();

	overall += GetReflexesOverall() * weights[0];
	overall += GetPuckControlOverall() * weights[1];
	overall += GetPhysicalOverall() * weights[2];

	int sum_weight = 0;
	for (int i = 0; i < NUM_WEIGHT; i++)
		sum_weight += weights[i];
	overall /= sum_weight;

	m_iOverall = (int)round(overall);
}
int Goalie::CalculateReflexesOverall()
{
	const int NUM_WEIGHT = 6;
	int weights[NUM_WEIGHT] = { 80, 80, 80, 80, 80, 100};

	double overall = 0;

	overall += m_iGloveHigh * weights[0];
	overall += m_iGloveLow * weights[1];
	overall += m_iStickHigh * weights[2];
	overall += m_iStickLow * weights[3];
	overall += m_iFiveHole * weights[4];
	overall += m_iPositioning * weights[5];

	int sum_weight = 0;
	for (int i = 0; i < NUM_WEIGHT; i++)
		sum_weight += weights[i];
	overall /= sum_weight;

	return (int)round(overall);
}
int Goalie::CalculatePuckControlOverall()
{
	const int NUM_WEIGHT = 5;
	int weights[NUM_WEIGHT] = { 60, 80, 100, 100, 60 };

	double overall = 0;

	overall += m_iPassing * weights[0];
	overall += m_iPokeCheck * weights[1];
	overall += m_iReboundControl * weights[2];
	overall += m_iShotRecovery * weights[3];
	overall += m_iStickHandling * weights[4];

	int sum_weight = 0;
	for (int i = 0; i < NUM_WEIGHT; i++)
		sum_weight += weights[i];
	overall /= sum_weight;

	return (int)round(overall);
}
int Goalie::CalculatePhysicalOverall()
{
	const int NUM_WEIGHT = 6;
	int weights[NUM_WEIGHT] = { 65, 100, 60, 75, 80, 100 };

	double overall = 0;

	overall += m_iAggressiveness * weights[0];
	overall += m_iAgility * weights[1];
	overall += m_iDurability * weights[2];
	overall += m_iEndurance * weights[3];
	overall += m_iFlexibility * weights[4];
	overall += m_iVision * weights[5];

	int sum_weight = 0;
	for (int i = 0; i < NUM_WEIGHT; i++)
		sum_weight += weights[i];
	overall /= sum_weight;

	return (int)round(overall);
}

BaseInfo* Goalie::GetInfo() const
{
	return pGoalieInfo;
}

bool Goalie::IsSkater() const
{
	return false;
}

bool Goalie::IsGoalie() const
{
	return true;
}

// Reflexes
int Goalie::GetGloveHigh() { return m_iGloveHigh; }
int Goalie::GetGloveLow() { return m_iGloveLow; }
int Goalie::GetStickHigh() { return m_iStickHigh; }
int Goalie::GetStickLow() { return m_iStickLow; }
int Goalie::GetFiveHole() { return m_iFiveHole; }
int Goalie::GetPositioning() { return m_iPositioning; }

// Puck Control
int Goalie::GetPassing() { return m_iPassing; }
int Goalie::GetPokeCheck() { return m_iPokeCheck; }
int Goalie::GetReboundControl() { return m_iReboundControl; }
int Goalie::GetShotRecovery() { return m_iShotRecovery; }
int Goalie::GetStickHandling() { return m_iStickHandling; }

// Physical
int Goalie::GetAggressiveness() { return m_iAggressiveness; }
int Goalie::GetAgility() { return m_iAgility; }
int Goalie::GetDurability() { return m_iDurability; }
int Goalie::GetEndurance() { return m_iEndurance; }
int Goalie::GetFlexibility() { return m_iFlexibility; }
int Goalie::GetVision() { return m_iVision; }

///////////////////////////////////
// GamePlayer
///////////////////////////////////

GameSkater::GameSkater(Skater* pSkater) :
	pSkater(pSkater),
	pGameStats(new GameSkaterStats()),
	m_iStamina(10000),
	m_bOnIce(false),

	// Skating
	m_iAccelerationMod(0),
	m_iAgilityMod(0),
	m_iBalanceMod(0),
	m_iEnduranceMod(0),
	m_iSpeedMod(0),

	// Shooting
	m_iAccuracyBackhandMod(0),
	m_iAccuracyForehandMod(0),
	m_iAccuracySlapshotMod(0),
	m_iPowerBackhandMod(0),
	m_iPowerForehandMod(0),
	m_iPowerSlapshotMod(0),
	m_iShotConfidenceMod(0),

	// Physical
	m_iAggressivenessMod(0),
	m_iBodyCheckingMod(0),
	m_iDurabilityMod(0),
	m_iFightingMod(0),
	m_iStrengthMod(0),

	// Puck Skills
	m_iDekingMod(0),
	m_iHandEyeMod(0),
	m_iPassingMod(0),
	m_iPuckControlMod(0),
	m_iVisionMod(0),

	// Defense
	m_iDisciplineMod(0),
	m_iFaceoffMod(0),
	m_iPositioningMod(0),
	m_iShotBlockingMod(0),
	m_iStickCheckingMod(0)
{

}

GameSkater::~GameSkater()
{
	delete pGameStats;
}

// Get the player's current stamina
int GameSkater::GetStamina() const
{
	return m_iStamina;
}

// Change the player's stamina (bounded between 0 and 10000
void GameSkater::ChangeStamina(int iAmount)
{
	const int MAX_SKATER_STAMINA = 10000;

	m_iStamina += iAmount;
	m_iStamina = std::min(m_iStamina, MAX_SKATER_STAMINA);
	m_iStamina = std::max(m_iStamina, 0);

	// After we change stamina, change the mods according to the formula.
	m_iAccelerationMod = -((MAX_SKATER_STAMINA - m_iStamina) / 250);
	m_iAgilityMod = -((MAX_SKATER_STAMINA - m_iStamina) / 400);
	m_iSpeedMod = -((MAX_SKATER_STAMINA - m_iStamina) / 400);
	m_iPowerBackhandMod = -((MAX_SKATER_STAMINA - m_iStamina) / 1000);
	m_iPowerForehandMod = -((MAX_SKATER_STAMINA - m_iStamina) / 1000);
	m_iPowerSlapshotMod = -((MAX_SKATER_STAMINA - m_iStamina) / 1000);
	m_iDisciplineMod = -((MAX_SKATER_STAMINA - m_iStamina) / 2000);
	m_iAggressivenessMod = -((MAX_SKATER_STAMINA - m_iStamina) / 1000);
	m_iStickCheckingMod = -((MAX_SKATER_STAMINA - m_iStamina) / 1000);
}

GameSkaterStats* GameSkater::GetGameStats() const { return pGameStats; }

// Player takes a shot on pGoalie. Returns the shot he took, which has all the information on it
Shot* GameSkater::TakeShot(int iLength, int iWidth, Goalie* pGoalie)
{
	int iDistance = (int)std::sqrt(double(iLength * iLength + iWidth * iWidth));

	// Calculate a random shot type.
	// Weighted on distance from net, because certain types are really hard to hit the net with
	int iShotTypeWeights[NUM_SHOT_TYPES];
	iShotTypeWeights[SHOT_TYPE_BACKHAND] = 100 * (-(2 * iDistance) + 100);
	iShotTypeWeights[SHOT_TYPE_FOREHAND] = 100 * (-(1 * iDistance) + 100);
	iShotTypeWeights[SHOT_TYPE_SLAPSHOT] = 100 * (-(iDistance / 2) + 100);

	ShotType eShotType = (ShotType)CORE.GetRandom().WeightedRandom(iShotTypeWeights, NUM_SHOT_TYPES);
	ShotSelectionType eShotSelectionType = (ShotSelectionType)CORE.GetRandom().GetRandInt(NUM_SHOT_SELECTION_TYPES);

	// Create a new shot from a random part of the zone
	Shot* pShot = new Shot(this, pGoalie, eShotType, eShotSelectionType, iLength, iWidth);

	pShot->CalculateHitNet();
	pShot->CalculateScore();		// does nothing if hit net failed

	return pShot;
}

// Is the player currently on the ice?
bool GameSkater::IsOnIce() const
{
	return m_bOnIce;
}

// Sets the player to be on/off the ice
void GameSkater::SetOnIce(bool bValue)
{
	m_bOnIce = bValue;
}

// Return the current time of the shift
int GameSkater::GetShiftTime() const
{
	return m_iShiftTime;
}

void GameSkater::ChangeShiftTime(int iChange)
{
	SetShiftTime(m_iShiftTime + iChange);
}

void GameSkater::SetShiftTime(int iValue)
{
	m_iShiftTime = iValue;
}

// Skating
int GameSkater::GetAcceleration() const{ return std::max(pSkater->GetAcceleration() + m_iAccelerationMod, 0); }
int GameSkater::GetAgility() const{ return std::max(pSkater->GetAgility() + m_iAgilityMod, 0); }
int GameSkater::GetBalance() const{ return std::max(pSkater->GetBalance() + m_iBalanceMod, 0); }
int GameSkater::GetEndurance() const{ return std::max(pSkater->GetEndurance() + m_iEnduranceMod, 0); }
int GameSkater::GetSpeed() const{ return std::max(pSkater->GetSpeed() + m_iSpeedMod, 0); }

// Shooting
int GameSkater::GetAccuracyBackhand() const{ return std::max(pSkater->GetAccuracyBackhand() + m_iAccuracyBackhandMod, 0); }
int GameSkater::GetAccuracyForehand() const{ return std::max(pSkater->GetAccuracyForehand() + m_iAccuracyForehandMod, 0); }
int GameSkater::GetAccuracySlapshot() const{ return std::max(pSkater->GetAccuracySlapshot() + m_iAccuracySlapshotMod, 0); }
int GameSkater::GetPowerBackhand() const{ return std::max(pSkater->GetPowerBackhand() + m_iPowerBackhandMod, 0); }
int GameSkater::GetPowerForehand() const{ return std::max(pSkater->GetPowerForehand() + m_iPowerForehandMod, 0); }
int GameSkater::GetPowerSlapshot() const{ return std::max(pSkater->GetPowerSlapshot() + m_iPowerSlapshotMod, 0); }
int GameSkater::GetShotConfidence() const{ return std::max(pSkater->GetShotConfidence() + m_iShotConfidenceMod, 0); }

// Physical
int GameSkater::GetAggressiveness() const{ return std::max(pSkater->GetAggressiveness() + m_iAggressivenessMod, 0); }
int GameSkater::GetBodyChecking() const{ return std::max(pSkater->GetBodyChecking() + m_iBodyCheckingMod, 0); }
int GameSkater::GetDurability() const{ return std::max(pSkater->GetDurability() + m_iDurabilityMod, 0); }
int GameSkater::GetFighting() const{ return std::max(pSkater->GetFighting() + m_iFightingMod, 0); }
int GameSkater::GetStrength() const{ return std::max(pSkater->GetStrength() + m_iStrengthMod, 0); }

// Puck Skills
int GameSkater::GetDeking() const{ return std::max(pSkater->GetDeking() + m_iDekingMod, 0); }
int GameSkater::GetHandEye() const{ return std::max(pSkater->GetHandEye() + m_iHandEyeMod, 0); }
int GameSkater::GetPassing() const{ return std::max(pSkater->GetPassing() + m_iPassingMod, 0); }
int GameSkater::GetPuckControl() const{ return std::max(pSkater->GetPuckControl() + m_iPuckControlMod, 0); }
int GameSkater::GetVision() const{ return std::max(pSkater->GetVision() + m_iVisionMod, 0); }

// Defense
int GameSkater::GetDiscipline() const{ return std::max(pSkater->GetDiscipline() + m_iDisciplineMod, 0); }
int GameSkater::GetFaceoff() const{ return std::max(pSkater->GetFaceoff() + m_iFaceoffMod, 0); }
int GameSkater::GetPositioning() const{ return std::max(pSkater->GetPositioning() + m_iPositioningMod, 0); }
int GameSkater::GetShotBlocking() const{ return std::max(pSkater->GetShotBlocking() + m_iShotBlockingMod, 0); }
int GameSkater::GetStickChecking() const{ return std::max(pSkater->GetStickChecking() + m_iStickCheckingMod, 0); }

Skater* GameSkater::GetSkater() const { return pSkater; }