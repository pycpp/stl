//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <random> aliases.
 */

#pragma once

#include <pycpp/config.h>
#include <random>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Random number engines
using std::linear_congruential_engine;
using std::mersenne_twister_engine;
using std::subtract_with_carry_engine;

// Random number engine adaptors
using std::discard_block_engine;
using std::independent_bits_engine;
using std::shuffle_order_engine;

// Predefined generators
using std::minstd_rand0;
using std::minstd_rand;
using std::mt19937;
using std::mt19937_64;
using std::ranlux24_base;
using std::ranlux48_base;
using std::ranlux24;
using std::ranlux48;
using std::knuth_b;
using std::default_random_engine;

// Non-deterministic random numbers
using std::random_device;

// Uniform distributions
using std::uniform_int_distribution;
using std::uniform_real_distribution;

// Bernoulli distributions
using std::bernoulli_distribution;
using std::binomial_distribution;
using std::negative_binomial_distribution;
using std::geometric_distribution;

// Poisson distributions
using std::poisson_distribution;
using std::exponential_distribution;
using std::gamma_distribution;
using std::weibull_distribution;
using std::extreme_value_distribution;

// Normal distributions
using std::normal_distribution;
using std::lognormal_distribution;
using std::chi_squared_distribution;
using std::cauchy_distribution;
using std::fisher_f_distribution;
using std::student_t_distribution;

// Sampling distributions
using std::discrete_distribution;
using std::piecewise_constant_distribution;
using std::piecewise_linear_distribution;

// Utilities
using std::generate_canonical;
using std::seed_seq;

PYCPP_END_NAMESPACE
