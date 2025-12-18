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

  for(;;) {
    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    ProcessUpdate();
    for(const auto& p : SeatedPlayers) {
      Pack = Package(p.right, true, false, false)->serial();
      UpDt.MkBodyLength(std::strlen(Pack.c_str())+1);
      std::memcpy(UpDt.Body(),Pack.c_str(), UpDt.RetBodyLength() );
      UpDt.EncodeHeader();
      p.left->count_timer();
      p.left->signal(UpDt);
    }
  }
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
int Table::IncomingPlayer(SeatPtr Seat, Update& UpDt) {
  ClientPackage Pack(UpDt.Body());
  int pos;
  //in >> Pack;

  pos = NewPlayer(Pack.name_);
  if (pos < 0)
    return pos;

  SeatedPlayers.insert(SocketSeatPair(Seat,
      std::make_shared<Player>(HostPlayers[pos])));

  dynamic_cast<Session&>(*Seat).Join();

  return pos;
}

void Table::PlayerLeave(SeatPtr Seat) {
  if (SeatedPlayers.left.at(Seat))
    SeatedPlayers.left.at(Seat)->Fold();
  SeatedPlayers.left.erase(Seat);
}

void Table::IncomingUpdate(Update UpDt) {
  IncomingQueue.push_back(UpDt);
}


// This can be written more effectively. maybe rewrite to attach the
// socket to the Player class.
void Table::ProcessUpdate() {
  std::shared_ptr<Update> UpdatePtr;
  while(!IncomingQueue.empty()) {
    ClientPackage Pack;
    UpdatePtr = std::make_shared<Update>(IncomingQueue.front());
    boost::interprocess::bufferstream BufferStream(UpdatePtr->Body(),
        UpdatePtr->RetBodyLength());
    BufferStream >> Pack;

    PlayerPtr Ptr;
    for(int k = 0; k < 5; ++k)
      if(HostPlayers[k].Who() == Pack.name_)
        Ptr = std::make_shared<Player>(HostPlayers[k]);
    if (!Ptr.get()) return;

    if (Pack.heart_beat_) {
      SeatedPlayers.right.at(Ptr)->reset_timer();
      return;
    }

    if (Pack.next_step_) {
      Step();
      return;
    }

    if (Pack.leave_) {
      PlayerLeave(SeatedPlayers.right.at(Ptr));
      return;
    }
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
    HostPlayers[k].NewHand(TableGame->deal(2));
}

cards Table::Flop() {
  cards ret = TableGame->flop();
  CommonCards = ret;
  return ret;
}

card Table::Turn() {
  card ret = TableGame->river();
  CommonCards.emplace_back(ret);
  return ret;
}

card Table::River() {
  card ret = TableGame->turn();
  CommonCards.emplace_back(ret);
  for (int k = 0; k < 5; ++k) {
    if (!FinalHands[k].player_)
      continue;
    FinalHands[k].player_ = std::make_shared<Player>(HostPlayers[k]);
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
    p.final_score_ = Tabulate(p.FinalHand);
  }

  if (FinalHands[0].final_score_.straight_flush_ or FinalHands[1].final_score_.straight_flush_ or
      FinalHands[2].final_score_.straight_flush_ or FinalHands[3].final_score_.straight_flush_ or
      FinalHands[4].final_score_.straight_flush_) {

  }

  if (FinalHands[0].final_score_.four_kind_ or FinalHands[1].final_score_.four_kind_ or
      FinalHands[2].final_score_.four_kind_ or FinalHands[3].final_score_.four_kind_ or
      FinalHands[4].final_score_.four_kind_) {

  }

  if (FinalHands[0].final_score_.full_house_ or FinalHands[1].final_score_.full_house_ or
      FinalHands[2].final_score_.full_house_ or FinalHands[3].final_score_.full_house_ or
      FinalHands[4].final_score_.full_house_) {

  }

  if (FinalHands[0].final_score_.flush_ or FinalHands[1].final_score_.flush_ or
      FinalHands[2].final_score_.flush_ or FinalHands[3].final_score_.flush_ or
      FinalHands[4].final_score_.flush_) {

  }

  if (FinalHands[0].final_score_.straight_ or FinalHands[1].final_score_.straight_ or
      FinalHands[2].final_score_.straight_ or FinalHands[3].final_score_.straight_ or
      FinalHands[4].final_score_.straight_) {

  }

  if (FinalHands[0].final_score_.three_kind_ or FinalHands[1].final_score_.three_kind_ or
      FinalHands[2].final_score_.three_kind_ or FinalHands[3].final_score_.three_kind_ or
      FinalHands[4].final_score_.three_kind_) {

  }

  if (FinalHands[0].final_score_.two_pair_ or FinalHands[1].final_score_.two_pair_ or
      FinalHands[2].final_score_.two_pair_ or FinalHands[3].final_score_.two_pair_ or
      FinalHands[4].final_score_.two_pair_) {

  }

  if (FinalHands[0].final_score_.one_pair_ or FinalHands[1].final_score_.one_pair_ or
      FinalHands[2].final_score_.one_pair_ or FinalHands[3].final_score_.one_pair_ or
      FinalHands[4].final_score_.one_pair_) {

  }


  
  return ret;
}

ServerPackage * Table::Package(PlayerPtr p, bool hb, bool wn, bool sp) {
  return new ServerPackage(hb, wn, sp, p->Who(),"", p->Call(),CommonCards);
}

