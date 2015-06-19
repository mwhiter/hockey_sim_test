#include "stdafx.h"

uint Game::uiGameIDs = 0;

/////////////////////////////////////////////
// PERIOD
/////////////////////////////////////////////

Period::Period(Game* game, Period::PeriodType ePeriodType, int minutes) :
	m_pGame(game),
	ePeriodType(ePeriodType),
	m_iSecondsLeft(minutes * 60),
	SkaterStats(),
	HomeStats(),
	AwayStats(),
	m_pCurrentPossessingTeam(NULL),
	m_eCurrentZone(ZONE_TYPE_NEUTRAL),
	iHomeForwardLine(0),
	iHomeDefenseLine(0),
	iAwayForwardLine(0),
	iAwayDefenseLine(0)
{
	// Fill up the game stats for skaters based on all active skaters
	GameRoster* pHomeRoster = m_pGame->GetHomeRoster();
	GameRoster* pAwayRoster = m_pGame->GetAwayRoster();

	for (int i = 0; i < NUM_LINES_FORWARDS; i++)
	{
		for (int iForward = 0; iForward < NUM_FORWARDS_PER_LINE; iForward++)
		{
			SkaterStats.push_back(std::pair<GameSkater*, PeriodSkaterStats>(pHomeRoster->GetForwardLine(i)->GetForward(iForward), PeriodSkaterStats()));
			SkaterStats.push_back(std::pair<GameSkater*, PeriodSkaterStats>(pAwayRoster->GetForwardLine(i)->GetForward(iForward), PeriodSkaterStats()));
		}
	}

	for (int i = 0; i < NUM_LINES_DEFENSEMEN; i++)
	{
		for (int iDefenseman = 0; iDefenseman < NUM_DEFENSEMEN_PER_LINE; iDefenseman++)
		{
			SkaterStats.push_back(std::pair<GameSkater*, PeriodSkaterStats>(pHomeRoster->GetDefenseLine(i)->GetDefenseman(iDefenseman), PeriodSkaterStats()));
			SkaterStats.push_back(std::pair<GameSkater*, PeriodSkaterStats>(pAwayRoster->GetDefenseLine(i)->GetDefenseman(iDefenseman), PeriodSkaterStats()));
		}
	}

	// Initial setting for players
	for (int i = 0; i < NUM_FORWARDS_PER_LINE; i++)
	{
		GetGame()->GetAwayRoster()->GetForwardLine(iAwayForwardLine)->GetForward(i)->SetOnIce(true);
		GetGame()->GetHomeRoster()->GetForwardLine(iHomeForwardLine)->GetForward(i)->SetOnIce(true);
	}

	for (int i = 0; i < NUM_DEFENSEMEN_PER_LINE; i++)
	{
		GetGame()->GetAwayRoster()->GetDefenseLine(iAwayDefenseLine)->GetDefenseman(i)->SetOnIce(true);
		GetGame()->GetHomeRoster()->GetDefenseLine(iHomeDefenseLine)->GetDefenseman(i)->SetOnIce(true);
	}

	// Update the on-ice skill difference
	UpdateSkillDifference();
}
Period::~Period()
{
}

// Simulate a period
void Period::Simulate()
{
	if (ePeriodType == Period::PERIOD_TYPE_REGULATION || ePeriodType == Period::PERIOD_TYPE_OVERTIME)
	{
		// Initial faceoff testing
		m_pCurrentPossessingTeam = SimulateFaceoff(FACEOFF_ZONE_NEUTRAL_HOME);

		while (m_iSecondsLeft > 0)
		{
			m_pCurrentPossessingTeam = TestPossessionChange();
			m_eCurrentZone = TestZoneEntry();
			m_eCurrentZone = TestClear();

			// Penalties can occur in any zone
			TestPenalty();

			// Stuff that occurs only in the neutral zone
			if (m_eCurrentZone == ZONE_TYPE_NEUTRAL)
			{
				TestForwardLineShift(GetGame()->GetHomeTeam());
				TestForwardLineShift(GetGame()->GetAwayTeam());
			}
			// Stuff that occurs only in a team's home
			else if (m_eCurrentZone == ZONE_TYPE_HOME)
			{
				// Must have possession in order to take a shot
				if (m_pCurrentPossessingTeam == GetGame()->GetAwayTeam())
				{
					TestShot(GetGame()->GetAwayRoster(), GetGame()->GetHomeRoster());
					AwayStats.iTimeOnAttack++;
				}

				TestDefenseLineShift(GetGame()->GetAwayTeam());
			}
			// Stuff that occurs only in a team's home
			else if (m_eCurrentZone == ZONE_TYPE_AWAY)
			{
				// Must have possession in order to take a shot
				if (m_pCurrentPossessingTeam == GetGame()->GetHomeTeam())
				{
					TestShot(GetGame()->GetHomeRoster(), GetGame()->GetAwayRoster());
					HomeStats.iTimeOnAttack++;
				}
				
				TestDefenseLineShift(GetGame()->GetHomeTeam());
			}

			// Always decrement number of seconds left in the period
			m_iSecondsLeft--;

			// Loop through all the players
			std::vector<std::pair<GameSkater*, PeriodSkaterStats>>::iterator it;
			for (it = SkaterStats.begin(); it != SkaterStats.end(); it++)
			{
				// Player is on the ice
				if (it->first->IsOnIce())
				{
					it->first->ChangeShiftTime(1);
					it->second.iTOI++;
				}

				CalculateStaminaChange(it->first);
			}
		}
	}
	else
	{
		CORE.GetRandom().GetRandBool() ? AwayStats.iGoals = 1 : HomeStats.iGoals = 1;
	}

	return;
}

