#pragma once
#ifndef MINER_H
#define MINER_H

#include <string>

#include "fsm/State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "JingJingOwnedStates.h"
#include "misc/ConsoleUtils.h"
#include "fsm/StateMachine.h"
#include "misc/Utils.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

// 최대 피곤함
const int MaxTired = 5;

// 최대 밀린 손님 수
const int MaxPeople = 5;


class JingJing : public BaseGameEntity
{
private:

    //an instance of the state machine class
    StateMachine<JingJing>* m_pStateMachine;

    location_type         m_Location;

    // 밀린 손님 수
    int                   m_iPeople;

    // 피곤함 정도
    int                   m_iTired;


public:

    JingJing(int id) :m_Location(KrabShop),
        m_iPeople(2),
        m_iTired(0),
        BaseGameEntity(id)

    {
        //set up state machine
        m_pStateMachine = new StateMachine<JingJing>(this);

        m_pStateMachine->SetCurrentState(MakeBurger::Instance());

        m_pStateMachine->SetGlobalState(JingJingGlobalState::Instance());

        /* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE MINER */
    }

    ~JingJing() { delete m_pStateMachine; }

    //this must be implemented
    void Update();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);


    StateMachine<JingJing>* GetFSM()const { return m_pStateMachine; }



    //-------------------------------------------------------------accessors
    location_type Location()const { return m_Location; }
    void          ChangeLocation(location_type loc) { m_Location = loc; }

    int           People()const { return m_iPeople; }
    void           MinusPeopel() { m_iPeople-=1; }

    void          SetTiredZero() { m_iTired = 0; }
    void          IncreaseTired() { m_iTired += 1; }
    void          MinusTired() { m_iTired += 1; }

    bool          Tired() { return m_iTired >= MaxTired; }

};



#endif
