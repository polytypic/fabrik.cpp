#include "fabrik_v1/synopsis.hpp"

#include "data_v1/strided.hpp"
#include "math3d_v1/vec.hpp"

#include <cassert>
#include <numeric>

namespace fabrik_v1 {

static void adjust_from(const vec<float, 3> &target,
                        const strided<vec<float, 3>> &positions,
                        const strided<const float> &distances) {
  auto currentPos = positions.begin();
  auto lastPos = --positions.end();
  auto currentDist = distances.begin();

  *currentPos = target;
  do {
    auto &previous = *currentPos;
    auto &current = *++currentPos;
    auto distance = mag(current - previous);
    *currentPos = lerp(previous, current, *currentDist++ / distance);
  } while (currentPos != lastPos);
}

static void point_towards(const vec<float, 3> &target,
                          const strided<vec<float, 3>> &positions,
                          const strided<const float> &distances) {
  auto currentPos = positions.begin();
  auto lastPos = --positions.end();
  auto currentDist = distances.begin();

  do {
    auto &previous = *currentPos++;
    auto distance = mag(target - previous);
    *currentPos = lerp(previous, target, *currentDist++ / distance);
  } while (currentPos != lastPos);
}

} // namespace fabrik_v1

size_t fabrik_v1::move_to(const vec<float, 3> &target,
                          float tolerance,
                          const strided<vec<float, 3>> &positions,
                          const strided<const float> &distances,
                          size_t max_iter) {
  assert(0 < max_iter);
  assert(2 <= positions.size());
  assert(positions.size() == distances.size() + 1);

  auto root = positions[0];

  auto distanceFromRoot2 = norm(target - root);
  auto totalLength = std::accumulate(distances.begin(), distances.end(), 0.0f);

  if (totalLength * totalLength < distanceFromRoot2) {
    point_towards(target, positions, distances);
    return 0;
  } else {
    auto tolerance2 = tolerance * tolerance;

    size_t iter = 0;

    while (tolerance2 < norm(target - positions.back()) && iter++ < max_iter) {
      adjust_from(target, reversed(positions), reversed(distances));
      adjust_from(root, positions, distances);
    }

    return iter;
  }
}
