// Created by CBunt on 30 May 2020.
//

#ifndef TEXAS_HOLD_EM_PACKAGE_HPP
#define TEXAS_HOLD_EM_PACKAGE_HPP
#include "Player.hpp"
#include <boost/array.hpp>

class ServerPackage {
public:
  bool HeartBeat;
  bool WinnerNotice;
  string Name;
  string Winner;
  hand Hand;
  boost::array<card, 5> Tbl;
  friend std::ostream &operator<<(std::ostream &out, const ServerPackage &s);
};

#endif //TEXAS_HOLD_EM_PACKAGE_HPP
