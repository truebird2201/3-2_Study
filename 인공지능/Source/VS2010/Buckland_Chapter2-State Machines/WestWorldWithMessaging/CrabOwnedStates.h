#ifndef CRAB_OWNED_STATES_H
#define CRAB_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinersWifeOwnedStates.h
//
//  Desc:   All the states that can be assigned to the MinersWife class
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "fsm/State.h"

class Crab;



//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class CrabGlobalState : public State<Crab>
{  
private:
  
	CrabGlobalState(){}

  //copy ctor and assignment should be private
	CrabGlobalState(const CrabGlobalState&);
	CrabGlobalState& operator=(const CrabGlobalState&);
 
public:

  //this is a singleton
  static CrabGlobalState* Instance();
  
  virtual void Enter(Crab* crab){}

  virtual void Execute(Crab* crab);

  virtual void Exit(Crab* crab){}

  virtual bool OnMessage(Crab* crab, const Telegram& msg);
};


//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class WatchTV : public State<Crab>
{
private:

	WatchTV(){}
  
  //copy ctor and assignment should be private
	WatchTV(const WatchTV&);
	WatchTV& operator=(const WatchTV&);

public:

  //this is a singleton
  static WatchTV* Instance();
  
  virtual void Enter(Crab* crab);

  virtual void Execute(Crab* crab);

  virtual void Exit(Crab* crab);
  
  virtual bool OnMessage(Crab* crab, const Telegram& msg);

};



//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class GiveMoney : public State<Crab>
{
private:
  
	GiveMoney(){}

  //copy ctor and assignment should be private
	GiveMoney(const GiveMoney&);
	GiveMoney& operator=(const GiveMoney&);
 
public:

  //this is a singleton
  static GiveMoney* Instance();
  
  virtual void Enter(Crab* crab);

  virtual void Execute(Crab* crab);

  virtual void Exit(Crab* crab);

  virtual bool OnMessage(Crab* crab, const Telegram& msg);

};


#endif