// Calculates stamina drain for player
void Period::CalculateStaminaChange(GameSkater* pSkater)
{
	if (pSkater == NULL) return;

	// On ice - drain stamina
	if (pSkater->IsOnIce())
	{
		int iStaminaDrainBase = 75;
		int iEnduranceModBase = 300;
		int iOffensiveZoneDrainMod = 75;
		int iDefensiveZoneDrainMod = 125;
		int iNeutralZoneDrainMod = 100;
		int iDefenderMod = 75;

		int iDurabilityMod = 100 - pSkater->GetSkater()->GetEndurance();
		int iSpeedMod = (pSkater->GetSkater()->GetSpeed() + pSkater->GetSkater()->GetAcceleration()) / 2;

		int iDurModWeight = 75;
		int iSpdModWeight = 25;

		int iTotalMod = (iDurabilityMod*iDurModWeight + iSpeedMod*iSpdModWeight) / iDurModWeight + iSpdModWeight;

		int iDrain = iStaminaDrainBase + (iEnduranceModBase * iTotalMod / 100);
		if (m_eCurrentZone == ZONE_TYPE_NEUTRAL)
		{
			iDrain *= iNeutralZoneDrainMod;
			iDrain /= 100;
		}
		else if (m_eCurrentZone == ZONE_TYPE_HOME)
		{
			iDrain *= (pSkater->GetSkater()->GetTeam() == GetGame()->GetHomeTeam() ? iDefensiveZoneDrainMod : iOffensiveZoneDrainMod);
			iDrain /= 100;
		}
		else if (m_eCurrentZone == ZONE_TYPE_AWAY)
		{
			iDrain *= (pSkater->GetSkater()->GetTeam() == GetGame()->GetAwayTeam() ? iDefensiveZoneDrainMod : iOffensiveZoneDrainMod);
			iDrain /= 100;
		}

		pSkater->ChangeStamina(-iDrain);
	}
	// Off ice - recover stamina
	else
	{
		// todo: don't hardcode
		int iStaminaRecovery = 125;
		pSkater->ChangeStamina(iStaminaRecovery);
	}
}

// Test whether or not a successful zone entry occured. Returns the zone type we should be in
ZoneType Period::TestZoneEntry()
{
	// If the current zone isn't the neutral zone, we can't enter another zone, so just return the zone we're in.
	if (m_eCurrentZone != ZONE_TYPE_NEUTRAL) return m_eCurrentZone;

	int iChanceToEnterAway = 0, iChanceToEnterHome = 0;
	// Who has possession of the puck?
	if (m_pCurrentPossessingTeam == GetGame()->GetHomeTeam())
	{
		iChanceToEnterAway = 1000;
		iChanceToEnterHome = 200;
	}
	else if (m_pCurrentPossessingTeam == GetGame()->GetAwayTeam())
	{
		iChanceToEnterAway = 200;
		iChanceToEnterHome = 1000;
	}

	// Test whether or not we've entered the zone.
	bool bEnterAway = CORE.GetRandom().GetRandInt(10000) < iChanceToEnterAway;
	bool bEnterHome = CORE.GetRandom().GetRandInt(10000) < iChanceToEnterHome;

	// Both teams want to enter the zone at the moment
	if (bEnterHome && bEnterAway)
	{
		int iHomeChance = 50;
		if (CORE.GetRandom().GetRandInt(100) < iHomeChance)
		{
			bEnterHome = true;
			bEnterAway = false;
		}
		else
		{
			bEnterHome = false;
			bEnterAway = true;
		}
	}

	// Enter the away zone
	if (bEnterAway)
	{
		TestForwardLineShift(GetGame()->GetHomeTeam());
		TestDefenseLineShift(GetGame()->GetHomeTeam());
		//std::cout << m_iSecondsLeft << ": ENTER: Away zone!" << std::endl;
		return ZONE_TYPE_AWAY;
	}
	// Enter the home zone
	if (bEnterHome)
	{
		TestForwardLineShift(GetGame()->GetAwayTeam());
		TestDefenseLineShift(GetGame()->GetAwayTeam());
		//std::cout << m_iSecondsLeft << ": ENTER: Home zone!" << std::endl;
		return ZONE_TYPE_HOME;
	}

	// Stay in current zone.
	return m_eCurrentZone;
}

// Test whether pTeam should change their forwards
void Period::TestForwardLineShift(Team* pTeam)
{
	if (pTeam == NULL) return;
	if (!GetGame()->IsPlayingInGame(pTeam)) return;

	bool bMakeChange = false;
	GameRoster* pGameRoster = NULL;

	int iCurrentLine = -1;
	int iExpectedShiftLength = -1;

	if (IsHomeTeam(pTeam))
	{
		pGameRoster = GetGame()->GetHomeRoster();
		iCurrentLine = iHomeForwardLine;
	}
	else
	{
		pGameRoster = GetGame()->GetAwayRoster();
		iCurrentLine = iAwayForwardLine;
	}
	int iAverageStamina = 0;
	for (int i = 0; i < NUM_FORWARDS_PER_LINE; i++)
	{
		iAverageStamina += pGameRoster->GetForwardLine(iCurrentLine)->GetForward(i)->GetStamina();
	}
	iAverageStamina /= NUM_FORWARDS_PER_LINE;

	// If we're tired, get off the ice!
	if (iAverageStamina < 4000)
		bMakeChange = true;

	// If our shift is over, get off the ice!
	if (!bMakeChange)
	{
		switch (iCurrentLine)
		{
		case 0: iExpectedShiftLength = 60; break;
		case 1: iExpectedShiftLength = 50; break;
		case 2: iExpectedShiftLength = 40; break;
		case 3: iExpectedShiftLength = 15; break;
		default: std::cout << "Error in testing forward line shift: home!" << std::endl;
		}

		int iAverageShiftTime = 0;
		for (int i = 0; i < NUM_FORWARDS_PER_LINE; i++)
		{
			iAverageShiftTime += pGameRoster->GetForwardLine(iCurrentLine)->GetForward(i)->GetShiftTime();
		}
		iAverageShiftTime /= NUM_FORWARDS_PER_LINE;

		int iShiftLengthThreshold = 15;
		if ((iAverageShiftTime + iShiftLengthThreshold) >= iExpectedShiftLength)
			bMakeChange = true;
	}

	if (bMakeChange)
	{
		int iNewLine = ChooseNewForwardLine(pTeam);
		SetForwardLine(pTeam, iNewLine);
	}
}

