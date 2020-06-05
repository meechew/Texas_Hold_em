// Created by CBunt on 30 May 2020.
//

#ifndef TEXAS_HOLD_EM_PACKAGE_HPP
#define TEXAS_HOLD_EM_PACKAGE_HPP
#include "Player.hpp"
#include <utility>
#include <boost/container/vector.hpp>
#include <cstdio>

class ServerPackage {
public:
  ServerPackage() = default;

  bool HeartBeat{};
  bool WinnerNotice{};
  bool SplitPot{};
  string Name;
  string Winner;
  hand Hand;
  boost::container::vector<card> Tbl;
  ServerPackage(bool hb, bool wn, bool sp, string n, string w, hand h,
      boost::container::vector<card> tbl):
    HeartBeat(hb), WinnerNotice(wn), SplitPot(sp), Name(std::move(n)),
    Winner(std::move(w)), Hand(std::move(h)), Tbl(std::move(tbl)){}
  friend std::ostream &operator<<(std::ostream &out, const ServerPackage &s);
  friend std::istream &operator>>(std::istream &in, ServerPackage &s);
};

class ClientPackage {
public:
  bool HeartBeat;
  bool NewGame;
  bool Leave;
  string Name;
  ClientPackage(bool hb, bool ng, bool l, string n):
    HeartBeat(hb), NewGame(ng), Leave(l), Name(std::move(n)){}
  friend std::ostream &operator<<(std::ostream &out, const ClientPackage &s);
  friend std::istream &operator>>(std::istream &in, ClientPackage &s);
};

#endif //TEXAS_HOLD_EM_PACKAGE_HPP
