#include "stdafx.h"

Shot::Shot(GameSkater* pSkater, Goalie* pGoalie, ShotType eShotType, ShotSelectionType eShotSelectionType, int iLengthFromNet, int iWidthFromNet) :
	pSkater(pSkater),
	pGoalie(pGoalie),
	m_eShotType(eShotType),
	m_eShotSelectionType(eShotSelectionType),
	m_iLengthFromNet(iLengthFromNet),
	m_iWidthFromNet(iWidthFromNet),
	m_bHitNet(false),
	m_bScored(false),
	m_iShotSpeed(0),
	m_dShotAngle(0.0),
	m_dShotDistance(0.0),
	m_iShotRating(0),
	m_iPlayerPowerRating(0),
	m_iPlayerAccuracyRating(0)
{
	// Calculate the shot modifier

	// Use the distance formula to calculate actual distance from the net
	m_dShotDistance = sqrt(pow(m_iLengthFromNet, 2) + pow(m_iWidthFromNet, 2));
	
	// Calculate the angle of the shot from the net.
	m_dShotAngle = m_iWidthFromNet == 0 ? M_PI_2 : atan(m_iLengthFromNet / (double)m_iWidthFromNet);


	switch (m_eShotType)
	{
	case SHOT_TYPE_BACKHAND: m_iPlayerAccuracyRating = pSkater->GetAccuracyBackhand(); m_iPlayerPowerRating = pSkater->GetPowerBackhand(); break;
	case SHOT_TYPE_FOREHAND: m_iPlayerAccuracyRating = pSkater->GetAccuracyForehand(); m_iPlayerPowerRating = pSkater->GetPowerForehand(); break;
	case SHOT_TYPE_SLAPSHOT: m_iPlayerAccuracyRating = pSkater->GetAccuracySlapshot();  m_iPlayerPowerRating = pSkater->GetPowerSlapshot(); break;
	}

	m_iPlayerAccuracyRating;
	m_iPlayerPowerRating;

	int iSpeedMinimum = 0;
	int iRange = 0;

	switch (m_eShotType)
	{
	case SHOT_TYPE_BACKHAND: iRange = 35; iSpeedMinimum = 15; break;
	case SHOT_TYPE_FOREHAND: iRange = 55; iSpeedMinimum = 20; break;
	case SHOT_TYPE_SLAPSHOT: iRange = 50; iSpeedMinimum = 50; break;
	}

	// Calculate the speed of the shot
	int iSpeedMiddle = iRange * pow(m_iPlayerPowerRating / 100.0, 2) + iSpeedMinimum;
	m_iShotSpeed = std::max(10, iSpeedMiddle + CORE.GetRandom().GetRandInt(-10, 10));

	// Calculate shot rating
	//CalculateShotRating();
}


Shot::~Shot()
{
}

// Calculate whether or not the shot hit the net
void Shot::CalculateHitNet()
{
	// Calculate a modifier of the shot accuracy based on the distance to the net
	// The formula is 1 / e^(distance / divisor)
	int iDistanceModDivisor;

	// TODO: Shot type definitions
	switch (m_eShotType)
	{
		case SHOT_TYPE_BACKHAND: iDistanceModDivisor = 18; break;
		case SHOT_TYPE_FOREHAND: iDistanceModDivisor = 64; break;
		case SHOT_TYPE_SLAPSHOT: iDistanceModDivisor = 68; break;
		default: std::cout << "Invalid shot type!" << std::endl; return;
	}

	int iDistanceMod = (1 / exp(m_dShotDistance / iDistanceModDivisor) * 100);

	// ex: 1 / exp(25 / 50) * 100 = 60

	// Calculate the modifier to hit the net based on the player's accuracy ratings, multipled by 100
	int iAccuracyRatingMod = (pow(double(m_iPlayerAccuracyRating / 120.0), 1.8) + 0.27) * 100;

	// ex: (((90 / 120) ^ 1.6) + 0.05) * 100 = 68

	// Base chance of hitting the net, unmodified by angle. This is multiplied by 100
	int iAccuracy = iAccuracyRatingMod * iDistanceMod;
	// ex: 60 * 93 = 5580

	// Calculate an angle modifier, times 100.
	int iAngleMod = 10000 * (sin(m_dShotAngle - M_PI_2) + 2);

	// ex: 10000 * (sin(.158655262 - pi/2) + 1) = 125
	int iPercentAngleModifies = 40;	// how much the angle will have an effect

	// Divided by 10,000 to make up for all the multiplied by hundreds above
	iAccuracy = iAccuracy - ((iAccuracy * ((10000 - iAngleMod) / 100)) * iPercentAngleModifies / 10000);
	iAccuracy /= 100;

	// as of now, iAccuracy should be something like 28 or something
	// And refers to the percentage that we actually hit the net
	// The end result looks like something like this: 
		// far away shots almost never hit the net
		// Harder to hit the net on sharp angle tries


	int iRoll = CORE.GetRandom().GetRandInt(100);
	bool bHitNet = iRoll < iAccuracy ? true : false;
	
	/*
	std::string szShotType = m_eShotType == SHOT_TYPE_BACKHAND ? "Backhand" : (m_eShotType == SHOT_TYPE_FOREHAND ? "Forehand" : "Slapshot");
	std::cout << "Shot attempt: " << pSkater->m_szFirst << " " << pSkater->m_szLast << std::endl;
	std::cout << "(Type: " << szShotType << " Accuracy: " << iAccuracy << "%, Rating: " << m_iShotRating << ", Spd: " << m_iShotSpeed << " mph, " << m_dShotDistance << " ft)" << std::endl;
	*/

	m_bHitNet = bHitNet;
}

