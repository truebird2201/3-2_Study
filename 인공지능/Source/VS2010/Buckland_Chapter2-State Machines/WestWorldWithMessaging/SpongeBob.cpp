#include "SpongeBob.h"
bool SpongeBob::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void SpongeBob::Update()
{
	//set text color to green
	SetTextColor(FOREGROUND_RED | FOREGROUND_GREEN);

	m_pStateMachine->Update();
}