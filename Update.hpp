// Created by CBunt on 28 May 2020.
//
// Documentation for this controller can be found at:
// https://www.boost.org/doc/libs/1_73_0/doc/html/boost_asio.html

#ifndef TEXAS_HOLD_EM_UPDATE_HPP
#define TEXAS_HOLD_EM_UPDATE_HPP


#ifndef CHAT_UPDATE_HPP
#define CHAT_UPDATE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class Update
{
public:
  enum {HeaderLength = 4};
  enum {MaxBodyLength = 512};

  Update()
      : BodyLength(0)
  {
  }

  const char* data() const
  {
    return Data;
  }

  char* data()
  {
    return Data;
  }

  std::size_t length() const
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

  bool MakeHeader()
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
    std::sprintf(Header, "%4d", static_cast<int>(BodyLength));
    std::memcpy(Data, Header, HeaderLength);
  }

private:
  char Data[HeaderLength + MaxBodyLength];
  std::size_t BodyLength;
};

#endif // CHAT_MESSAGE_HPP
#endif //TEXAS_HOLD_EM_UPDATE_HPP
