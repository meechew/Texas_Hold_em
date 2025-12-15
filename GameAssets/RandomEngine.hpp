// Created by CBunt on 14 May 2020.
//

#ifndef TEXAS_HOLD_EM_RANDOMENGINE_HPP
#define TEXAS_HOLD_EM_RANDOMENGINE_HPP
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <ctime>

class RandomEngine {
private:
  boost::mt19937 gen;
  boost::uniform_int<int> RangeDef;
  boost::random::variate_generator<boost::mt19937,
				   boost::uniform_int<int>> *rng;

public:
  //RandomEngine() = default;
  RandomEngine(int x, int y);
  unsigned int operator()(int);
};


#endif //TEXAS_HOLD_EM_RANDOMENGINE_HPP
