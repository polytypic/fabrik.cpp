#pragma once

#include "fabrik_v1/config.hpp"

#include "data_v1/synopsis.hpp"
#include "math3d_v1/synopsis.hpp"

namespace fabrik_v1 {

// fabrik.hpp ==================================================================

size_t move_to(const vec<float, 3> &target,
               float tolerance,
               const strided<vec<float, 3>> &positions,
               const strided<const float> &distances,
               size_t max_iter);

bool is_within_tolerance(const vec<float, 3> &target,
                         float tolerance,
                         const strided<vec<float, 3>> &positions);

void adjust_from(const vec<float, 3> &root,
                 const strided<vec<float, 3>> &positions,
                 const strided<const float> &distances);

void point_towards(const vec<float, 3> &target,
                   const strided<vec<float, 3>> &positions,
                   const strided<const float> &distances);

} // namespace fabrik_v1
