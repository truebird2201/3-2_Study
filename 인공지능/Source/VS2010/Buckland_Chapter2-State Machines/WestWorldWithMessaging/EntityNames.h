#ifndef NAMES_H
#define NAMES_H

#include <string>

enum 
{
  ent_SpongeBob,

  ent_Crab,

  ent_JingJing
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
  {
  case ent_SpongeBob:

    return "SpongeBob";

  case ent_Crab:
    
    return "Crab";

  case ent_JingJing:

      return "JingJing";

  default:

    return "UNKNOWN!";
  }
}

#endif