#include "stdafx.h"
#include "Infos.h"

BaseInfo::BaseInfo() :
	m_szFirst(""),
	m_szLast(""),
	id(-1),
	teamtype_(NO_TEAM)
{
}

BaseInfo::~BaseInfo()
{

}

TeamType BaseInfo::GetTeam() const
{
	return teamtype_;
}

int BaseInfo::GetID() const
{
	return id;
}

std::string BaseInfo::GetLastName() const
{
	return m_szLast;
}
std::string BaseInfo::GetFirstName() const
{
	return m_szFirst;
}

bool BaseInfo::CacheResults(Database::DatabaseResults* pResults)
{
	if (pResults == NULL)
		return false;

	id = pResults->GetInt("ID");

	m_szFirst = pResults->GetText("First");
	m_szLast = pResults->GetText("Last");

	teamtype_ = (TeamType) CORE.GetDatabase().FindIndexTypeInTable(pResults->GetText("Team"), "Teams");

	return true;
}

SkaterInfo::SkaterInfo():
	BaseInfo(),
	m_iAcceleration(0),
	m_iAgility(0),
	m_iBalance(0),
	m_iEndurance(0),
	m_iSpeed(0),

	m_iAccuracyBackhand(0),
	m_iAccuracyForehand(0),
	m_iAccuracySlapshot(0),
	m_iPowerBackhand(0),
	m_iPowerForehand(0),
	m_iPowerSlapshot(0),

	m_iAggressiveness(0),
	m_iBodyChecking(0),
	m_iDurability(0),
	m_iFighting(0),
	m_iStrength(0),

	m_iDeking(0),
	m_iHandEye(0),
	m_iPassing(0),
	m_iPuckControl(0),
	m_iVision(0),

	m_iDiscipline(0),
	m_iFaceoff(0),
	m_iPositioning(0),
	m_iShotBlocking(0),
	m_iStickChecking(0)
{
}


SkaterInfo::~SkaterInfo()
{
}

bool SkaterInfo::CacheResults(Database::DatabaseResults* pResults)
{
	if (pResults == NULL)
		return false;
	
	// Load the base info first - it's a superclass so this is legal
	if (!BaseInfo::CacheResults(pResults))
		return false;

	std::string temp = pResults->GetText("Position");
	if (temp == "LW")		position_ = POSITION_TYPE_LEFT_WING;
	else if (temp == "RW")	position_ = POSITION_TYPE_RIGHT_WING;
	else if (temp == "C")	position_ = POSITION_TYPE_CENTER;
	else if (temp == "D")	position_ = POSITION_TYPE_DEFENSE;
	else std::cout << "Invalid Position type!" << std::endl;

	m_iAcceleration = pResults->GetInt("ACC");
	m_iAgility = pResults->GetInt("AGI");
	m_iBalance = pResults->GetInt("BAL");
	m_iEndurance = pResults->GetInt("END");
	m_iSpeed = pResults->GetInt("SPD");

	m_iAccuracyBackhand = pResults->GetInt("BH ACC");
	m_iAccuracyForehand = pResults->GetInt("FH ACC");
	m_iAccuracySlapshot = pResults->GetInt("SS ACC");
	m_iPowerBackhand = pResults->GetInt("BH PWR");
	m_iPowerForehand = pResults->GetInt("FH PWR");
	m_iPowerSlapshot = pResults->GetInt("SS PWR");
	m_iShotConfidence = pResults->GetInt("SHT CFD");

	m_iAggressiveness = pResults->GetInt("AGG");
	m_iBodyChecking = pResults->GetInt("BDY CHK");
	m_iDurability = pResults->GetInt("DUR");
	m_iFighting = pResults->GetInt("FIGHT");
	m_iStrength = pResults->GetInt("STR");

	m_iDeking = pResults->GetInt("DEK");
	m_iHandEye = pResults->GetInt("HNDEYE");
	m_iPassing = pResults->GetInt("PASS");
	m_iPuckControl = pResults->GetInt("PCK CTL");
	m_iVision = pResults->GetInt("VIS");

	m_iDiscipline = pResults->GetInt("DSC");
	m_iFaceoff = pResults->GetInt("FO");
	m_iPositioning = pResults->GetInt("POS");
	m_iShotBlocking = pResults->GetInt("SH BLK");
	m_iStickChecking = pResults->GetInt("STK CHK");

	return true;
}

PositionType SkaterInfo::GetPosition() const { return position_; }

// Skating
int SkaterInfo::GetAcceleration() const{ return m_iAcceleration; }
int SkaterInfo::GetAgility() const{ return m_iAgility; }
int SkaterInfo::GetBalance() const{ return m_iBalance; }
int SkaterInfo::GetEndurance() const{ return m_iEndurance; }
int SkaterInfo::GetSpeed() const{ return m_iSpeed; }

// Shooting
int SkaterInfo::GetAccuracyBackhand() const{ return m_iAccuracyBackhand; }
int SkaterInfo::GetAccuracyForehand() const{ return m_iAccuracyForehand; }
int SkaterInfo::GetAccuracySlapshot() const{ return m_iAccuracySlapshot; }
int SkaterInfo::GetPowerBackhand() const{ return m_iPowerBackhand; }
int SkaterInfo::GetPowerForehand() const{ return m_iPowerForehand; }
int SkaterInfo::GetPowerSlapshot() const{ return m_iPowerSlapshot; }
int SkaterInfo::GetShotConfidence() const{ return m_iShotConfidence; }

