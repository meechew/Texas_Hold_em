// Created by CBunt on 31 May 2020.
//

#include <boost/interprocess/streams/bufferstream.hpp>
#include "Package.hpp"

ServerPackage::ServerPackage(char*)
{
}

std::ostream& operator<<(std::ostream& out, const ServerPackage& s)
{
    out << R"({"HeartBeat" : )" << s.heart_beat_ << ','
        << R"( "WinnerNotice" : )" << s.winner_notice_ << ','
        << R"( "Name" : ")" << s.name_ << "\","
        << R"( "Winner" : ")" << s.winner_ << "\","
        << R"( "Hand" : { "First" : {)"
        << R"( "Rank" : )" << s.hand_.first.rank << ','
        << R"( "Suit" : )" << s.hand_.first.suit << "},"
        << R"( "Second" : {)"
        << R"( "Rank" : )" << s.hand_.second.rank << ','
        << R"( "Suit" : )" << s.hand_.second.suit << "},"
        << R"( "CommonCards" : [)";


    for (int k = 0; k < 5; ++k)
    {
        out << R"({ "Rank" : )" << s.table_[k].rank << ','
            << R"( "Suit" : )" << s.table_[k].suit << '}';
        if (k < 4) out << ',';
        else out << "]}}";
    }

    return out;
}

string ServerPackage::serial() const
{
    string ret;
    ret = R"({ "HeartBeat" : )" + heart_beat_ + ',';
    ret += R"( "WinnerNotice" : )" + winner_notice_ + ',';
    ret += R"( "Name" : ")" + name_ + "\",";
    ret += R"( "Winner" : ")" + winner_ + "\",";
    ret += R"( "Hand" : { "First" : {)";
    ret += R"( "Rank" : )" + hand_.first.rank + ',';
    ret += R"( "Suit" : )" + hand_.first.suit;
    ret += "},";
    ret += R"( "Second" : {)";
    ret += R"( "Rank" : )" + hand_.second.rank + ',';
    ret += R"( "Suit" : )" + hand_.second.suit;
    ret += "},";
    ret += R"( "CommonCards" : [)";
    return ret;
}

std::istream& operator>>(std::istream& in, ServerPackage& s)
{
    char* c = new char;
    try
    {
        in.ignore(100, '{');
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("HeartBeat", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, '"');
        in.get(*c);
        s.heart_beat_ = atoi(c);
        in.ignore();
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("WinnerNotice", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, '"');
        in.get(*c);
        s.winner_notice_ = atoi(c);
        in.ignore();
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("SplitPot", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, '"');
        in.get(*c);
        s.split_pot_ = atoi(c);
        in.ignore();
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("Name", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, '"');
        in.get(c, 100, '"');
        s.name_ = c;
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("Winner", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, '"');
        in.get(c, 100, '"');
        s.winner_ = c;
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("Hand", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("First", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("Rank", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, '"');
        in.get(*c);
        s.hand_.first.rank = atoi(c);
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("Suit", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, '"');
        in.get(*c);
        s.hand_.first.suit = atoi(c);
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("Second", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("Rank", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, '"');
        in.get(*c);
        s.hand_.second.rank = atoi(c);
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("Suit", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, '"');
        in.get(*c);
        s.hand_.second.suit = atoi(c);
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("CommonCards", c)) { throw std::domain_error("Not valid package format"); }

        for (int k = 0; k < 5; ++k)
        {
            in.ignore(100, '"');
            in.get(c, 100, '"');
            if (strcmp("Rank", c)) { throw std::domain_error("Not valid package format"); }
            in.ignore(100, '"');
            in.get(*c);
            s.table_[k].rank = atoi(c);
            in.ignore(100, '"');
            in.get(c, 100, '"');
            if (strcmp("Suit", c)) { throw std::domain_error("Not valid package format"); }
            in.ignore(100, '"');
            in.get(*c);
            s.table_[k].suit = atoi(c);
        }
    }
    catch (std::domain_error& e)
    {
        delete c;
        throw e;
    }
    delete c;
    return in;
}

ClientPackage::ClientPackage(char* str)
{
    boost::interprocess::bufferstream in(str, 516);
    in >> *this;
    return;
}

string ClientPackage::serial()
{
    return "";
}

std::ostream& operator<<(std::ostream& out, const ClientPackage& s)
{
    out << R"({ "HeartBeat" : )" << s.heart_beat_ << ','
        << R"( "NextStep" : )" << s.next_step_ << ','
        << R"( "Leave" : )" << s.leave_ << ','
        << R"( "Name" : ")" << s.name_ << "\","
        << R"( "Uuid" : ")" << s.uuid_.begin() << "\" }";

    return out;
}

std::istream& operator>>(std::istream& in, ClientPackage& s)
{
    char* c = new char;
    try
    {
        in.ignore(100, '{');
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("HeartBeat", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, ':');
        in.get(c, 100, ',');
        s.heart_beat_ = atoi(c);
        in.ignore();
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("NextStep", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, ':');
        in.get(c, 100, ',');
        s.next_step_ = atoi(c);
        in.ignore();
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("Leave", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, ':');
        in.get(c, 100, ',');
        s.leave_ = atoi(c);
        in.ignore();
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("Name", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, ':');
        in.ignore(100, '"');
        in.get(c, 100, '"');
        s.name_ = c;
        in.ignore();
        in.ignore(100, '"');
        in.get(c, 100, '"');
        if (strcmp("Uuid", c)) { throw std::domain_error("Not valid package format"); }
        in.ignore(100, ':');
        in.ignore(100, '"');
        in.get(c, 17);
        memcpy(s.uuid_.data, c, 16);
    }
    catch (std::domain_error& e)
    {
        std::cerr << e.what() << " package dropped\n";
    }
    delete c;
    return in;
}
