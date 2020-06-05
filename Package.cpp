// Created by CBunt on 31 May 2020.
//

#include "Package.hpp"


std::ostream &operator<<(std::ostream &out, const ServerPackage& s) {

  out << R"({ "HeartBeat" : )" << s.HeartBeat << ','
      << R"( "WinnerNotice" : )" << s.WinnerNotice << ','
      << R"( "Name" : ")" << s.Name << "\","
      << R"( "Winner" : ")" << s.Winner << "\","
      << R"( "Hand" : { "First" : {)"
      << R"( "Rank" : )" << s.Hand.first.rank << ','
      << R"( "Suit" : )" << s.Hand.first.suit << "},"
      << R"( "Second" : {)"
      << R"( "Rank" : )" << s.Hand.second.rank << ','
      << R"( "Suit" : )" << s.Hand.second.suit << "},"
      << R"( "CommonCards" : [)";


  for(int k = 0 ; k < 5 ; ++k ) {
    out << R"({ "Rank" : )" << s.Tbl[k].rank << ','
        << R"( "Suit" : )" << s.Tbl[k].suit << '}';
    if (k < 4) out << ',';
    else out << "]}}";
  }

  return out;
}

std::istream &operator>>(std::istream &in, ServerPackage &s) {
  char *c = new char;
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("HeartBeat", c));
  in.ignore(100, '"');
  in.get(*c);
  s.HeartBeat = atoi(c);
  in.ignore();
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("WinnerNotice", c));
  in.ignore(100, '"');
  in.get(*c);
  s.WinnerNotice = atoi(c);
  in.ignore();
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("SplitPot", c));
  in.ignore(100, '"');
  in.get(*c);
  s.SplitPot = atoi(c);
  in.ignore();
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("Name", c));
  in.ignore(100, '"');
  in.get(c, 100, '"');
  s.Name = c;
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("Winner", c));
  in.ignore(100, '"');
  in.get(c, 100, '"');
  s.Winner = c;
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("Hand", c));
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("First", c));
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("Rank", c));
  in.ignore(100, '"');
  in.get(*c);
  s.Hand.first.rank = atoi(c);
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("Suit", c));
  in.ignore(100, '"');
  in.get(*c);
  s.Hand.first.suit = atoi(c);
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("Second", c));
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("Rank", c));
  in.ignore(100, '"');
  in.get(*c);
  s.Hand.second.rank = atoi(c);
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("Suit", c));
  in.ignore(100, '"');
  in.get(*c);
  s.Hand.second.suit = atoi(c);
  in.ignore(100, '"');
  in.get(c, 100, '"');
  assert(strcmp("CommonCards", c));

  for (int k = 0; k < 5; ++k) {
    in.ignore(100, '"');
    in.get(c, 100, '"');
    assert(strcmp("Rank", c));
    in.ignore(100, '"');
    in.get(*c);
    s.Tbl[k].rank = atoi(c);
    in.ignore(100, '"');
    in.get(c, 100, '"');
    assert(strcmp("Suit", c));
    in.ignore(100, '"');
    in.get(*c);
    s.Tbl[k].suit = atoi(c);
  }

  delete c;
}

std::ostream &operator<<(std::ostream &out, const ClientPackage &s) {
  out << R"({ "HeartBeat" : )" << s.HeartBeat << ','
      << R"( "NewGame" : )" << s.NewGame << ','
      << R"( "Leave" : )" << s.Leave << ','
      << R"( "NAME" : )" << s.Name << '}';
}

std::istream &operator>>(std::istream &in, ClientPackage &s) {
  char *c = new char;
  in.ignore(100,'"');
  in.get(c,100,'"');
  assert(strcmp("HeartBeat",c));
  in.ignore(100,'"');
  in.get(*c);
  s.HeartBeat = atoi(c);
  in.ignore();
  in.ignore(100,'"');
  in.get(c,100,'"');
  assert(strcmp("NewGame",c));
  in.ignore(100,'"');
  in.get(*c);
  s.NewGame = atoi(c);
  in.ignore();
  in.ignore(100,'"');
  in.get(c,100,'"');
  assert(strcmp("Leave",c));
  in.ignore(100,'"');
  in.get(*c);
  s.Leave = atoi(c);
  in.ignore();
  in.ignore(100,'"');
  in.get(c,100,'"');
  assert(strcmp("Name",c));
  in.ignore(100,'"');
  in.get(c,100,'"');
  s.Name = c;
  delete c;
}