// Test whether pTeam should change their defense
void Period::TestDefenseLineShift(Team* pTeam)
{
	if (pTeam == NULL) return;
	if (!GetGame()->IsPlayingInGame(pTeam)) return;

	bool bMakeChange = false;
	GameRoster* pGameRoster = NULL;

	int iCurrentLine = -1;
	int iExpectedShiftLength = -1;

	if (IsHomeTeam(pTeam))
	{
		pGameRoster = GetGame()->GetHomeRoster();
		iCurrentLine = iHomeDefenseLine;
	}
	else
	{
		pGameRoster = GetGame()->GetAwayRoster();
		iCurrentLine = iAwayDefenseLine;
	}
	
	int iAverageStamina = 0;
	for (int i = 0; i < NUM_DEFENSEMEN_PER_LINE; i++)
	{
		iAverageStamina += pGameRoster->GetDefenseLine(iCurrentLine)->GetDefenseman(i)->GetStamina();
	}
	iAverageStamina /= NUM_DEFENSEMEN_PER_LINE;

	// If we're tired, get off the ice!
	if (iAverageStamina < 4000)
		bMakeChange = true;

	// If our shift is over, get off the ice!
	if (!bMakeChange)
	{
		// Get the expected shift length
		switch (iCurrentLine)
		{
		case 0: iExpectedShiftLength = 80; break;
		case 1: iExpectedShiftLength = 50; break;
		case 2: iExpectedShiftLength = 40; break;
		default: std::cout << "Error in testing defense line shift: home!" << std::endl;
		}

		// Calculate the average shift time
		int iAverageShiftTime = 0;
		for (int i = 0; i < NUM_DEFENSEMEN_PER_LINE; i++)
		{
			iAverageShiftTime += pGameRoster->GetDefenseLine(iCurrentLine)->GetDefenseman(i)->GetShiftTime();
		}
		iAverageShiftTime /= NUM_DEFENSEMEN_PER_LINE;

		int iShiftLengthThreshold = 25;
		if ((iAverageShiftTime + iShiftLengthThreshold) >= iExpectedShiftLength)
			bMakeChange = true;
	}

	// If we need to make a change, pick an appropriate line to take the ice
	if (bMakeChange)
	{
		int iNewLine = ChooseNewDefenseLine(pTeam);
		SetDefenseLine(pTeam, iNewLine);
	}
}

// Choose a new forward line for pTeam
int Period::ChooseNewForwardLine(Team* pTeam) const
{
	if (pTeam == NULL) return -1;
	if (!GetGame()->IsPlayingInGame(pTeam)) return -1;
	int iNewLine = IsHomeTeam(pTeam) ? iHomeForwardLine + 1 : iAwayForwardLine + 1;
	return iNewLine % NUM_LINES_FORWARDS;
}

// Choose a new defense line for pTeam
int Period::ChooseNewDefenseLine(Team* pTeam) const
{
	if (pTeam == NULL) return -1;
	if (!GetGame()->IsPlayingInGame(pTeam)) return -1;
	int iNewLine = IsHomeTeam(pTeam) ? iHomeDefenseLine + 1 : iAwayDefenseLine + 1;
	return iNewLine % NUM_LINES_DEFENSEMEN;
}

// Test whether or not there's been a change in possession
Team* Period::TestPossessionChange()
{
	if (m_pCurrentPossessingTeam == NULL) return NULL;

	// Is the currently possessing team the home team?
	bool bIsHomeTeam = IsHomeTeam(m_pCurrentPossessingTeam);

	int iChanceToChangePossession = 300;

	// Change in possession - return the other team!
	if (CORE.GetRandom().GetRandInt(10000) < iChanceToChangePossession)
	{
		return bIsHomeTeam ? GetGame()->GetAwayTeam() : GetGame()->GetHomeTeam();
	}

	return m_pCurrentPossessingTeam;
}

// Test whether or not the defense clears the zone.
ZoneType Period::TestClear()
{
	// Can't clear the neutral zone!
	if (m_eCurrentZone == ZONE_TYPE_NEUTRAL) return m_eCurrentZone;
	
	Team *pAttacker = NULL, *pDefender = NULL;
	if (m_eCurrentZone == ZONE_TYPE_HOME)
	{
		pAttacker = GetGame()->GetAwayTeam();
		pDefender = GetGame()->GetHomeTeam();
	}
	else
	{
		pAttacker = GetGame()->GetHomeTeam();
		pDefender = GetGame()->GetAwayTeam();
	}

	// Calculate chance to clear zone based on stats. For now, hardcoded.
	int iChanceToClear = 220;

	// If the defending team has possession in their own zone, they have very high chance of clearing the puck!
	if (m_pCurrentPossessingTeam == pDefender)
		iChanceToClear = 5000;

	if (CORE.GetRandom().GetRandInt(10000) < iChanceToClear)
	{
		return ZONE_TYPE_NEUTRAL;
	}

	// Don't do anything
	return m_eCurrentZone;
}

