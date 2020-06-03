// Created by CBunt on 30 May 2020.
//

#ifndef TEXAS_HOLD_EM_PACKAGE_HPP
#define TEXAS_HOLD_EM_PACKAGE_HPP
#include "Player.hpp"
#include <boost/container/vector.hpp>

class ServerPackage {
public:
  bool HeartBeat;
  bool WinnerNotice;
  bool SplitPot;
  string Name;
  string Winner;
  hand Hand;
  boost::container::vector<card> Tbl;
  ServerPackage(bool hb, bool wn, bool sp, string n, string w, hand h, boost::container::vector<card> tbl):
    HeartBeat(hb), WinnerNotice(wn), SplitPot(sp), Name(n), Hand(h), Tbl(tbl){}
  friend std::ostream &operator<<(std::ostream &out, const ServerPackage &s);
};

#endif //TEXAS_HOLD_EM_PACKAGE_HPP
