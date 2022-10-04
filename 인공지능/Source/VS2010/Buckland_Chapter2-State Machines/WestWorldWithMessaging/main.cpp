#include <fstream>
#include <time.h>

#include "Locations.h"
#include "SpongeBob.h"
#include "Crab.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"


std::ofstream os;

int main()
{
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  //seed random number generator
  srand((unsigned) time(NULL));

  //create a miner
  SpongeBob* Bob = new SpongeBob(ent_SpongeBob);

  //create his wife
  Crab* Boss = new Crab(ent_Crab);

  //register them with the entity manager
  EntityMgr->RegisterEntity(Bob);
  EntityMgr->RegisterEntity(Boss);

  //run Bob and Boss through a few Update calls
  for (int i=0; i<30; ++i)
  { 
    Bob->Update();
    Boss->Update();

    //dispatch any delayed messages
    Dispatch->DispatchDelayedMessages();

    Sleep(800);
  }

  //tidy up
  delete Bob;
  delete Boss;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();


  return 0;
}






