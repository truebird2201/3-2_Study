#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_TakeRest,
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_TakeRest:
    
    return "TakeRest"; 

  default:

    return "Not recognized!";
  }
}

#endif