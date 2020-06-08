// Created by CBunt on 16 May 2020.
//

#include "NetworkController.hpp"

Table::Table(boost::asio::io_context &Context, const tcp::endpoint Endpoint) :
    ioContext(Context), Socket(Context) {
  NetworkController Server(Context, Endpoint, this);
  ServerThread = std::make_shared<boost::thread>(
      boost::thread([&Context](){Context.run();}));
  TableGame = new Game;

  boost::format Fmt;

  for(int k = 0; k < 5; ++k) {
    Fmt = boost::format("Player%1%") %k;
    NewPlayer(boost::container::string(Fmt.str().c_str()));
  }

  StartHeartBeat();
  ServerThread->join();
}


[[noreturn]] void Table::StartHeartBeat() {
  std::cout << "--STARTING HEART BEAT--\n";

  Update UpDt;
  string Pack;
  std::stringstream StringBuff;
  ServerPackage* cpack;

  for(;;) {
    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    ProssesUpdates();
    for(const auto& p : SeatedPlayers) {
      //cpack = Package(p.second, true, false, false);
      //StringBuff << cpack;
      Pack = Package(p.second, true, false, false)->Serial();
      UpDt.MkBodyLength(std::strlen(Pack.c_str())+1);
      std::memcpy(UpDt.Body(),Pack.c_str(), UpDt.RetBodyLength() );
      UpDt.EncodeHeader();
      p.first->CountTimer();
      p.first->Signal(UpDt);
    }
  }
  delete cpack;
}

int Table::NewPlayer(const boost::container::string& name) {
  boost::format Fmt;

  for (int k = 0; k < 5; ++k) {
    Fmt = boost::format("Player%1%") %k;
    if (Fmt.str().c_str() != HostPlayers[k].Who().c_str()) {
      HostPlayers[k].AddPlayer(name);
      std::cout << "--Player: " << name << " Added--\n";
      return k;
    }
  }

  return -1;
}

// Take a pointer to a socket and the first message from that socket
// Pulls the Name out of it and assigning association with player pointer.
int Table::IncomingPlayer(const SeatPtr& Seat, Update& UpDt) {
  ClientPackage Pack(UpDt.Body());
  int pos;
  //in >> Pack;

  pos = NewPlayer(Pack.Name);
  if (pos < 0)
    return pos;

  SeatedPlayers[Seat] = std::make_shared<Player>(HostPlayers[pos]);

  return pos;
}

void Table::PlayerLeave(SeatPtr Seat) {
  if (SeatedPlayers[Seat])
    SeatedPlayers[Seat]->Fold();
  SeatedPlayers.erase(Seat);
}

void Table::IncomingUpdate(Update UpDt) {
  IncomingQueue.push_back(UpDt);
}

void Table::ProssesUpdates() {
  std::shared_ptr<Update> UpdatePtr;
  while(IncomingQueue.size()) {
    UpdatePtr = std::make_shared<Update>(IncomingQueue.front());
    boost::interprocess::bufferstream BuffersStream(UpdatePtr->Body(),
        UpdatePtr->RetBodyLength());
    ClientPackage Pack;
    BuffersStream >> Pack;
/*
    if (Pack.HeartBeat) {
      Seat->ResetTimer();
      return;
    }*/

    if (Pack.NextStep) {
      Step();
      return;
    }
/*
    if (Pack.Leave) {
      PlayerLeave(Seat);
      return;
    }*/
    IncomingQueue.pop_front();
  }
}


void Table::GameStart() {
  delete TableGame;
  TableGame = new Game;
  Stage = 0;
}

void Table::Step() {
  switch (Stage) {
    case 0:
      Deal();
      std::cout << "\tDeal\n";
      return;
    case 1:
      Flop();
      std::cout << "\tFlop\n";
      return;
    case 2:
      Turn();
      std::cout << "\tTurn\n";
      return;
    case 3:
      River();
      std::cout << "\tRiver\n";
      return;
    case 4:
      CheckForWinner();
      std::cout << "\tCall\n";
      return;
    case 5:
      GameStart();
      std::cout << "\tNew Game\n";
      return;
    default:
      return;
  }
}

void Table::Deal() {
  for (int k = 0; k < 5; ++k)
    HostPlayers[k].NewHand(TableGame->Deal(2));
}

cards Table::Flop() {
  cards ret = TableGame->Flop();
  CommonCards = ret;
  return ret;
}

card Table::Turn() {
  card ret = TableGame->River();
  CommonCards.emplace_back(ret);
  return ret;
}

card Table::River() {
  card ret = TableGame->Turn();
  CommonCards.emplace_back(ret);
  for (int k = 0; k < 5; ++k) {
    if (!FinalHands[k].player)
      continue;
    FinalHands[k].player = std::make_shared<Player>(HostPlayers[k]);
    FinalHands[k].FinalHand = CommonCards + HostPlayers[k].Call();
  }
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


  
  return ret;
}

ServerPackage * Table::Package(PlayerPtr p, bool hb, bool wn, bool sp) {
  return new ServerPackage(hb, wn, sp, p->Who(),"", p->Call(),CommonCards);
}

