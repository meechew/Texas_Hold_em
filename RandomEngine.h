// Created by CBunt on 14 May 2020.
//

#ifndef TEXAS_HOLD_EM_RANDOMENGINE_H
#define TEXAS_HOLD_EM_RANDOMENGINE_H
#include <random>

template<typename T>
class RandomEngine {
private:
  std::random_device Device;
  std::mt19937  rng;
  std::uniform_int_distribution<T> RangeDef;

public:
  typedef T                               value_type;
  typedef T*                              pointer;
  typedef T&                              reference;
  RandomEngine() = delete;
  RandomEngine<T>(int x, int y);
  T operator()();
};


#endif //TEXAS_HOLD_EM_RANDOMENGINE_H
