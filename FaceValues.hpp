// Created by CBunt on 16 May 2020.
//

#ifndef TEXAS_HOLD_EM_FACEVALUES_HPP
#define TEXAS_HOLD_EM_FACEVALUES_HPP
#include "Deck.hpp"
#include <boost/container/map.hpp>

typedef map<int,char> CardFace;

CardFace Rank = {
    {2, '2'},
    {3, '3'},
    {4, '4'},
    {5, '5'},
    {6, '6'},
    {7, '7'},
    {8, '8'},
    {9, '9'},
    {10, 'T'},
    {11, 'J'},
    {12, 'Q'},
    {13, 'K'},
    {14, 'A'}
};

/*CardFace SuitSym= {
    {1, '\u2660'},
    {2, '\u2665'},
    {3, '\u2663'},
    {4, '\u2666'}
};*/

CardFace SuitChar = {
    {1, 'S'},
    {2, 'H'},
    {3, 'C'},
    {4, 'D'}
};

#endif //TEXAS_HOLD_EM_FACEVALUES_HPP