// Test whether or not pAttacker took a shot on pDefender
void Period::TestShot(GameRoster* pAttackerRoster, GameRoster* pDefenderRoster)
{
	// Can't take shots from the neutral zone (yet)
	if (m_eCurrentZone == ZONE_TYPE_NEUTRAL) return;

	// Get the skill difference of on-ice players. Because this difference is positive for home adv and negative for away adv, negate it if away team is on the attack.
	int iSkillDifference = pAttackerRoster->GetTeam() == m_pGame->GetHomeTeam() ? GetSkillDifference() : -GetSkillDifference();
	int iBaseChanceOfShot = 450;
	int iChanceOfShot = std::max(0, iBaseChanceOfShot + (iSkillDifference * 25));

	int iRoll = CORE.GetRandom().GetRandInt(10000);
	if (iRoll < iChanceOfShot)
	{
		pAttackerRoster->GetTeam() == m_pGame->GetAwayTeam() ? AwayStats.iShotAttempts++ : HomeStats.iShotAttempts++;

		Goalie* pGoalie = pDefenderRoster->GetGoalie(0)->GetGoalie();
		GameSkater* pShooter = ChooseShooter(pAttackerRoster);
		
		Shot* pShot = NULL;

		int iLength;
		int iWidth;
		// Forward shot
		if (pShooter->GetSkater()->GetPosition() != POSITION_TYPE_DEFENSE)
		{
			// This formula will weight 0 as 100, 42 as 30, according to -((x/5)^2) + 100
			int iLengthWeights[60];
			for (int i = 0; i < 60; i++)
				iLengthWeights[i] = -((i / 6) * (i / 6)) + 100;
			int iWidthWeights[42];
			for (int i = 0; i < 42; i++)
				iWidthWeights[i] = -((i / 5) * (i / 5)) + 100;

			iLength = CORE.GetRandom().WeightedRandom(iLengthWeights, 60);
			iWidth = CORE.GetRandom().WeightedRandom(iWidthWeights, 42);

			pShot = pShooter->TakeShot(iLength, iWidth, pGoalie);
		}
		else
		{
			// This formula will weight 0 as 100, 42 as 30, according to -((x/5)^2) + 100
			int iLengthWeights[60];
			for (int i = 0; i < 60; i++)
				iLengthWeights[i] = i + 40;
			int iWidthWeights[42];
			for (int i = 0; i < 42; i++)
				iWidthWeights[i] = -((i / 5) * (i / 5)) + 100;

			iLength = CORE.GetRandom().WeightedRandom(iLengthWeights, 60);
			iWidth = CORE.GetRandom().WeightedRandom(iWidthWeights, 42);
			pShot = pShooter->TakeShot(iLength, iWidth, pGoalie);
		}

		CORE.GetLogger().SetCurrentFile("GameEvent.csv");

		// Abort shift if we scored a goal.
		bool bHitNet = pShot->DidHitNet();
		bool bScored = pShot->DidScore();

		std::string szString = std::to_string(GetGame()->GetID()) + ",";
		szString += CORE.GetTimeString(GetSecondsLeft()) + ",";
		szString += "Shot Taken, " + pAttackerRoster->GetTeam()->GetInfo()->GetAbbreviation() + ",";
		szString += pShooter->GetSkater()->GetInfo()->GetFirstName() + " " + pShooter->GetSkater()->GetInfo()->GetLastName() + ",";
		szString += std::to_string(pShot->GetLengthFromNet()) + "L," + std::to_string(pShot->GetWidthFromNet()) + "W" + ",";
		szString += (pShot->GetShotType() == SHOT_TYPE_BACKHAND ? "Backhand," : pShot->GetShotType() == SHOT_TYPE_FOREHAND ? "Forehand," : "Slapshot,");
		szString += (bHitNet ? "Hit Net," : "Missed Net,");
		szString += (bScored ? "Scored!" : "Saved!");
		CORE.GetLogger().WriteLogToFile(szString);

		// delete the shot pointer now that we're done with it
		delete pShot;

		std::vector<std::pair<GameSkater*, PeriodSkaterStats>>::iterator shootIt;
		for (shootIt = SkaterStats.begin(); shootIt != SkaterStats.end(); shootIt++)
		{
			if (pShooter == shootIt->first)
				break;
		}

		shootIt->second.iShotAttempts++;

		if (bHitNet)
		{
			IsHomeTeam(pAttackerRoster->GetTeam()) ? HomeStats.iShots++ : AwayStats.iShots++;
			shootIt->second.iShots++;
		}
		if (bScored)
		{
			IsHomeTeam(pAttackerRoster->GetTeam()) ? HomeStats.iGoals++ : AwayStats.iGoals++;
			
			int iNumAssistsWeight[NUM_MAXIMUM_ASSISTS+1] = { 100, 300, 600 };
			int iNumAssists = CORE.GetRandom().WeightedRandom(iNumAssistsWeight, 3);
			GameSkater* pFirstAssist = NULL, *pSecondAssist = NULL;
			for (int i = 0; i < iNumAssists; i++)
			{
				if (i == 0) pFirstAssist = ChooseAssist(pShooter, NULL, pAttackerRoster);
				if (i == 1) pSecondAssist = ChooseAssist(pShooter, pFirstAssist, pAttackerRoster);
			}

			shootIt->second.iGoals++;

			std::vector<std::pair<GameSkater*, PeriodSkaterStats>>::iterator it;
			for (it = SkaterStats.begin(); it != SkaterStats.end(); it++)
			{
				if (pFirstAssist == it->first) it->second.iAssists++;
				if (pSecondAssist == it->first) it->second.iAssists++;
			}

			// Attacker is the away team
			if (pAttackerRoster->GetTeam() == m_pGame->GetAwayTeam())
			{
				for (int i = 0; i < NUM_FORWARDS_PER_LINE; i++)
				{
					pAttackerRoster->GetForwardLine(iAwayForwardLine)->GetForward(i)->GetGameStats()->iPlusMinus++;
					pDefenderRoster->GetForwardLine(iHomeForwardLine)->GetForward(i)->GetGameStats()->iPlusMinus--;
				}
				for (int i = 0; i < NUM_DEFENSEMEN_PER_LINE; i++)
				{
					pAttackerRoster->GetDefenseLine(iAwayDefenseLine)->GetDefenseman(i)->GetGameStats()->iPlusMinus++;
					pDefenderRoster->GetDefenseLine(iHomeDefenseLine)->GetDefenseman(i)->GetGameStats()->iPlusMinus--;
				}
			}
			else
			{
				for (int i = 0; i < NUM_FORWARDS_PER_LINE; i++)
				{
					pAttackerRoster->GetForwardLine(iHomeForwardLine)->GetForward(i)->GetGameStats()->iPlusMinus++;
					pDefenderRoster->GetForwardLine(iAwayForwardLine)->GetForward(i)->GetGameStats()->iPlusMinus--;
				}
				for (int i = 0; i < NUM_DEFENSEMEN_PER_LINE; i++)
				{
					pAttackerRoster->GetDefenseLine(iHomeDefenseLine)->GetDefenseman(i)->GetGameStats()->iPlusMinus++;
					pDefenderRoster->GetDefenseLine(iAwayDefenseLine)->GetDefenseman(i)->GetGameStats()->iPlusMinus--;
				}
			}

			// Back to the neutral zone with us.
			m_eCurrentZone = ZONE_TYPE_NEUTRAL;
			m_pCurrentPossessingTeam = SimulateFaceoff(FACEOFF_ZONE_NEUTRAL_HOME);
			//std::cout << "Goal scored! Returning to neutral zone." << std::endl;
		}
	}
}

