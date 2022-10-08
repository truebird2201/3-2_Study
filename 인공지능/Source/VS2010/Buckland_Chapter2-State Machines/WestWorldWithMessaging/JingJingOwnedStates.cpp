#include "JingJingOwnedStates.h"
#include "JingJing.h"
#include "Locations.h"
#include "Time/CrudeTimer.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"
#include "misc/ConsoleUtils.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

// 글로벌 state 

JingJingGlobalState* JingJingGlobalState::Instance()
{
    static JingJingGlobalState instance;

    return &instance;
}


void JingJingGlobalState::Execute(JingJing* jing)
{
    // 
    if ((RandFloat() < 0.2) &&
        !jing->GetFSM()->isInState(*AngryToSponge::Instance()))
    {
        jing->GetFSM()->ChangeState(AngryToSponge::Instance());
    }
}

bool JingJingGlobalState::OnMessage(JingJing* jing, const Telegram& msg)
{
    switch (msg.Msg)
    {
    case Msg_Money:
    {   SetTextColor(BACKGROUND_BLUE | BACKGROUND_INTENSITY);
        cout << "\n" << GetNameOfEntity(jing->ID()) <<
            ": 췡.... 누구코에 붙이라구..";
    }

    return true;

    }//end switch


    return false;
}

//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
AngryToSponge* AngryToSponge::Instance()
{
    static AngryToSponge instance;

    return &instance;
}


void AngryToSponge::Enter(JingJing* jing)
{
}


void AngryToSponge::Execute(JingJing* jing)
{
    // 햄버거를 만든다
    jing->MinusTired();
    SetTextColor(BACKGROUND_BLUE | BACKGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(jing->ID()) << ": 스폰지밥! 넌 최악이야!";
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
        jing->ID(),
        ent_SpongeBob,
        Msg_Angry,
        NO_ADDITIONAL_INFO);
    
    jing->GetFSM()->ChangeState(TakeOrder::Instance());

}


void AngryToSponge::Exit(JingJing* jing)
{
    SetTextColor(BACKGROUND_BLUE | BACKGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(jing->ID()) << ": " << "히히.. 피곤한게 좀 사라졌다..";
}

bool AngryToSponge::OnMessage(JingJing* jing, const Telegram& msg)
{
    return false;
}

//------------------------------------------------------------------------쉬기
Angry* Angry::Instance()
{
    static Angry instance;

    return &instance;
}


void Angry::Enter(JingJing* jing)
{
}


void Angry::Execute(JingJing* jing)
{
    jing->IncreaseTired();
    cout << "\n" << GetNameOfEntity(jing->ID()) << ": 손님!!! 주문좀 똑바로 하세요!!!! ";
    jing->GetFSM()->ChangeState(TakeOrder::Instance());
}


void Angry::Exit(JingJing* jing)
{
}

bool Angry::OnMessage(JingJing* jing, const Telegram& msg)
{
    return false;
}

//------------------------------------------------------------------------주문 받기

TakeOrder* TakeOrder::Instance()
{
    static TakeOrder instance;

    return &instance;
}

void TakeOrder::Enter(JingJing* jing)
{
    cout << "\n" << GetNameOfEntity(jing->ID()) << ": " << "주문이나 받아야지..";
}


void TakeOrder::Execute(JingJing* jing)
{
    //deposit the gold
    jing->IncreaseTired();

    switch (RandInt(0, 2))
    {
    case 0:
        cout << "\n" << GetNameOfEntity(jing->ID()) << ": 게살버거 하나요~!";

        break;

    case 1:
        cout << "\n" << GetNameOfEntity(jing->ID()) << ": 불고기버거 하나요~!";

        break;

    case 2:
        cout << "\n" << GetNameOfEntity(jing->ID()) << ": 치킨버거 하나요~!";

        break;
    }
    if (RandFloat() < 0.1)
        jing->GetFSM()->ChangeState(Angry::Instance());
    if (jing->Tired())
    {
        jing->GetFSM()->ChangeState(Play::Instance());
    }
}


void TakeOrder::Exit(JingJing* jing)
{

}


bool TakeOrder::OnMessage(JingJing* jing, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}
//------------------------------------------------------------------------똥싸기

Play* Play::Instance()
{
    static Play instance;

    return &instance;
}

void Play::Enter(JingJing* jing)
{
    cout << "\n" << GetNameOfEntity(jing->ID()) << ": " << " 클라리넷이나 불어야겠다.. ";
}

void Play::Execute(JingJing* jing)
{
    cout << "\n" << GetNameOfEntity(jing->ID()) << ": " << " --삘릴리 뽀로로롱뿌뿌 삐삥-- ";
    jing->SetTiredZero();
    jing->GetFSM()->ChangeState(TakeOrder::Instance());
}

void Play::Exit(JingJing* jing)
{
    cout << "\n" << GetNameOfEntity(jing->ID()) << ": " << "즐거웠다..";
}


bool Play::OnMessage(JingJing* jing, const Telegram& msg)
{
    return false; //send message to global message handler
}
