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

// ±Û·Î¹ú state 

JingJingGlobalState* JingJingGlobalState::Instance()
{
    static JingJingGlobalState instance;

    return &instance;
}


void JingJingGlobalState::Execute(JingJing* jing)
{
    // 
    if ((RandFloat() < 0.1) &&
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
    {
        cout << "\n" << GetNameOfEntity(jing->ID()) <<
            ": ®Z.... ´©±¸ÄÚ¿¡ ºÙÀÌ¶ó±¸..";
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
    // ÇÜ¹ö°Å¸¦ ¸¸µç´Ù
    jing->MinusTired();
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
        jing->ID(),
        ent_SpongeBob,
        Msg_Angry,
        NO_ADDITIONAL_INFO);
    cout << "\n" << GetNameOfEntity(jing->ID()) << ": ½ºÆùÁö¹ä! ³Í ÃÖ¾ÇÀÌ¾ß!";
    jing->GetFSM()->RevertToPreviousState();

}


void Angry::Exit(JingJing* jing)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "È÷È÷.. ÇÇ°ïÇÑ°Ô Á» »ç¶óÁ³´Ù..";
}

//------------------------------------------------------------------------½¬±â
Angry* Angry::Instance()
{
    static Rest instance;

    return &instance;
}


void Angry::Enter(JingJing* jing)
{
    Dispatch->DispatchMessage(1.5,                  //time delay
        sponge->ID(),           //sender ID
        sponge->ID(),           //receiver ID
        Msg_TakeRest,        //msg
        NO_ADDITIONAL_INFO);

    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "½¬¾î¾ß °Ú¾î¿ä ( Áö±Ý ½Ã°¢ : " << Clock->GetCurrentTime() << " )";
}


void Angry::Execute(JingJing* jing)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": Zzzz .... ";
    sponge->GetFSM()->RevertToPreviousState();
}


void Angry::Exit(JingJing* jing)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "ÀÌÁ¦ ÀÏ¾î³ªº¼±î?";
    sponge->SetTiredZero();
}

bool Angry::OnMessage(JingJing* jing, const Telegram& msg)
{
    return false;
}

//------------------------------------------------------------------------ÇØÆÄ¸® Àâ±â

CatchJellyFish* CatchJellyFish::Instance()
{
    static CatchJellyFish instance;

    return &instance;
}

void CatchJellyFish::Enter(SpongeBob* sponge)
{
    if (sponge->Location() != Hill)
    {
        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "ÇØÆÄ¸® µ¿»êÀ¸·Î °©´Ï´Ù";

        sponge->ChangeLocation(Hill);
    }
}


void CatchJellyFish::Execute(SpongeBob* sponge)
{
    //deposit the gold
    sponge->AddJellyFish();
    sponge->IncreaseTired();

    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "½Å³­´Ù! ÇØÆÄ¸®¸¦ Àâ¾Ò´Ù! ( ÇØÆÄ¸® =  " << sponge->CatchedJellyFish() << " / " << sponge->MaxJF() << " )";

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
        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "ÈÞ~ ÇØÆÄ¸® µ¿»êÀ» ¶°³³´Ï´Ù.";
}


bool CatchJellyFish::OnMessage(SpongeBob* sponge, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}
//------------------------------------------------------------------------¶Ë½Î±â

Poop* Poop::Instance()
{
    static Poop instance;

    return &instance;
}

void Poop::Enter(SpongeBob* sponge)
{
    if (sponge->Location() == Hill)
    {
        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << " ¶ËÀÌ ¸¶·Á¿îµ¥ ±ÙÃ³¿¡ È­Àå½ÇÀÌ ¾øÀ¸´Ï ¾ð´ö µÚÂÊÀ¸·Î °©´Ï´Ù..";
    }
    else {
        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << " ¶ËÀÌ ¸¶·Á¿ö¼­ È­Àå½Ç·Î µé¾î°©´Ï´Ù! ";
    }
}

void Poop::Execute(SpongeBob* sponge)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << " »× ";
    sponge->GetFSM()->RevertToPreviousState();
}

void Poop::Exit(SpongeBob* sponge)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "¾Æ ½Ã¿øÇà~";
}


bool Poop::OnMessage(SpongeBob* sponge, const Telegram& msg)
{
    return false; //send message to global message handler
}

//------------------------------------------------------------------------Áý°¡±â

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

        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "ÀÌÁ¦ ÁýÀ¸·Î °¡¼­ ½¬¾î¾ß°Ú¾î¿ä";
    }
}

void GoHome::Execute(SpongeBob* sponge)
{
    sponge->IncreaseTired();

}


void GoHome::Exit(SpongeBob* sponge)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "Áý¿¡¼­ ³ª°¡¾ßÁö";
}


bool GoHome::OnMessage(SpongeBob* sponge, const Telegram& msg)
{
    return false;
}

