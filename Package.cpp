// Created by CBunt on 31 May 2020.
//

#include <local/include/boost/interprocess/streams/bufferstream.hpp>
#include "Package.hpp"

ServerPackage::ServerPackage(char *) {

}

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
  try {
    in.ignore(100, '{');
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("HeartBeat", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(*c);
    s.HeartBeat = atoi(c);
    in.ignore();
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("WinnerNotice", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(*c);
    s.WinnerNotice = atoi(c);
    in.ignore();
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("SplitPot", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(*c);
    s.SplitPot = atoi(c);
    in.ignore();
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("Name", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(c, 100, '"');
    s.Name = c;
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("Winner", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(c, 100, '"');
    s.Winner = c;
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("Hand", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("First", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("Rank", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(*c);
    s.Hand.first.rank = atoi(c);
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("Suit", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(*c);
    s.Hand.first.suit = atoi(c);
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("Second", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("Rank", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(*c);
    s.Hand.second.rank = atoi(c);
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("Suit", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(*c);
    s.Hand.second.suit = atoi(c);
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("CommonCards", c)) {throw  std::domain_error("Not valid package format");}

    for (int k = 0; k < 5; ++k) {
      in.ignore(100, '"');
      in.get(c, 100, '"');
      if (strcmp("Rank", c)) {throw  std::domain_error("Not valid package format");}
      in.ignore(100, '"');
      in.get(*c);
      s.Tbl[k].rank = atoi(c);
      in.ignore(100, '"');
      in.get(c, 100, '"');
      if (strcmp("Suit", c)) {throw  std::domain_error("Not valid package format");}
      in.ignore(100, '"');
      in.get(*c);
      s.Tbl[k].suit = atoi(c);
    }
  } catch (std::domain_error& e) {
    delete c;
    throw e;
  }
  delete c;
  return in;
}

ClientPackage::ClientPackage(char* str) {


  auto in = boost::interprocess::ibufferstream(str, 512);
  char *c = new char;
  try {
    in.ignore(100, '{');
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("HeartBeat", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, ':');
    in.get(c, 100, ',');
    HeartBeat = atoi(c);
    in.ignore();
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("NextStep", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, ':');
    in.get(c, 100, ',');
    NextStep = atoi(c);
    in.ignore();
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("Leave", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, ':');
    in.get(c, 100, ',');
    Leave = atoi(c);
    in.ignore();
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("Name", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(c, 100, '"');
    Name = c;
  } catch (std::domain_error& e) {
    std::cerr << e.what() << " package dropped\n";
  }
  delete c;
}

std::ostream &operator<<(std::ostream &out, const ClientPackage &s) {
  out << R"({ "HeartBeat" : )" << s.HeartBeat << ','
      << R"( "NextStep" : )" << s.NextStep << ','
      << R"( "Leave" : )" << s.Leave << ','
      << R"( "Name" : ")" << s.Name << '\"' << '}';

  return out;
}

std::istream &operator>>(std::istream &in, ClientPackage &s) {
  char *c = new char;
  try {
    in.ignore(100, '{');
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("HeartBeat", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, ':');
    in.get(c, 100, ',');
    s.HeartBeat = atoi(c);
    in.ignore();
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("NextStep", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, ':');
    in.get(c, 100, ',');
    s.NextStep = atoi(c);
    in.ignore();
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("Leave", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, ':');
    in.get(c, 100, ',');
    s.Leave = atoi(c);
    in.ignore();
    in.ignore(100, '"');
    in.get(c, 100, '"');
    if (strcmp("Name", c)) {throw  std::domain_error("Not valid package format");}
    in.ignore(100, '"');
    in.get(c, 100, '"');
    s.Name = c;
  } catch (std::domain_error& e) {
    std::cerr << e.what() << " package dropped\n";
  }
  delete c;
  return in;
}

