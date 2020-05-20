// Created by CBunt on 14 May 2020.
//

#include "RandomEngine.h"

RandomEngine::RandomEngine(int x, int y) {
  //initialize RNG with a random value
  gen = boost::mt19937(* ctime(0));
  RangeDef = boost::uniform_int<int>(x, y);
  rng = boost::random::variate_generator<boost::mt19937,  boost::uniform_int<int>>(gen, RangeDef);
}

int RandomEngine::operator()(int i) {
  return gen()%i;
}
