// Created by CBunt on 14 May 2020.
//

#include "RandomEngine.h"

template<typename T>
RandomEngine<T>::RandomEngine(int x, int y) {
  //initialize RNG with a random value
  rng = boost::mt19937(* ctime(0));
  RangeDef = boost::random::uniform_int_distribution<T>(x, y);
  gen =   boost::random::variate_generator<boost::mt19937,  boost::random::uniform_int_distribution<T>> (rng, RangeDef);
}

template<typename T>
T RandomEngine<T>::operator()() {
  return gen();
}