// Pick a shooter from pAttacker out of all active skaters on the ice, based on their ratings
GameSkater* Period::ChooseShooter(GameRoster* pAttackerRoster)
{
	if (pAttackerRoster == NULL) return NULL;

	int iForwardLine = pAttackerRoster->GetTeam() == m_pGame->GetAwayTeam() ? iAwayForwardLine : iHomeForwardLine;
	int iDefenseLine = pAttackerRoster->GetTeam() == m_pGame->GetAwayTeam() ? iAwayDefenseLine : iHomeDefenseLine;

	GameSkater* skaters[NUM_FORWARDS_PER_LINE + NUM_DEFENSEMEN_PER_LINE] =
	{
		pAttackerRoster->GetForwardLine(iForwardLine)->GetForward(0),
		pAttackerRoster->GetForwardLine(iForwardLine)->GetForward(1),
		pAttackerRoster->GetForwardLine(iForwardLine)->GetForward(2),
		pAttackerRoster->GetDefenseLine(iDefenseLine)->GetDefenseman(0),
		pAttackerRoster->GetDefenseLine(iDefenseLine)->GetDefenseman(1)
	};

	int weights[NUM_FORWARDS_PER_LINE + NUM_DEFENSEMEN_PER_LINE];
	for (int i = 0; i < NUM_FORWARDS_PER_LINE + NUM_DEFENSEMEN_PER_LINE; i++)
	{
		// todo - don't hardcore this
		weights[i] = 0;
		// wrong.
		weights[i] += skaters[i]->GetSkater()->GetShootingOverall() * 250;
		weights[i] += skaters[i]->GetAgility() * 50;
		weights[i] += skaters[i]->GetBalance() * 50;
		weights[i] += skaters[i]->GetHandEye() * 50;
		weights[i] += skaters[i]->GetPuckControl() * 50;
		weights[i] += skaters[i]->GetStrength() * 100;
		weights[i] += skaters[i]->GetShotConfidence() * 500;
		weights[i] /= (250 + 50 + 50 + 50 + 50 + 100 + 500);

		// Positional weight - Defenders shoot less than Forwards even when on the ice longer
		if (skaters[i]->GetSkater()->GetPosition() == POSITION_TYPE_DEFENSE)
		{
			weights[i] *= 50;
			weights[i] /= 100;
		}
	}

	// Pick a random player from either offense or defense to take a shot
	int iPlayerIndex = CORE.GetRandom().WeightedRandom(weights, NUM_FORWARDS_PER_LINE + NUM_DEFENSEMEN_PER_LINE);
	return skaters[iPlayerIndex];
}

// If a goal was scored, calculate who gets assists
GameSkater* Period::ChooseAssist(GameSkater* pShooter, GameSkater* pFirstAssist, GameRoster* pAttackerRoster)
{
	if (pShooter == NULL)
		return NULL;

	int iForwardLine = pAttackerRoster->GetTeam() == m_pGame->GetAwayTeam() ? iAwayForwardLine : iHomeForwardLine;
	int iDefenseLine = pAttackerRoster->GetTeam() == m_pGame->GetAwayTeam() ? iAwayDefenseLine : iHomeDefenseLine;

	GameSkater* skaters[NUM_FORWARDS_PER_LINE + NUM_DEFENSEMEN_PER_LINE] =
	{ 
		pAttackerRoster->GetForwardLine(iForwardLine)->GetForward(0),
		pAttackerRoster->GetForwardLine(iForwardLine)->GetForward(1),
		pAttackerRoster->GetForwardLine(iForwardLine)->GetForward(2),
		pAttackerRoster->GetDefenseLine(iDefenseLine)->GetDefenseman(0),
		pAttackerRoster->GetDefenseLine(iDefenseLine)->GetDefenseman(1)
	};

	int weights[NUM_FORWARDS_PER_LINE + NUM_DEFENSEMEN_PER_LINE];
	for (int i = 0; i < NUM_FORWARDS_PER_LINE + NUM_DEFENSEMEN_PER_LINE; i++)
	{
		// todo - don't hardcore this
		weights[i] = 0;

		// Don't give any weight to this if the player is a shooter, or we gave an assist out on this goal already
		if (skaters[i] == pShooter) continue;
		if (skaters[i] == pFirstAssist) continue;

		weights[i] += skaters[i]->GetHandEye() * 100;
		weights[i] += skaters[i]->GetPassing() * 400;
		weights[i] += skaters[i]->GetPuckControl() * 100;
		weights[i] += skaters[i]->GetVision() * 400;
		weights[i] /= (100 + 400 + 100 + 400);

		// Positional weight - Defenders will get less assists because they're on the ice longer, more opportunities to get points
		if (skaters[i]->GetSkater()->GetPosition() == POSITION_TYPE_DEFENSE)
		{
			weights[i] *= 75;
			weights[i] /= 100;
		}
	}

	// Pick a random player from either offense or defense to take a shot
	int iPlayerIndex = CORE.GetRandom().WeightedRandom(weights, NUM_FORWARDS_PER_LINE + NUM_DEFENSEMEN_PER_LINE);
	if (skaters[iPlayerIndex] == pShooter) std::cout << "ERROR: Goal scored, gave assist to player who scored!" << std::endl;
	return skaters[iPlayerIndex];
}

// Test whether or not to call a penalty right now
void Period::TestPenalty()
{
	int iChanceToCallPenalty = 20;		// 100 = 1%
	bool bPenaltyCalled = CORE.GetRandom().GetRandInt(10000) < iChanceToCallPenalty;

	if (bPenaltyCalled)
	{
		Team* pPenalizedTeam = CORE.GetRandom().GetRandBool() ? m_pGame->GetHomeTeam() : m_pGame->GetAwayTeam();
		pPenalizedTeam == m_pGame->GetHomeTeam() ? HomeStats.iPIM += 2 : AwayStats.iPIM += 2;
	}
}

