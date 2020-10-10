#include "fabrik_v1/fabrik.hpp"

#include "data_v1/array.hpp"
#include "data_v1/strided.hpp"
#include "math3d_v1/vec.hpp"

#include "testing_v1/test.hpp"

using namespace testing_v1;
using namespace data_v1;
using namespace math3d_v1;
using namespace fabrik_v1;

auto fabrik_test = test([]() {
  vec<float, 3> target = {1, -2, 3};
  vec<float, 3> positions[] = {
      {0, 0, 0}, {0, 1, 0}, {0, 2.5, 0}, {0, 3, 0}, {0, 5, 0}, {0, 5.5, 0}};
  float distances[] = {1, 1.5, 0.5, 2, 0.5};

  auto iters = move_to(target, 0.001, positions, distances, 100);

  verify(0 < iters && iters <= 100);

  verify(mag(positions[size(positions) - 1] - target) <= 0.001);

  for (size_t i = 0; i < size(distances); ++i) {
    verify(abs(mag(positions[i] - positions[i + 1]) - distances[i]) < 0.0001);
  }
});