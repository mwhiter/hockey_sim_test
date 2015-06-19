#pragma once

#include "Team.h"
#include "Player.h"
#include <list>

#define NUM_LINES_FORWARDS 4 
#define NUM_LINES_DEFENSEMEN 3
#define NUM_FORWARDS_PER_LINE 3
#define NUM_DEFENSEMEN_PER_LINE 2
#define NUM_GOALIES_PER_LINE 1

#define NUM_ACTIVE_ROSTER_SKATERS 18
#define NUM_ACTIVE_ROSTER_GOALIES 2
#define MAX_ROSTER_SIZE 23

class Player;
class Skater;
class Goalie;
class Team;

// A line of Forwards
// Contains an array of pointers to Skaters
class ForwardLine
{
public:
	ForwardLine() : ppForwardLine(new Skater*[NUM_FORWARDS_PER_LINE]{NULL, NULL, NULL}){}
	~ForwardLine() { };

	inline void SetLeftWing(Skater* pWinger)  { ppForwardLine[0] = pWinger; }
	inline Skater* GetLeftWing() const { return ppForwardLine[0]; }

	inline void SetCenter(Skater* pCenter)  { ppForwardLine[1] = pCenter; }
	inline Skater* GetCenter() const { return ppForwardLine[1]; }

	inline void SetRightWing(Skater* pWinger)  { ppForwardLine[2] = pWinger; }
	inline Skater* GetRightWing() const { return ppForwardLine[2]; }

	inline void SetForward(int index, Skater* pForward) const { ppForwardLine[index] = pForward; }
	inline Skater* GetForward(int index) const { return ppForwardLine[index]; }
private:
	Skater** ppForwardLine;
};

// A line of Defenseman
// Contains an array of pointers to Skaters
class DefenseLine
{
public:
	DefenseLine() : ppDefenseLine(new Skater*[NUM_DEFENSEMEN_PER_LINE]{NULL, NULL}){}
	~DefenseLine() { };

	inline void SetLeftDefenseman(Skater* defenseman)  { ppDefenseLine[0] = defenseman; }
	inline Skater* GetLeftDefenseman()  { return ppDefenseLine[0]; }

	inline void SetRightDefenseman(Skater* defenseman)  { ppDefenseLine[1] = defenseman; }
	inline Skater* GetRightDefenseman()  { return ppDefenseLine[1]; }

	inline void SetDefenseman(int index, Skater* pDefenseman) const { ppDefenseLine[index] = pDefenseman; }
	inline Skater* GetDefenseman(int index) const { return ppDefenseLine[index]; }
private:
	Skater** ppDefenseLine;
};

class GoalieLine
{
public:
	GoalieLine() : pGoalie(NULL) { }
	~GoalieLine() { };

	void SetGoalie(Goalie* goalie) { pGoalie = goalie; }
	Goalie* GetGoalie() { return pGoalie; }

private:
	Goalie* pGoalie;
};

class Roster
{
public:
	Roster(Team* pTeam);
	~Roster();

	bool AddPlayer(Skater* pSkater);
	bool AddPlayer(Goalie* pGoalie);

	std::pair<Skater*, bool>* FindSkater(int id);
	std::pair<Goalie*, bool>* FindGoalie(int id);
	
	void RemoveSkater(int id);
	void RemoveGoalie(int id);

	bool DressSkater(int roster_id, bool _dressed);
	bool DressGoalie(int roster_id, bool _dressed);

	void BestLines();

	bool IsValid() const;

	ForwardLine& GetForwardLine(int line);
	DefenseLine& GetDefenseLine(int line);
	GoalieLine& GetGoalieLine(int line);

	const std::list<std::pair<Skater*, bool>>& GetSkaters();

	// Accessors
	int GetNumActiveSkaters() const;
	int GetNumActiveGoalies() const;
	int GetSize() const;
private:
	Team* pTeam;

	int ScoreCenter(Skater* skater) const;