// Change forwards for pTeam to iForwardLine
void Period::SetForwardLine(Team* pTeam, int iForwardLine)
{
	if (pTeam == NULL) return;
	if (pTeam != m_pGame->GetHomeTeam() && pTeam != m_pGame->GetAwayTeam()) return;
	if (iForwardLine < 0 || iForwardLine >= NUM_LINES_FORWARDS) return;

	int iOldLine = IsHomeTeam(pTeam) ? iHomeForwardLine : iAwayForwardLine;
	GameRoster* pGameRoster = IsHomeTeam(pTeam) ? GetGame()->GetHomeRoster() : GetGame()->GetAwayRoster();

	// Update players on ice.
	for (int i = 0; i < NUM_FORWARDS_PER_LINE; i++)
	{
		pGameRoster->GetForwardLine(iOldLine)->GetForward(i)->SetOnIce(false);
		pGameRoster->GetForwardLine(iForwardLine)->GetForward(i)->SetOnIce(true);
	}

	IsHomeTeam(pTeam) ? iHomeForwardLine = iForwardLine : iAwayForwardLine = iForwardLine;

	UpdateSkillDifference();
}

// Change defense for pTeam to iDefenseLine
void Period::SetDefenseLine(Team* pTeam, int iDefenseLine)
{
	if (pTeam == NULL) return;
	if (pTeam != m_pGame->GetHomeTeam() && pTeam != m_pGame->GetAwayTeam()) return;
	if (iDefenseLine < 0 || iDefenseLine >= NUM_LINES_DEFENSEMEN) return;

	int iOldLine = IsHomeTeam(pTeam) ? iHomeDefenseLine : iAwayDefenseLine;
	GameRoster* pGameRoster = IsHomeTeam(pTeam) ? GetGame()->GetHomeRoster() : GetGame()->GetAwayRoster();

	// Update players on ice.
	for (int i = 0; i < NUM_DEFENSEMEN_PER_LINE; i++)
	{
		pGameRoster->GetDefenseLine(iOldLine)->GetDefenseman(i)->SetOnIce(false);
		pGameRoster->GetDefenseLine(iDefenseLine)->GetDefenseman(i)->SetOnIce(true);
	}

	IsHomeTeam(pTeam) ? iHomeDefenseLine = iDefenseLine : iAwayDefenseLine = iDefenseLine;

	UpdateSkillDifference();
}

// Updates the skill difference between the teams on the ice (0 = no diff, + = advantage home, - = advantage away)
void Period::UpdateSkillDifference()
{
	int iHomeScore = 0;
	int iAwayScore = 0;

	GameSkater *pHomeSkater = NULL, *pAwaySkater = NULL;
	for (int iForward = 0; iForward < NUM_FORWARDS_PER_LINE; iForward++)
	{
		pHomeSkater = m_pGame->GetHomeRoster()->GetForwardLine(iHomeForwardLine)->GetForward(iForward);
		pAwaySkater = m_pGame->GetAwayRoster()->GetForwardLine(iAwayForwardLine)->GetForward(iForward);

		iHomeScore += pHomeSkater->GetAcceleration();
		iHomeScore += pHomeSkater->GetAgility();
		iHomeScore += pHomeSkater->GetBalance();
		iHomeScore += pHomeSkater->GetSpeed();
		iHomeScore += pHomeSkater->GetBodyChecking();
		iHomeScore += pHomeSkater->GetStrength();
		iHomeScore += pHomeSkater->GetHandEye();
		iHomeScore += pHomeSkater->GetPuckControl();
		iHomeScore += pHomeSkater->GetVision();
		iHomeScore += pHomeSkater->GetPositioning();
		iHomeScore += pHomeSkater->GetShotBlocking();
		iHomeScore += pHomeSkater->GetStickChecking();

		iAwayScore += pAwaySkater->GetAcceleration();
		iAwayScore += pAwaySkater->GetAgility();
		iAwayScore += pAwaySkater->GetBalance();
		iAwayScore += pAwaySkater->GetSpeed();
		iAwayScore += pAwaySkater->GetBodyChecking();
		iAwayScore += pAwaySkater->GetStrength();
		iAwayScore += pAwaySkater->GetHandEye();
		iAwayScore += pAwaySkater->GetPuckControl();
		iAwayScore += pAwaySkater->GetVision();
		iAwayScore += pAwaySkater->GetPositioning();
		iAwayScore += pAwaySkater->GetShotBlocking();
		iAwayScore += pAwaySkater->GetStickChecking();
	}
	for (int iDefenseman = 0; iDefenseman < NUM_DEFENSEMEN_PER_LINE; iDefenseman++)
	{
		pHomeSkater = m_pGame->GetHomeRoster()->GetDefenseLine(iHomeDefenseLine)->GetDefenseman(iDefenseman);
		pAwaySkater = m_pGame->GetAwayRoster()->GetDefenseLine(iAwayDefenseLine)->GetDefenseman(iDefenseman);

		iHomeScore += pHomeSkater->GetAcceleration();
		iHomeScore += pHomeSkater->GetAgility();
		iHomeScore += pHomeSkater->GetBalance();
		iHomeScore += pHomeSkater->GetSpeed();
		iHomeScore += pHomeSkater->GetBodyChecking();
		iHomeScore += pHomeSkater->GetStrength();
		iHomeScore += pHomeSkater->GetHandEye();
		iHomeScore += pHomeSkater->GetPuckControl();
		iHomeScore += pHomeSkater->GetVision();
		iHomeScore += pHomeSkater->GetPositioning();
		iHomeScore += pHomeSkater->GetShotBlocking();
		iHomeScore += pHomeSkater->GetStickChecking();

		iAwayScore += pAwaySkater->GetAcceleration();
		iAwayScore += pAwaySkater->GetAgility();
		iAwayScore += pAwaySkater->GetBalance();
		iAwayScore += pAwaySkater->GetSpeed();
		iAwayScore += pAwaySkater->GetBodyChecking();
		iAwayScore += pAwaySkater->GetStrength();
		iAwayScore += pAwaySkater->GetHandEye();
		iAwayScore += pAwaySkater->GetPuckControl();
		iAwayScore += pAwaySkater->GetVision();
		iAwayScore += pAwaySkater->GetPositioning();
		iAwayScore += pAwaySkater->GetShotBlocking();
		iAwayScore += pAwaySkater->GetStickChecking();
	}

	// Increment this if another stat is added above!
	int iNumStats = 12;
	// Average the numbers out
	iHomeScore /= (iNumStats * (NUM_FORWARDS_PER_LINE + NUM_DEFENSEMEN_PER_LINE));
	iAwayScore /= (iNumStats * (NUM_FORWARDS_PER_LINE + NUM_DEFENSEMEN_PER_LINE));

	m_iSkillDifference = (iHomeScore - iAwayScore);
}

