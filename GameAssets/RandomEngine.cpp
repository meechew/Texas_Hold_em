// Created by CBunt on 14 May 2020.
//

#include "RandomEngine.hpp"

RandomEngine::RandomEngine(int x, int y) {
  //initialize RNG with a random value
  gen = boost::mt19937(time(0));
  RangeDef = boost::uniform_int<int>(x, y);
  rng = new boost::random::variate_generator<boost::mt19937,  boost::uniform_int<int>>(gen, RangeDef);
}

unsigned int RandomEngine::operator()(int i) {
  return gen()%i;
}
