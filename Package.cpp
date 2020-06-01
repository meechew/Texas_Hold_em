// Created by CBunt on 31 May 2020.
//

#include "Package.hpp"


std::ostream &operator<<(std::ostream &out, const ServerPackage& s) {

  out << R"({ "HeartBeat" : )" << s.HeartBeat << ','
      << R"("WinnerNotice" : )" << s.WinnerNotice << ','
      << R"("Name" : )" << s.Name << ','
      << R"("Winner" : )" << s.Winner << ','
      << R"("Hand" : { "First" : {)"
      << R"( "Rank" : )" << s.Hand.first.rank << ','
      << R"( "Suit" : )" << s.Hand.first.suit << "},"
      << R"( "Second : {)"
      << R"( "Rank" : )" << s.Hand.second.rank << ','
      << R"( "Suit" : )" << s.Hand.second.suit << "},"
      << R"( "CommonCards : [)";


  for(int k = 0 ; k < 5 ; ++k ) {
    out << R"({ "Rank" : )" << s.Tbl[k].rank << ','
        << R"( "Suit" : )" << s.Tbl[k].suit << '}';
    if (k < 4) out << ',';
  }
  out << "]}"
  ;
  return out;
}