// Simulate a faceoff. If pIcingTeam is not NULL, then pIcingTeam is the team that has iced the puck!
Team* Period::SimulateFaceoff(FaceoffZoneType eFaceoffZone, Team* pIcingTeam)
{
	if (!GetGame()->IsPlayingInGame(pIcingTeam)) pIcingTeam = NULL;

	// Set puck location relative to eFaceoffZone
	switch (eFaceoffZone)
	{
	case FACEOFF_ZONE_HOME:
		m_eCurrentZone = ZONE_TYPE_HOME;
		break;
	case FACEOFF_ZONE_AWAY:
		m_eCurrentZone = ZONE_TYPE_AWAY;
		break;
	case FACEOFF_ZONE_NEUTRAL_AWAY:
	case FACEOFF_ZONE_NEUTRAL_HOME:
	case FACEOFF_ZONE_NEUTRAL_CENTER:
		m_eCurrentZone = ZONE_TYPE_NEUTRAL;
		break;
	}

	GameSkater* pHomeCenter = GetGame()->GetHomeRoster()->GetForwardLine(iHomeForwardLine)->GetCenter();
	GameSkater* pAwayCenter = GetGame()->GetAwayRoster()->GetForwardLine(iAwayForwardLine)->GetCenter();

	// Small chance to be tossed out of the faceoff circle
	int iHomeChanceToBeTossed = 200 + pHomeCenter->GetAggressiveness();
	int iAwayChanceToBeTossed = 200 + pHomeCenter->GetAggressiveness();

	// Replace the center with the right winger (todo: best of two winger faceoffs...
	if (CORE.GetRandom().GetRandInt(10000) < iHomeChanceToBeTossed)
		pHomeCenter = GetGame()->GetHomeRoster()->GetForwardLine(iHomeForwardLine)->GetRightWing();
	if (CORE.GetRandom().GetRandInt(10000) < iAwayChanceToBeTossed)
		pAwayCenter = GetGame()->GetAwayRoster()->GetForwardLine(iAwayForwardLine)->GetRightWing();

	int iHomeScore = 0;
	iHomeScore = pHomeCenter->GetFaceoff() * 75;
	iHomeScore += pHomeCenter->GetStrength() * 25;

	int iAwayScore = 0;
	iAwayScore = pAwayCenter->GetFaceoff() * 75;
	iAwayScore += pAwayCenter->GetStrength() * 25;

	int iHomeWinChance = iHomeScore * 100 / (iHomeScore + iAwayScore);
	bool bHomeWin = CORE.GetRandom().GetRandInt(100) < iHomeWinChance;

	GameSkater* pWinningCenter = bHomeWin ? pHomeCenter : pAwayCenter;
	bHomeWin ? HomeStats.iFaceoffs++ : AwayStats.iFaceoffs++;

	// Log faceoff win
	CORE.GetLogger().SetCurrentFile("GameEvent.csv");

	std::string szString = std::to_string(GetGame()->GetID()) + ",";
	szString += CORE.GetTimeString(GetSecondsLeft()) + ",";
	szString += "Faceoff Win, " + pWinningCenter->GetSkater()->GetTeam()->GetInfo()->GetAbbreviation() + ",";
	szString += pWinningCenter->GetSkater()->GetInfo()->GetFirstName() + " " + pWinningCenter->GetSkater()->GetInfo()->GetLastName() + ",";
	CORE.GetLogger().WriteLogToFile(szString);

	return bHomeWin ? GetGame()->GetHomeTeam() : GetGame()->GetAwayTeam();
}

// Get the skill difference of the players on the ice (in Home - Away format)
int Period::GetSkillDifference() const
{
	return m_iSkillDifference;
}

PeriodTeamStats& Period::GetHomeStats()
{
	return HomeStats;
}
PeriodTeamStats& Period::GetAwayStats()
{
	return AwayStats;
}

// Get the game this period belongs to
Game* Period::GetGame() const
{
	return m_pGame;
}

// Is pTeam the home team of this game?
bool Period::IsHomeTeam(Team* pTeam) const
{
	return m_pGame->GetHomeTeam() == pTeam;
}

int Period::GetSecondsLeft() const
{
	return m_iSecondsLeft;
}

Period::PeriodType Period::GetPeriodType() const
{
	return ePeriodType;
}

std::vector<std::pair<GameSkater*, PeriodSkaterStats>>& Period::GetSkaterStats()
{
	return SkaterStats;
}

////////////////////////////////
// GAME
////////////////////////////////

