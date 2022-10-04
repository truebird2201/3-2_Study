#include "SpongeBobOwnedStates.h"
#include "SpongeBob.h"
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

SpongeBobGlobalState* SpongeBobGlobalState::Instance()
{
    static SpongeBobGlobalState instance;

    return &instance;
}


void SpongeBobGlobalState::Execute(SpongeBob* sponge)
{
    // 
    if ((RandFloat() < 0.1) &&
        !sponge->GetFSM()->isInState(*Poop::Instance()))
    {
        sponge->GetFSM()->ChangeState(Poop::Instance());
    }
}

bool SpongeBobGlobalState::OnMessage(SpongeBob* sponge, const Telegram& msg)
{
    switch (msg.Msg)
    {
    case Msg_Money:
    {
        cout << "\n" << GetNameOfEntity(sponge->ID()) <<
            ": 보너스 감사합니다 집게사장님~";
    }

    case Msg_Angry:
    {
        if(sponge->Location() == KrabShop)
            cout << "\n" << GetNameOfEntity(sponge->ID()) << ": 징징아! 일이나해! ";
        if (sponge->Location() == Hill)
            cout << "\n" << GetNameOfEntity(sponge->ID()) << ": 으악 징징아 그렇다구 해파리 동산까지 오니";
        if (sponge->Location() == SpongeBobHouse)
            cout << "\n" << GetNameOfEntity(sponge->ID()) << ": 집에 있을 땐 쉬게해줘 징징아...";
    }

    return true;

    }//end switch


    return false;
}

//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
MakeBurger* MakeBurger::Instance()
{
    static MakeBurger instance;

    return &instance;
}


void MakeBurger::Enter(SpongeBob* sponge)
{
    if (sponge->Location() != KrabShop)
    {
        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "일을 하기 위하여 집게리아로 출근합니다.";
        sponge->ChangeLocation(KrabShop);
    }
}


void MakeBurger::Execute(SpongeBob* sponge)
{
    // 햄버거를 만든다
    sponge->AddBurger();
    sponge->IncreaseTired();

    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "햄버거를 하나 만들었습니다. ( 현재 버거 = " << sponge->MakedBurger() << " / " << sponge->MaxBurger() << " )";

    if (sponge->FinishWork())
    {
        sponge->GetFSM()->ChangeState(CatchJellyFish::Instance());
    }

    if (sponge->Tired())
    {
        sponge->GetFSM()->ChangeState(Rest::Instance());
    }
}


void MakeBurger::Exit(SpongeBob* sponge)
{
    if (sponge->Location() == KrabShop)
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "집게리아에서 퇴근합니다.";

}

//------------------------------------------------------------------------쉬기
Rest* Rest::Instance()
{
    static Rest instance;

    return &instance;
}


void Rest::Enter(SpongeBob* sponge)
{
    Dispatch->DispatchMessage(1.5,                  //time delay
        sponge->ID(),           //sender ID
        sponge->ID(),           //receiver ID
        Msg_TakeRest,        //msg
        NO_ADDITIONAL_INFO);

        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "쉬어야 겠어요 ( 지금 시각 : " << Clock->GetCurrentTime() << " )";
}


void Rest::Execute(SpongeBob* sponge)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": Zzzz .... ";
    sponge->GetFSM()->RevertToPreviousState();
}


void Rest::Exit(SpongeBob* sponge)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "이제 일어나볼까?";
    sponge->SetTiredZero();
}

bool Rest::OnMessage(SpongeBob* sponge, const Telegram& msg)
{
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    switch (msg.Msg)
    {
    case Msg_TakeRest:
    {

        SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": 깼다!" << " ( 지금 시각 : " << Clock->GetCurrentTime() << " )";
        sponge->SetTiredZero();
        sponge->GetFSM()->RevertToPreviousState();
    }

    return true;

    }//end switch

    return false;
}

//------------------------------------------------------------------------해파리 잡기

CatchJellyFish* CatchJellyFish::Instance()
{
    static CatchJellyFish instance;

    return &instance;
}

void CatchJellyFish::Enter(SpongeBob* sponge)
{
    if (sponge->Location() != Hill)
    {
        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "해파리 동산으로 갑니다";

        sponge->ChangeLocation(Hill);
    }
}


void CatchJellyFish::Execute(SpongeBob* sponge)
{
    //deposit the gold
    sponge->AddJellyFish();
    sponge->IncreaseTired();

    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "신난다! 해파리를 잡았다! ( 해파리 =  " << sponge->CatchedJellyFish() << " / " << sponge->MaxJF() << " )";

    if (sponge->FinishCatch())
    {
        sponge->GetFSM()->ChangeState(GoHome::Instance());
    }

    if (sponge->Tired())
    {
        sponge->GetFSM()->ChangeState(Rest::Instance());
    }
}


void CatchJellyFish::Exit(SpongeBob* sponge)
{
    if (sponge->Location() == Hill)
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "휴~ 해파리 동산을 떠납니다.";
}


bool CatchJellyFish::OnMessage(SpongeBob* sponge, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}
//------------------------------------------------------------------------똥싸기

Poop* Poop::Instance()
{
    static Poop instance;

    return &instance;
}

void Poop::Enter(SpongeBob* sponge)
{
    if (sponge->Location() == Hill)
    {
        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << " 똥이 마려운데 근처에 화장실이 없으니 언덕 뒤쪽으로 갑니다..";
    }
    else {
        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << " 똥이 마려워서 화장실로 들어갑니다! ";
    }
}

void Poop::Execute(SpongeBob* sponge)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << " 뿡 ";
    sponge->GetFSM()->RevertToPreviousState();
}

void Poop::Exit(SpongeBob* sponge)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "아 시원행~";
}


bool Poop::OnMessage(SpongeBob* sponge, const Telegram& msg)
{
    return false; //send message to global message handler
}

//------------------------------------------------------------------------집가기

GoHome* GoHome::Instance()
{
    static GoHome instance;

    return &instance;
}

void GoHome::Enter(SpongeBob* sponge)
{
    if (sponge->Location() != SpongeBobHouse)
    {
        sponge->ChangeLocation(SpongeBobHouse);

        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "이제 집으로 가서 쉬어야겠어요";
    }
}

void GoHome::Execute(SpongeBob* sponge)
{
    sponge->IncreaseTired();

}


void GoHome::Exit(SpongeBob* sponge)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "집에서 나가야지";
}


bool GoHome::OnMessage(SpongeBob* sponge, const Telegram& msg)
{
    return false;
}

