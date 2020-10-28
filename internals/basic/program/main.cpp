#include "data_v1/array.hpp"
#include "data_v1/lazy.hpp"
#include "emscripten_v1/html5.hpp"
#include "fabrik_v1/fabrik.hpp"
#include "gl_v1/gl.hpp"
#include "math3d_v1/transform.hpp"
#include "math3d_v1/trig.hpp"

#include <cstdio>

#include "mesh.hpp"

namespace basic {

using namespace emscripten;

static const auto viewport = vec(800, 800);

static const auto prepared = lazy([]() {
  return prepare_mesh(strided<vertex_t>({
                          {{0, 0, 0}, {-1, 0, 0}},
                          {{0.2, 0, -1}, {0, 0, -1}},
                          {{0.2, 1, 0}, {0, 1, 0}},
                          {{0.2, 0, 1}, {0, 0, 1}},
                          {{0.2, -1, 0}, {0, -1, 0}},
                          {{1, 0, 0}, {1, 0, 0}},
                      }),
                      strided<triangle_t>({
                          {0, 2, 1},
                          {0, 3, 2},
                          {0, 4, 3},
                          {0, 1, 4},
                          {5, 1, 2},
                          {5, 2, 3},
                          {5, 3, 4},
                          {5, 4, 1},
                      }));
});

static const auto projection = perspective(from_angle(45.0f), 1.0f, 9.0f);
static const auto camera = translation(vec(0.0f, 0.0f, -8.0f));

static const auto projection_camera = projection * camera;

static const auto center_z = homogenize(projection_camera * vec(0, 0, 0, 1))[2];

static const auto projection_camera_inv = inverse(projection_camera);

static vec_t<float, 3> target = {1, -2, 0};
static vec_t<float, 3> positions[] = {
    {0, 0, 0}, {0, 1, 0}, {0, 2.5, 0}, {0, 3, 0}, {0, 4, 0}, {0, 4.5, 0}};
static const float distances[] = {1, 1.5, 0.5, 1, 0.5};

void main() {
  gl::Init("canvas");

  gl::Viewport(viewport);

  set_mouse_callback(
      "canvas", strided({MOUSEDOWN, MOUSEMOVE}), [](auto, auto &event) {
        if (event.buttons & 1) {
          auto mouse_pos =
              (vec(event.targetX, event.targetY) * 2.0f - viewport) / viewport *
              vec(1, -1);

          auto r = projection_camera_inv * vec(mouse_pos, center_z, 1.0f);

          target = sub<3>(homogenize(r));
        }
        return true;
      });

  request_animation_frame_loop([](auto) {
    gl::ClearColor(0, 0, 0, 1);
    gl::Clear(gl::COLOR_BUFFER_BIT);

    fabrik::move_to(target, 0.001, positions, distances, 1);

    auto currentPos = begin(positions);
    auto lastPos = end(positions) - 1;
    do {
      auto &previous = *currentPos;
      auto &current = *++currentPos;

      auto delta = current - previous;
      auto distance = mag(delta);

      auto x = delta * (1 / distance);
      auto y = normalize(cross(vec(0, 0, 1), x));
      auto z = cross(x, y);

      auto world =
          translation(previous) *
          from_columns(vec(x, 0), vec(y, 0), vec(z, 0), vec(0, 0, 0, 1)) *
          scaling(vec(distance, 0.1f, 0.1f));

      render(prepared, world, projection_camera);
    } while (currentPos != lastPos);

    return true;
  });
}

} // namespace basic

int main() {
  printf("main()\n");

  basic::main();

  printf("main() -> 0\n");

  return 0;
}