Game::Game(Team* home, Team* away, Game::GameType eGameType, Game::OvertimeType eOvertimeType):
	uiId(uiGameIDs++),		// assign the lastest game ID to id, and then increment the game ID
	pHomeTeam(home),
	pAwayTeam(away),
	winner_(NULL),
	periods(),
	eGameType(eGameType),
	eOvertimeType(eOvertimeType),
	bIsFinal(false),
	paGameSkaters(),
	HomeStats(),
	AwayStats(),
	pHomeRoster(new GameRoster(home)),
	pAwayRoster(new GameRoster(away))
{
	// Test code...
	for (int i = 0; i < NUM_LINES_FORWARDS; i++)
	{
		for (int j = 0; j < NUM_FORWARDS_PER_LINE; j++)
		{
			paGameSkaters.push_back(pHomeRoster->GetForwardLine(i)->GetForward(j));
			paGameSkaters.push_back(pAwayRoster->GetForwardLine(i)->GetForward(j));
		}
	}
	for (int i = 0; i < NUM_LINES_DEFENSEMEN; i++)
	{
		for (int j = 0; j < NUM_DEFENSEMEN_PER_LINE; j++)
		{
			paGameSkaters.push_back(pHomeRoster->GetDefenseLine(i)->GetDefenseman(j));
			paGameSkaters.push_back(pAwayRoster->GetDefenseLine(i)->GetDefenseman(j));
		}
	}

	// Todo: Goalie stats here?
	/*
	for (int i = 0; i < NUM_ACTIVE_ROSTER_GOALIES)
	{

	}
	*/

	// Push back three periods to start
	periods.push_back(Period(this, Period::PERIOD_TYPE_REGULATION, 20));
	periods.push_back(Period(this, Period::PERIOD_TYPE_REGULATION, 20));
	periods.push_back(Period(this, Period::PERIOD_TYPE_REGULATION, 20));
}

Game::~Game()
{
	// Clear memory from the Game Rosters for each because the game is over!
	delete pHomeRoster;
	delete pAwayRoster;
}

// Simulate a game
void Game::Simulate()
{
	std::list<Period>::iterator periodIter = periods.begin();
	while (!IsFinal())
	{
		periodIter->Simulate();

		// Update the game stats with the period's stats when the period has completed
		HomeStats += (periodIter->GetHomeStats());
		AwayStats += (periodIter->GetAwayStats());

		// Increment stats for the players
		for (int i = 0; i < paGameSkaters.size(); i++)
		{
			*paGameSkaters[i]->GetGameStats() += periodIter->GetSkaterStats()[i].second;
		}

		// No more periods ahead of us
		if (std::next(periodIter, 1) == periods.end())
		{
			// Check to see if it's a tie game
			if (HomeStats.iGoals == AwayStats.iGoals)
			{
				switch (periodIter->GetPeriodType())
				{
				// Regulation period ended in a tie
				case Period::PERIOD_TYPE_REGULATION:
					// Start a five minute overtime
					if (eOvertimeType == Game::OVERTIME_FIVE_OT_SHOOTOUT)
						periods.push_back(Period(this, Period::PERIOD_TYPE_OVERTIME, 5));
					// Start a full-length overtime period
					else if (eOvertimeType == Game::OVERTIME_CONTINUOUS)
						periods.push_back(Period(this, Period::PERIOD_TYPE_OVERTIME, 20));
					break;
				// Overtime period ended in a tie
				case Period::PERIOD_TYPE_OVERTIME:
					// Start the shoout
					if (eOvertimeType == Game::OVERTIME_FIVE_OT_SHOOTOUT)
						periods.push_back(Period(this, Period::PERIOD_TYPE_SHOOTOUT, 0));
					// Start a full-length overtime period
					else if (eOvertimeType == Game::OVERTIME_CONTINUOUS)
						periods.push_back(Period(this, Period::PERIOD_TYPE_OVERTIME, 20));
					break;
				// Period was a shootout. This should never happen!
				case Period::PERIOD_TYPE_SHOOTOUT:
					std::cout << "Error! A shootout resulted in a tie. Something went wrong." << std::endl;
					break;
				default:
					std::cout << "Something broke terribly! A period without a period type tied!" << std::endl;
				}
			}
			// Not a tied game
			else
			{
				// Determine the winner of the game.
				DetermineWinner(*periodIter);
				Finalize();
				return;
			}
		}

		if (!bIsFinal)
		{
			periodIter++;
		}
	}
}

// Once a game is over, determine a winner based on the last period that ended
void Game::DetermineWinner(const Period& kLastPeriod)
{
	Team* pWinner = GetWinner();
	if (pWinner == NULL)
	{
		std::cout << "Warning! Finding a winner failed!" << std::endl;
		return;
	}
	Team* pLoser = pWinner == pHomeTeam ? pAwayTeam : pHomeTeam;

	pWinner->GetRegularSeasonRecord().AwardWin(eGameType, kLastPeriod.GetPeriodType());
	pLoser->GetRegularSeasonRecord().AwardLoss(eGameType, kLastPeriod.GetPeriodType());
}

// Finalize the game (such as adding player stats, etc.)
void Game::Finalize()
{
	// Increment stats for the players
	std::vector< GameSkater* >::iterator it;
	for (it = paGameSkaters.begin(); it != paGameSkaters.end(); it++)
	{
		(*it)->GetSkater()->GetSeasonStats() += *(*it)->GetGameStats();
	}
}

GameTeamStats& Game::GetHomeStats()
{
	return HomeStats;
}
GameTeamStats& Game::GetAwayStats()
{
	return AwayStats;
}

// Is the game over?
bool Game::IsFinal() const
{
	return bIsFinal;
}

// Get the game's away team
Team* Game::GetAwayTeam() const
{
	return pAwayTeam;
}

// Get the game's home team
Team* Game::GetHomeTeam() const
{
	return pHomeTeam;
}

// Return the winner of the game. Will be  NULL if the game has not ended yet
Team* Game::GetWinner() const
{
	if (AwayStats.iGoals < HomeStats.iGoals) return pHomeTeam;
	if (AwayStats.iGoals > HomeStats.iGoals) return pAwayTeam;
	return NULL;
}

Game::GameType Game::GetGameType() const
{
	return eGameType;
}

Game::OvertimeType Game::GetOvertimeType() const
{
	return eOvertimeType;
}

// Return the ID of the game
uint Game::GetID() const
{
	return uiId;
}

std::vector<GameSkater*> Game::GetGameSkaters() const
{
	return paGameSkaters;
}

// Is pTeam in this game?
bool Game::IsPlayingInGame(Team* pTeam) const
{
	return(pTeam == pHomeTeam || pTeam == pAwayTeam );
}

GameRoster* Game::GetHomeRoster() const
{
	return pHomeRoster;
}

GameRoster* Game::GetAwayRoster() const
{
	return pAwayRoster;
}