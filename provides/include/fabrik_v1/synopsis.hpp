#pragma once

#include "config.hpp"

#include "data_v1/synopsis.hpp"
#include "math3d_v1/synopsis.hpp"

namespace fabrik_v1 {

// fabrik.hpp ==================================================================

size_t move_to(const vec<float, 3> &target,
               float tolerance,
               const strided<vec<float, 3>> &positions,
               const strided<const float> &distances,
               size_t max_iter);

} // namespace fabrik_v1
