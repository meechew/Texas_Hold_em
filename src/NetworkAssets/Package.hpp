// Created by CBunt on 30 May 2020.
//

#ifndef TEXAS_HOLD_EM_PACKAGE_HPP
#define TEXAS_HOLD_EM_PACKAGE_HPP
#include "../Player/Player.hpp"
#include <utility>
#include <boost/container/vector.hpp>
#include <cstdio>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace boost::container;
using namespace boost::uuids;

class ServerPackage
{
public:
    bool heart_beat_ = false;
    bool winner_notice_ = false;
    bool split_pot_ = false;
    string name_;
    string winner_;
    Hand hand_;
    Cards table_;
    ServerPackage() = default;
    ServerPackage(char* msg);

    ServerPackage(bool hb, bool wn, bool sp, string n, string w, Hand h,
                  Cards tbl) :
        heart_beat_(hb), winner_notice_(wn), split_pot_(sp), name_(std::move(n)),
        winner_(std::move(w)), hand_(std::move(h)), table_(std::move(tbl)) {}

    string serial() const;
    friend std::ostream& operator<<(std::ostream& out, const ServerPackage& s);
    friend std::istream& operator>>(std::istream& in, ServerPackage& s);
};

class ClientPackage
{
public:
    int heart_beat_ = 0;
    bool next_step_ = false;
    bool leave_ = false;
    string name_;
    uuid uuid_{};

    ClientPackage() = default;
    ClientPackage(char* str);
    ClientPackage(int hb, bool ng, bool l, string n, uuid u) :
        heart_beat_(hb), next_step_(ng), leave_(l), name_(std::move(n)), uuid_(u) {}

    string serial();
    friend std::ostream& operator<<(std::ostream& out, const ClientPackage& s);
    friend std::istream& operator>>(std::istream& in, ClientPackage& s);
};

#endif //TEXAS_HOLD_EM_PACKAGE_HPP
