#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_TakeRest,
  Msg_Money
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_TakeRest:
    
    return "TakeRest"; 

  case Msg_Money:

      return "Money";

  default:

    return "Not recognized!";
  }
}

#endif