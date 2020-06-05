// Created by CBunt on 16 May 2020.
//

#include "NetworkController.hpp"

Table::Table() {
  TableGame = new Game;
  ServerThread = std::make_shared<boost::thread>(TheadStarter());
  boost::format Fmt;

  for(int k = 0; k < 5; ++k) {
    Fmt = boost::format("Player%1%") %k;
    NewPlayer(boost::container::string(Fmt.str().c_str()));
  }
}

boost::thread Table::TheadStarter() {
  return boost::thread([this]{StartServer();});
}

[[noreturn]] void Table::StartServer() {
  std::cerr << "--STARTING SERVER--\n";
  try {
    tcp::endpoint EndPoint(tcp::v4(), 5000);
    Context.run();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  Update UpDt;
  std::stringstream StringBuff;
  for(;;) {
    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    for(int k = 0; k < 5 ; ++k) {
      StringBuff << Package(k, true, false, false);
      UpDt.MkBodyLength(std::strlen(StringBuff.str().c_str())+1);
      std::memcpy(UpDt.Body(),StringBuff.str().c_str(), UpDt.RetBodyLength() );
      UpDt.EncodeHeader();

    }
  }
}

int Table::NewPlayer(boost::container::string name) {
  boost::format Fmt;

  for (int k = 0; k < 5; ++k) {
    Fmt = boost::format("Player%1%") %k;
    if (Fmt.str().c_str() != HostPlayers[k].Who()) {
      HostPlayers[k].AddPlayer(name);
      return 0;
    }
  }

  return -1;
}

int Table::IncomingPlayer(SeatPtr) {


  return 0;
}

void Table::PlayerLeave(SeatPtr Seat) {
  SeatedPlayers[Seat]->Fold();
  SeatedPlayers.erase(Seat);
}

void Table::IncomingUpdate(Update) {

}

void Table::GameStart() {
  if (TableGame)
    delete TableGame;
  TableGame = new Game;
}

void Table::Deal() {
  cards tmp;
  for (int k = 0; k < 5; ++k)
    HostPlayers[k].NewHand(TableGame->Deal(2));
}

cards Table::Flop() {
  cards ret = TableGame->Flop();
  CommonCards = ret;
  return ret;
}

card Table::River() {
  card ret = TableGame->River();
  CommonCards.emplace_back(ret);
  return ret;
}

ScoreBoard Table::Tabulate(const cards& Hand) {
  boost::array<int, 14> ranks = {0};
  boost::array<int, 4> suits = {0};

  for (auto c : Hand) {
    ++ranks[c.rank -2];
    ++suits[c.suit -1];
  }
  int n = 0;
  bool Straight = false;
  for (int r = 1; r < 14; ++r) {
    if (ranks[r] and ranks[r-1])
      ++n;
    else n = 0;
    if (n > 4) Straight = true;
  }

  bool Flush = false;
  for (auto s : suits)
    if (s > 4 ) Flush = true;

  bool FourKind = false;
  for (auto r : ranks)
    if ( r == 4) FourKind = true;

  bool ThreeKind = false;
  for (auto r : ranks)
    if (r == 3) ThreeKind = true;

  bool Pair1 = false;
  bool TwoPair = false;
  for (auto r : ranks) {
    if (r == 2 and Pair1)
      TwoPair = true;
    if (r == 2) Pair1 = true;
  }

  bool FullHouse = false;
  if (ThreeKind and Pair1 and !FourKind) {
    ThreeKind = false;
    Pair1 = false;
    FullHouse = true;
  }

  bool StraightFlush = false;
  if (Straight and Flush) {
    Straight = false;
    Flush = false;
    StraightFlush = true;
  }

  card HighCard;
  for (auto c : Hand) {
    if (c.rank < HighCard.rank)
      HighCard = c;
  }
 return ScoreBoard(StraightFlush, FourKind, FullHouse, Flush,
                   Straight, ThreeKind, TwoPair, Pair1, HighCard);
}

card Table::Turn() {
  card ret = TableGame->Turn();
  CommonCards.emplace_back(ret);
//  int k = 0;
//  for (auto p : HostPlayers) {
//    FinalHands[k].player = p;
//  }
  for (int k = 0; k < 5; ++k) {
    if (!FinalHands[k].player)
      continue;
    FinalHands[k].player = std::make_shared<Player>(HostPlayers[k]);
    FinalHands[k].FinalHand = CommonCards + HostPlayers[k].Call();
  }
  return ret;
}

Player* Table::CheckForWinner() {
  Player* ret = nullptr;

  for (auto p : FinalHands) {
    p.FinalScore = Tabulate(p.FinalHand);
  }

  if (FinalHands[0].FinalScore.StraightFlush or FinalHands[1].FinalScore.StraightFlush or
      FinalHands[2].FinalScore.StraightFlush or FinalHands[3].FinalScore.StraightFlush or
      FinalHands[4].FinalScore.StraightFlush) {

  }

  if (FinalHands[0].FinalScore.FourKind or FinalHands[1].FinalScore.FourKind or
      FinalHands[2].FinalScore.FourKind or FinalHands[3].FinalScore.FourKind or
      FinalHands[4].FinalScore.FourKind) {

  }

  if (FinalHands[0].FinalScore.FullHouse or FinalHands[1].FinalScore.FullHouse or
      FinalHands[2].FinalScore.FullHouse or FinalHands[3].FinalScore.FullHouse or
      FinalHands[4].FinalScore.FullHouse) {

  }

  if (FinalHands[0].FinalScore.Flush or FinalHands[1].FinalScore.Flush or
      FinalHands[2].FinalScore.Flush or FinalHands[3].FinalScore.Flush or
      FinalHands[4].FinalScore.Flush) {

  }

  if (FinalHands[0].FinalScore.Straight or FinalHands[1].FinalScore.Straight or
      FinalHands[2].FinalScore.Straight or FinalHands[3].FinalScore.Straight or
      FinalHands[4].FinalScore.Straight) {

  }

  if (FinalHands[0].FinalScore.ThreeKind or FinalHands[1].FinalScore.ThreeKind or
      FinalHands[2].FinalScore.ThreeKind or FinalHands[3].FinalScore.ThreeKind or
      FinalHands[4].FinalScore.ThreeKind) {

  }

  if (FinalHands[0].FinalScore.TwoPair or FinalHands[1].FinalScore.TwoPair or
      FinalHands[2].FinalScore.TwoPair or FinalHands[3].FinalScore.TwoPair or
      FinalHands[4].FinalScore.TwoPair) {

  }

  if (FinalHands[0].FinalScore.OnePair or FinalHands[1].FinalScore.OnePair or
      FinalHands[2].FinalScore.OnePair or FinalHands[3].FinalScore.OnePair or
      FinalHands[4].FinalScore.OnePair) {

  }


  
  return nullptr;
}

ServerPackage * Table::Package(int n, bool hb, bool wn, bool sp) {
  return new ServerPackage(hb, wn, sp, HostPlayers[n].Who(),"",HostPlayers[n].Call(),CommonCards);
}

