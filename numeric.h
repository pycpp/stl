//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief <numeric> aliases.
 */

#pragma once

#include <pycpp/stl/numeric/exclusive_scan.h>
#include <pycpp/stl/numeric/gcd.h>
#include <pycpp/stl/numeric/inclusive_scan.h>
#include <pycpp/stl/numeric/lcm.h>
#include <pycpp/stl/numeric/reduce.h>
#include <pycpp/stl/numeric/transform_exclusive_scan.h>
#include <pycpp/stl/numeric/transform_inclusive_scan.h>
#include <pycpp/stl/numeric/transform_reduce.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

// Functions
using std::iota;
using std::accumulate;
using std::inner_product;
using std::adjacent_difference;
using std::partial_sum;

PYCPP_END_NAMESPACE