// Calculates a shot rating based on shot power and accuracy ratings
/*
void Shot::CalculateShotRating()
{
	// Get the absolute maximum accuracy modifier, based on ratings.
	int iAccuracyMod = 5 * pow(m_iPlayerAccuracyRating, 2);
	iAccuracyMod /= 10;

	// this will be a number from 0 - 5000, if the rating was 100
	iAccuracyMod = CORE.GetRandom().GetRandInt(0, iAccuracyMod);

	// Take the numbers from 0 - 5000 and arrange them so they fit exponentially from 0 - 100
	int iMaxMod = (5 * pow(100, 2)) / 10;
	double dPower = pow(iAccuracyMod / (double)iMaxMod, 2);
	iAccuracyMod = 100 * dPower;

	// Calculate an angle modifier, times 100.
	int iAngleMod = 10000 * (sin(m_dShotAngle - M_PI_2) + 1);
	int iPercentAngleModifies = 85;	// how much the angle will have an effect

	int iPowerMod = m_iShotSpeed * 100 / 110;
	int iAverage = ((iAccuracyMod * iPercentAngleModifies / 100) + iPowerMod) / 2;

	m_iShotRating = iAverage;
}
*/

// Calculates whether or not the shot was scored
void Shot::CalculateScore()
{
	if (!DidHitNet()) return;

	int iGoalieRating;
	switch (m_eShotSelectionType)
	{
	case SHOT_SELECTION_FIVE_HOLE:	iGoalieRating = pGoalie->GetFiveHole(); break;
	case SHOT_SELECTION_GLOVE_HIGH: iGoalieRating = pGoalie->GetGloveHigh();  break;
	case SHOT_SELECTION_GLOVE_LOW:	iGoalieRating = pGoalie->GetGloveLow(); break;
	case SHOT_SELECTION_STICK_HIGH: iGoalieRating = pGoalie->GetStickHigh(); break;
	case SHOT_SELECTION_STICK_LOW:	iGoalieRating = pGoalie->GetStickLow(); break;
	case SHOT_SELECTION_UPPER_NET:	iGoalieRating = pGoalie->GetPositioning(); break;
	default:
		return;
	}
	// to-do: revise?
	int iBaseChanceToSaveShot = 93;
	int iSpeedMod = m_iShotSpeed + 15;

	int iChanceToScore = ((10000 - (iGoalieRating * iBaseChanceToSaveShot)) / 100);
	iChanceToScore *= (iSpeedMod * 90 / 100);
	iChanceToScore /= 100;

	int iRoll = CORE.GetRandom().GetRandInt(100);
	bool bScored = iRoll < iChanceToScore ? true : false;
	m_bScored = bScored;
}

// Get the shot's length from the net
int Shot::GetLengthFromNet() const
{
	return m_iLengthFromNet;
}

// Get the shot's width from the net
int Shot::GetWidthFromNet() const
{
	return m_iWidthFromNet;
}

// Did this shot hit the net??
bool Shot::DidHitNet() const
{
	return m_bHitNet;
}

// Did this shot score?
bool Shot::DidScore() const
{
	return m_bScored;
}

ShotType Shot::GetShotType() const
{
	return m_eShotType;
}

ShotSelectionType Shot::GetShotSelectionType() const
{
	return m_eShotSelectionType;
}