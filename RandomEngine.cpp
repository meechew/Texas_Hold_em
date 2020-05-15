// Created by CBunt on 14 May 2020.
//

#include "RandomEngine.h"

template<typename T>
RandomEngine<T>::RandomEngine(int x, int y) {
  //initializ RNG with a random value
  rng = std::mt19937(Device());
  RangeDef = std::uniform_int_distribution<T>(x, y);
}

template<typename T>
T RandomEngine<T>::operator()() {
  return rng();
}