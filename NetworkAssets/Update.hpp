// Created by CBunt on 28 May 2020.
//
// Documentation for this controller can be found at:
// https://www.boost.org/doc/libs/1_73_0/doc/html/boost_asio.html

#ifndef TEXAS_HOLD_EM_UPDATE_HPP
#define TEXAS_HOLD_EM_UPDATE_HPP


#include <cstdio>
#include <cstdlib>
#include <cstring>

class Update
{
public:
  static constexpr int HeaderLength  = 4;
  static constexpr int MaxBodyLength = 512;

  Update()
      : BodyLength(0)
  {
  }

  const char* Header() const
  {
    return Data;
  }

  char* Header()
  {
    return Data;
  }

  std::size_t Length() const
  {
    return HeaderLength + BodyLength;
  }

  const char* Body() const
  {
    return Data + HeaderLength;
  }

  char* Body()
  {
    return Data + HeaderLength;
  }

  std::size_t RetBodyLength() const
  {
    return BodyLength;
  }

  void MkBodyLength(std::size_t NewLength)
  {
    BodyLength = NewLength;
    if (BodyLength > MaxBodyLength)
      BodyLength = MaxBodyLength;
  }

  bool DecodeHeader()
  {
    char Header[HeaderLength + 1] = "";
    std::strncat(Header, Data, HeaderLength);
    BodyLength = std::atoi(Header);
    if (BodyLength > MaxBodyLength)
    {
      BodyLength = 0;
      return false;
    }
    return true;
  }

  void EncodeHeader()
  {
    char Header[HeaderLength + 1] = "";
    std::sprintf(Header, "%4lu", BodyLength);
    std::memcpy(Data, Header, HeaderLength);
  }

private:
  char Data[HeaderLength + MaxBodyLength];
  std::size_t BodyLength;
};


#endif //TEXAS_HOLD_EM_UPDATE_HPP
