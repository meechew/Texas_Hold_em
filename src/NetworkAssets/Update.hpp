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
  static constexpr int HEADER_LENGTH   = 4;
  static constexpr int MAX_BODY_LENGTH = 512;

  Update()
      : body_length_(0)
  {
  }

  const char* header() const
  {
    return data_;
  }

  char* header()
  {
    return data_;
  }

  std::size_t length() const
  {
    return HEADER_LENGTH + body_length_;
  }

  const char* body() const
  {
    return data_ + HEADER_LENGTH;
  }

  char* body()
  {
    return data_ + HEADER_LENGTH;
  }

  std::size_t get_body_length() const
  {
    return body_length_;
  }

  void allocate_body(std::size_t bl)
  {
    body_length_ = bl;
    if (body_length_ > MAX_BODY_LENGTH)
      body_length_ = MAX_BODY_LENGTH;
  }

  bool decode_header()
  {
    char _header[HEADER_LENGTH + 1] = "";
    std::strncat(_header, data_, HEADER_LENGTH);
    body_length_ = std::atoi(_header);
    if (body_length_ > MAX_BODY_LENGTH)
    {
      body_length_ = 0;
      return false;
    }
    return true;
  }

  void encode_header()
  {
    char _header[HEADER_LENGTH + 1] = "";
    std::sprintf(_header, "%4lu", body_length_);
    std::memcpy(data_, _header, HEADER_LENGTH);
  }

private:
  std::size_t body_length_;
  char        data_[HEADER_LENGTH + MAX_BODY_LENGTH];
};


#endif //TEXAS_HOLD_EM_UPDATE_HPP
