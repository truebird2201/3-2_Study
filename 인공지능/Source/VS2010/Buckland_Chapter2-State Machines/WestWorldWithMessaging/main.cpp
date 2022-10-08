#include <fstream>
#include <time.h>

#include "Locations.h"

#include "SpongeBob.h"
#include "Crab.h"
#include "JingJing.h"

#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"


std::ofstream os;

int main()
{
    printf("2020182031 ÀÌ¼­¿¬\n\n");
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  //seed random number generator
  srand((unsigned) time(NULL));

  //create a Sponge
  SpongeBob* Bob = new SpongeBob(ent_SpongeBob);

  //create Crab
  Crab* Boss = new Crab(ent_Crab);

  //create JingJing
  JingJing* Jing = new JingJing(ent_JingJing);

  //register them with the entity manager
  EntityMgr->RegisterEntity(Bob);
  EntityMgr->RegisterEntity(Boss);
  EntityMgr->RegisterEntity(Jing);

  //run Bob and Boss through a few Update calls
  for (int i=0; i<30; ++i)
  { 
    Bob->Update();
    Boss->Update();
    Jing->Update();

    //dispatch any delayed messages
    Dispatch->DispatchDelayedMessages();

    Sleep(800);
  }

  //tidy up
  delete Bob;
  delete Boss;
  delete Jing;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();


  return 0;
}






