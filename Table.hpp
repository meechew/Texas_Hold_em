// Created by CBunt on 16 May 2020.
//

#ifdef TEXAS_HOLD_EM_TABLE_HPP
#define TEXAS_HOLD_EM_TABLE_HPP

#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/format.hpp>
#include <utility>
#include "Deck.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include "Package.hpp"


struct ScoreBoard {
  card HighCard = card(0, 0);
  bool StraightFlush = false;
  bool FourKind = false;
  bool FullHouse = false;
  bool Flush = false;
  bool Straight = false;
  bool ThreeKind = false;
  bool TwoPair = false;
  bool OnePair = false;
  ScoreBoard() = default;
  ScoreBoard(bool StraightFlush, bool FourKind, bool FullHouse, bool Flush, bool Straight,
             bool ThreeKind, bool TwoPair, bool OnePair, const card &HighCard) : HighCard(HighCard), StraightFlush(StraightFlush),
             FourKind(FourKind), FullHouse(FullHouse), Flush(Flush), Straight(Straight),
              ThreeKind(ThreeKind), TwoPair(TwoPair), OnePair(OnePair) {}
};

typedef std::shared_ptr<Player> PlayerPtr;

struct Finals {
  PlayerPtr player;
  cards FinalHand;
  ScoreBoard FinalScore;
};

typedef boost::array<Finals,5> PlayerFinals;
typedef boost::array<Player,5> Players;


class Table {
private:
  Game *TableGame = nullptr;
  Players HostPlayers;
  PlayerFinals FinalHands;
  static ScoreBoard Tabulate(const cards&);
  boost::thread TheadStarter();

  std::shared_ptr<boost::thread> ServerThread;

public:
  cards CommonCards;
  Table();
  int NewPlayer(boost::container::string name);
  void PlayerLeave(PlayerPtr);
  void GameStart();
  void Deal();
  cards Flop();
  card River();
  card Turn();
  Player* CheckForWinner();
  ServerPackage *Package(int n, bool hb, bool wn, bool sp);
};


#endif //TEXAS_HOLD_EM_TABLE_HPP
