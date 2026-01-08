// Created by meechew on 12/27/25.
//

#include "GameTable.hpp"

void GameTable::incoming_player(SeatPtr s, ClientPackage p)
{
    Player _player(p.name_, p.uuid_);
    SeatPtr _seat_pointer(find_socket(_player));
    if (_seat_pointer) return;
    add_play(s, boost::make_shared<Player>(_player));
}

void GameTable::player_leave(SeatPtr s)
{
}

void GameTable::process_package(ClientPackage p)
{

}

void GameTable::game_start()
{
}

void GameTable::step()
{
}

// ScoreBoard GameTable::tabulate(const Cards&)
// {
// }
//
// void GameTable::deal()
// {
// }
//
// Cards GameTable::flop()
// {
// }
//
// Card GameTable::turn()
// {
// }
//
// Card GameTable::river()
// {
// }
//
// Player* GameTable::check_for_winner()
// {
// }
//
// ServerPackage* GameTable::make_package(PlayerPtr p, bool hb, bool wn, bool sp) const
// {
// }
