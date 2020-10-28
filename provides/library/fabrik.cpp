#include "fabrik_v1/fabrik.hpp"

#include "data_v1/strided.hpp"
#include "math3d_v1/vec.hpp"

#include <cassert>
#include <numeric>

using namespace math3d_v1;
using namespace data_v1;

bool fabrik_v1::is_within_tolerance(
    const vec_t<float, 3> &target,
    float tolerance,
    const strided_t<vec_t<float, 3>> &positions) {
  assert(2 <= positions.size());

  return norm(target - positions.back()) <= tolerance * tolerance;
}

void fabrik_v1::adjust_from(const vec_t<float, 3> &root,
                            const strided_t<vec_t<float, 3>> &positions,
                            const strided_t<const float> &distances) {
  assert(2 <= positions.size());
  assert(positions.size() == distances.size() + 1);

  auto currentPos = positions.begin();
  auto lastPos = --positions.end();
  auto currentDist = distances.begin();

  *currentPos = root;
  do {
    auto &previous = *currentPos;
    auto &current = *++currentPos;
    auto distance = mag(current - previous);
    *currentPos = lerp(previous, current, *currentDist++ / distance);
  } while (currentPos != lastPos);
}

void fabrik_v1::point_towards(const vec_t<float, 3> &target,
                              const strided_t<vec_t<float, 3>> &positions,
                              const strided_t<const float> &distances) {
  assert(2 <= positions.size());
  assert(positions.size() == distances.size() + 1);

  auto currentPos = positions.begin();
  auto lastPos = --positions.end();
  auto currentDist = distances.begin();

  do {
    auto &previous = *currentPos++;
    auto distance = mag(target - previous);
    *currentPos = lerp(previous, target, *currentDist++ / distance);
  } while (currentPos != lastPos);
}

size_t fabrik_v1::move_to(const vec_t<float, 3> &target,
                          float tolerance,
                          const strided_t<vec_t<float, 3>> &positions,
                          const strided_t<const float> &distances,
                          size_t max_iter) {
  assert(0 < max_iter);
  assert(positions.size() == distances.size() + 1);

  if (is_within_tolerance(target, tolerance, positions)) {
    return 0;
  }

  auto root = positions.front();

  auto totalLength = std::accumulate(distances.begin(), distances.end(), 0.0f);

  if (totalLength * totalLength < norm(target - root)) {
    point_towards(target, positions, distances);
    return 0;
  }

  size_t iter = 0;

  do {
    adjust_from(target, reversed(positions), reversed(distances));
    adjust_from(root, positions, distances);
  } while (++iter < max_iter &&
           !is_within_tolerance(target, tolerance, positions));

  return iter;
}
