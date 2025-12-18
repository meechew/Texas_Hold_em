// Created by CBunt on 14 May 2020.
//

#include "RandomEngine.hpp"

RandomEngine::RandomEngine(int x, int y) {
  //initialize RNG with a random value
  generator_ = boost::mt19937(time(0));
  range_ = boost::uniform_int<int>(x, y);
  rng_ = new boost::random::variate_generator<boost::mt19937,  boost::uniform_int<int>>(generator_, range_);
}

unsigned int RandomEngine::operator()(int i) {
  return generator_()%i;
}