	ForwardLine forwardLines[NUM_LINES_FORWARDS];
	DefenseLine defenseLines[NUM_LINES_DEFENSEMEN];
	GoalieLine goalieLines[NUM_ACTIVE_ROSTER_GOALIES];

	int num_active_skaters_;
	int num_active_goalies_;

	std::list<std::pair<Skater*, bool>> skaters_;
	std::list<std::pair<Goalie*, bool>> goalies_;
};

////////////////////////////
////////////////////////////
// Temp code for lines of GameSkaters

class GameSkater;

// A line of Forwards
// Contains an array of pointers to Skaters
class GameForwardLine
{
public:
	GameForwardLine() : ppForwardLine(new GameSkater*[NUM_FORWARDS_PER_LINE]{NULL, NULL, NULL}){}
	~GameForwardLine() { };

	inline void SetLeftWing(GameSkater* pWinger)  { ppForwardLine[0] = pWinger; }
	inline GameSkater* GetLeftWing() const { return ppForwardLine[0]; }

	inline void SetCenter(GameSkater* pCenter)  { ppForwardLine[1] = pCenter; }
	inline GameSkater* GetCenter() const { return ppForwardLine[1]; }

	inline void SetRightWing(GameSkater* pWinger)  { ppForwardLine[2] = pWinger; }
	inline GameSkater* GetRightWing() const { return ppForwardLine[2]; }

	inline void SetForward(int index, GameSkater* pForward) const { ppForwardLine[index] = pForward; }
	inline GameSkater* GetForward(int index) const { return ppForwardLine[index]; }
private:
	GameSkater** ppForwardLine;
};

// A line of Defenseman
// Contains an array of pointers to Skaters
class GameDefenseLine
{
public:
	GameDefenseLine() : ppDefenseLine(new GameSkater*[NUM_DEFENSEMEN_PER_LINE]{NULL, NULL}){}
	~GameDefenseLine() { };

	inline void SetLeftDefenseman(GameSkater* defenseman)  { ppDefenseLine[0] = defenseman; }
	inline GameSkater* GetLeftDefenseman()  { return ppDefenseLine[0]; }

	inline void SetRightDefenseman(GameSkater* defenseman)  { ppDefenseLine[1] = defenseman; }
	inline GameSkater* GetRightDefenseman()  { return ppDefenseLine[1]; }

	inline void SetDefenseman(int index, GameSkater* pDefenseman) const { ppDefenseLine[index] = pDefenseman; }
	inline GameSkater* GetDefenseman(int index) const { return ppDefenseLine[index]; }
private:
	GameSkater** ppDefenseLine;
};

// A Roster for a Game.
// Created and destroyed with each new game.
// Necessary because in game we can't touch the lines, and player attributes may change based on what happens (injuries, stamina loss, etc.)
// Will create COPIES of players to store in them (not the actual players themselves)
//	- This is so that their ratings can change dynamically, for example when they get tired
class GameRoster
{
public:
	GameRoster(Team* pTeam);
	~GameRoster();

	GameForwardLine* GetForwardLine(int iLine) const;
	GameDefenseLine* GetDefenseLine(int iLine) const;
	GoalieLine* GetGoalie(int iLine) const;

	Team* GetTeam() const;
private:
	Roster* pRoster;	// pointer to the Team's roster. Can change, because at the end of the game, we want to be able to update injuries and stuff (when that's implemented)
	Team* pTeam;		// pointer to the Team

	// Lines that store GameSkaters (and copies of goalies ATM)
	GameForwardLine* paForwardLines[NUM_LINES_FORWARDS];
	GameDefenseLine* paDefenseLines[NUM_LINES_DEFENSEMEN];
	GoalieLine* paGoalieLines[NUM_ACTIVE_ROSTER_GOALIES];
};

// Helper struct to help us sort players by a member function
template <class T>
struct PlayerSorter
{
	int(T::* func)() const;
	PlayerSorter(int(T::* func)() const) :func(func){}
	inline bool operator()(const T* first, const T* second)
	{
		return (first->*func)() < (second->*func)();
	}
};