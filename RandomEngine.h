// Created by CBunt on 14 May 2020.
//

#ifndef TEXAS_HOLD_EM_RANDOMENGINE_H
#define TEXAS_HOLD_EM_RANDOMENGINE_H
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <ctime>

template<typename T>
class RandomEngine {
private:
  boost::mt19937 rng;
  boost::random::uniform_int_distribution<T> RangeDef;

public:
  typedef T                               value_type;
  typedef T*                              pointer;
  typedef T&                              reference;
  RandomEngine() = default;
  RandomEngine<T>(int x, int y);
  T operator()();
};


#endif //TEXAS_HOLD_EM_RANDOMENGINE_H
