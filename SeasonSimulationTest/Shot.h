#pragma once

#include "Player.h"
#include "enums.h"

class Shot
{
public:
	Shot(GameSkater* pSkater, Goalie* pGoalie, ShotType eShotType, ShotSelectionType eShotSelectionType, int iLengthFromNet, int iWidthFromNet);
	~Shot();

	ShotType GetShotType() const;
	ShotSelectionType GetShotSelectionType() const;

	void CalculateScore();
	void CalculateHitNet();

	int GetLengthFromNet() const;
	int GetWidthFromNet() const;
	bool DidHitNet() const;
	bool DidScore() const;
private:
	void CalculateShotRating();

	GameSkater* pSkater;
	Goalie* pGoalie;

	ShotType m_eShotType;
	ShotSelectionType m_eShotSelectionType;

	int m_iShotRating;
	int m_iPlayerAccuracyRating;
	int m_iPlayerPowerRating;

	int m_iShotSpeed;
	double m_dShotAngle;
	double m_dShotDistance;
	int m_iLengthFromNet;
	int m_iWidthFromNet;

	bool m_bHitNet;
	bool m_bScored;
};

