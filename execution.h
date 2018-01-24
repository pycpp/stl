//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief <execution> aliases.
// TODO: may support parallel algorithms in C++11?
// Maybe?
 */

#pragma once

#include <pycpp/config.h>
#include <execution>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::is_execution_policy;
using std::is_execution_policy_v;

namespace execution
{

using std::execution::sequenced_policy;
using std::execution::parallel_policy;
using std::execution::parallel_unsequenced_policy;
using std::execution::seq;
using std::execution::par;
using std::execution::par_unseq;

}   /* execution */

PYCPP_END_NAMESPACE