// Physical
int SkaterInfo::GetAggressiveness() const{ return m_iAggressiveness; }
int SkaterInfo::GetBodyChecking() const{ return m_iBodyChecking; }
int SkaterInfo::GetDurability() const{ return m_iDurability; }
int SkaterInfo::GetFighting() const{ return m_iFighting; }
int SkaterInfo::GetStrength() const{ return m_iStrength; }

// Puck Skills
int SkaterInfo::GetDeking() const{ return m_iDeking; }
int SkaterInfo::GetHandEye() const{ return m_iHandEye; }
int SkaterInfo::GetPassing() const{ return m_iPassing; }
int SkaterInfo::GetPuckControl() const{ return m_iPuckControl; }
int SkaterInfo::GetVision() const{ return m_iVision; }

// Defense
int SkaterInfo::GetDiscipline() const{ return m_iDiscipline; }
int SkaterInfo::GetFaceoff() const{ return m_iFaceoff; }
int SkaterInfo::GetPositioning() const{ return m_iPositioning; }
int SkaterInfo::GetShotBlocking() const{ return m_iShotBlocking; }
int SkaterInfo::GetStickChecking() const{ return m_iStickChecking; }

////////////////////////
// GoalieInfo
////////////////////////

GoalieInfo::GoalieInfo():
	m_iGloveHigh(0),
	m_iGloveLow(0),
	m_iStickHigh(0),
	m_iStickLow(0),
	m_iFiveHole(0),
	m_iPositioning(0),
	m_iPassing(0),
	m_iPokeCheck(0),
	m_iReboundControl(0),
	m_iShotRecovery(0),
	m_iStickHandling(0),
	m_iAggressiveness(0),
	m_iAgility(0),
	m_iDurability(0),
	m_iEndurance(0),
	m_iFlexibility(0),
	m_iVision(0)
{

}

GoalieInfo::~GoalieInfo()
{

}

bool GoalieInfo::CacheResults(Database::DatabaseResults* pResults)
{
	if (pResults == NULL)
		return false;

	if (!BaseInfo::CacheResults(pResults))
		return false;

	m_iGloveHigh = pResults->GetInt("GLVHI");
	m_iGloveLow = pResults->GetInt("GLVLO");
	m_iStickHigh = pResults->GetInt("STKHI");
	m_iStickLow = pResults->GetInt("STKLO");
	m_iFiveHole = pResults->GetInt("FV HL");
	m_iPositioning = pResults->GetInt("POS");

	m_iPassing = pResults->GetInt("PASS");
	m_iPokeCheck = pResults->GetInt("PK CHK");
	m_iReboundControl = pResults->GetInt("RB CTL");
	m_iShotRecovery = pResults->GetInt("SH RCV");
	m_iStickHandling = pResults->GetInt("STK HN");

	m_iAggressiveness = pResults->GetInt("AGG");
	m_iAgility = pResults->GetInt("AGI");
	m_iDurability = pResults->GetInt("DUR");
	m_iEndurance = pResults->GetInt("END");
	m_iFlexibility = pResults->GetInt("FLX");
	m_iVision = pResults->GetInt("VIS");

	return true;
}

// Reflexes
int GoalieInfo::GetGloveHigh() const { return m_iGloveHigh; }
int GoalieInfo::GetGloveLow() const { return m_iGloveLow; }
int GoalieInfo::GetStickHigh() const { return m_iStickHigh; }
int GoalieInfo::GetStickLow() const { return m_iStickLow; }
int GoalieInfo::GetFiveHole() const { return m_iFiveHole; }
int GoalieInfo::GetPositioning() const { return m_iPositioning; }

// Puck Control
int GoalieInfo::GetPassing() const { return m_iPassing; }
int GoalieInfo::GetPokeCheck() const { return m_iPokeCheck; }
int GoalieInfo::GetReboundControl() const { return m_iReboundControl; }
int GoalieInfo::GetShotRecovery() const { return m_iShotRecovery; }
int GoalieInfo::GetStickHandling() const { return m_iStickHandling; }

// Physical
int GoalieInfo::GetAggressiveness() const { return m_iAggressiveness; }
int GoalieInfo::GetAgility() const { return m_iAgility; }
int GoalieInfo::GetDurability() const { return m_iDurability; }
int GoalieInfo::GetEndurance() const { return m_iEndurance; }
int GoalieInfo::GetFlexibility() const { return m_iFlexibility; }
int GoalieInfo::GetVision() const { return m_iVision; }


TeamInfo::TeamInfo():
	id(),
	city(),
	name(),
	adjective(),
	abbreviation()
{

}
TeamInfo::~TeamInfo()
{

}

bool TeamInfo::CacheResults(Database::DatabaseResults* pResults)
{
	if (pResults == NULL)
		return false;

	id = pResults->GetInt("ID");
	city = pResults->GetText("City");
	name = pResults->GetText("Name");
	adjective = pResults->GetText("Adjective");
	abbreviation = pResults->GetText("Abbreviation");

	return true;
}

int TeamInfo::GetID() const
{
	return id;
}

std::string TeamInfo::GetCity() const
{
	return city;
}
std::string TeamInfo::GetName() const
{
	return name;
}
std::string TeamInfo::GetAdjective() const
{
	return adjective;
}
std::string TeamInfo::GetAbbreviation() const
{
	return abbreviation;
}