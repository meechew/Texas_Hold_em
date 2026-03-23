// Created by CBunt on 28 May 2020.
//
// Documentation for this controller can be found at:
// https://www.boost.org/doc/libs/1_73_0/doc/html/boost_asio.html

#ifndef TEXAS_HOLD_EM_UPDATE_HPP
#define TEXAS_HOLD_EM_UPDATE_HPP


#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace boost::uuids;

class Message
{
public:
    static constexpr int HEADER_LENGTH = 20;
    static constexpr int MAX_BODY_LENGTH = 512;

    Message()
        : body_length_(0), data_{}
    {
    }

    void new_message(std::stringstream& s, uuid u)
    {
        allocate_body(std::strlen(s.str().c_str()));
        std::memcpy(data_, s.str().c_str(), std::strlen(s.str().c_str()));
        uuid_ = reinterpret_cast<char*>(u.data);
    }

    const char* header() const
    {
        return data_;
    }

    char* header()
    {
        return data_;
    }

    const uint8_t* uuid() const
    {
        return reinterpret_cast<uint8_t*>(uuid_);
    }

    uint8_t* uuid()
    {
        return reinterpret_cast<uint8_t*>(uuid_);
    }

    char* data()
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
        body_length_ = bl + 1;
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
    char  data_[HEADER_LENGTH + MAX_BODY_LENGTH];
    char* uuid_ = data_ + 4;
};


#endif //TEXAS_HOLD_EM_UPDATE_HPP
