// Created by CBunt on 30 May 2020.
//

#ifndef TEXAS_HOLD_EM_PACKAGE_HPP
#define TEXAS_HOLD_EM_PACKAGE_HPP
#include "Player.hpp"
#include <utility>
#include <boost/container/vector.hpp>
#include <cstdio>

using string = boost::container::string;

class ServerPackage {
public:
  bool HeartBeat = false;
  bool WinnerNotice = false;
  bool SplitPot = false;
  string Name;
  string Winner;
  hand Hand;
  boost::container::vector<card> Tbl;
  ServerPackage() = default;
  ServerPackage(char* UpDt);
  ServerPackage(bool hb, bool wn, bool sp, string n, string w, hand h,
      boost::container::vector<card> tbl):
    HeartBeat(hb), WinnerNotice(wn), SplitPot(sp), Name(std::move(n)),
    Winner(std::move(w)), Hand(std::move(h)), Tbl(std::move(tbl)){}
  string Serial();
  friend std::ostream &operator<<(std::ostream &out, const ServerPackage &s);
  friend std::istream &operator>>(std::istream &in, ServerPackage &s);
};

class ClientPackage {
public:
  bool HeartBeat = false;
  bool NextStep = false;
  bool Leave = false;
  string Name;
  ClientPackage() = default;
  ClientPackage(char* UpDt);
  ClientPackage(bool hb, bool ng, bool l, string n):
      HeartBeat(hb), NextStep(ng), Leave(l), Name(std::move(n)){}
  friend std::ostream &operator<<(std::ostream &out, const ClientPackage &s);
  friend std::istream &operator>>(std::istream &in, ClientPackage &s);
  string Serial();
};

#endif //TEXAS_HOLD_EM_PACKAGE_HPP
