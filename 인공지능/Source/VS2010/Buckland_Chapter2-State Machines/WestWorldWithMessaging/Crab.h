#ifndef MINERSWIFE_H
#define MINERSWIFE_H
//------------------------------------------------------------------------
//
//  Name: MinersWife.h
//
//  Desc: class to implement Miner Bob's wife.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <string>

#include "fsm/State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "CrabOwnedStates.h"
#include "misc/ConsoleUtils.h"
#include "fsm/StateMachine.h"
#include "misc/Utils.h"



class Crab : public BaseGameEntity
{
private:

  //an instance of the state machine class
  StateMachine<Crab>* m_pStateMachine;

  location_type   m_Location;

  //is she presently cooking?
  bool            m_bWorking;


public:

   Crab(int id):m_Location(KrabShop),
       m_bWorking(false),
                     BaseGameEntity(id)
                                        
  {
    //set up the state machine
    m_pStateMachine = new StateMachine<Crab>(this);

    m_pStateMachine->SetCurrentState(WatchTV::Instance());

    m_pStateMachine->SetGlobalState(CrabGlobalState::Instance());
  }

  ~Crab(){delete m_pStateMachine;}


  //this must be implemented
  void          Update();

  //so must this
  virtual bool  HandleMessage(const Telegram& msg);

  StateMachine<Crab>* GetFSM()const{return m_pStateMachine;}

  //----------------------------------------------------accessors
  location_type Location()const{return m_Location;}
  void          ChangeLocation(location_type loc){m_Location=loc;}

  bool          Cooking()const{return m_bWorking;}
  void          SetCooking(bool val){ m_bWorking = val;}
   
};

#endif
