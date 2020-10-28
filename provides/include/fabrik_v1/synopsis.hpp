#pragma once

#include "fabrik_v1/config.hpp"

#include "data_v1/synopsis.hpp"
#include "math3d_v1/synopsis.hpp"

namespace fabrik_v1 {

// fabrik.hpp ==================================================================

size_t move_to(const vec_t<float, 3> &target,
               float tolerance,
               const strided_t<vec_t<float, 3>> &positions,
               const strided_t<const float> &distances,
               size_t max_iter);

bool is_within_tolerance(const vec_t<float, 3> &target,
                         float tolerance,
                         const strided_t<vec_t<float, 3>> &positions);

void adjust_from(const vec_t<float, 3> &root,
                 const strided_t<vec_t<float, 3>> &positions,
                 const strided_t<const float> &distances);

void point_towards(const vec_t<float, 3> &target,
                   const strided_t<vec_t<float, 3>> &positions,
                   const strided_t<const float> &distances);

} // namespace fabrik_v1
