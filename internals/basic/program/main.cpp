#include "data_v1/array.hpp"
#include "fabrik_v1/synopsis.hpp"
#include "gl_v1/gl.hpp"
#include "math3d_v1/mtx.hpp"
#include "math3d_v1/transform.hpp"
#include "math3d_v1/vec.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <tuple>

#include "mesh.hpp"

namespace basic {

static const basic::vertex vertices[] = {
    {{0, 0, 0}, {-1, 0, 0}},
    {{0.2, 0, -1}, {0, 0, -1}},
    {{0.2, 1, 0}, {0, 1, 0}},
    {{0.2, 0, 1}, {0, 0, 1}},
    {{0.2, -1, 0}, {0, -1, 0}},
    {{1, 0, 0}, {1, 0, 0}},
};

static const basic::triangle indices[] = {
    {0, 2, 1},
    {0, 3, 2},
    {0, 4, 3},
    {0, 1, 4},
    {5, 1, 2},
    {5, 2, 3},
    {5, 3, 4},
    {5, 4, 1},
};

static prepared_mesh prepared;

static const auto projection = make_projection<float>(45, 1, 9);
static const auto camera = make_translation(vec<float, 3>{0, 0, -8});

static const auto center =
    homogenize(projection * camera * vec<float, 4>{0, 0, 0, 1});

static const auto projection_camera_inverse = inverse(projection * camera);

static vec<float, 3> target = {1, -2, 0};
static vec<float, 3> positions[] = {
    {0, 0, 0}, {0, 1, 0}, {0, 2.5, 0}, {0, 3, 0}, {0, 4, 0}, {0, 4.5, 0}};
static const float distances[] = {1, 1.5, 0.5, 1, 0.5};

EM_BOOL mouse_move(int, const EmscriptenMouseEvent *mouse_event, void *) {
  if (mouse_event->buttons & 1) {
    auto x = (mouse_event->targetX - 400) / 400.0f;
    auto y = (400 - mouse_event->targetY) / 400.0f;

    auto r = projection_camera_inverse * vec<float, 4>{x, y, center[2], 1};

    r = r * (1 / r[3]);

    target[0] = r[0];
    target[1] = r[1];
    target[2] = r[2];
  }
  return true;
}

EM_BOOL animation_frame(double, void *) {
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
    auto y = normalize(cross(vec<float, 3>{0, 0, 1}, x));
    auto z = cross(x, y);

    auto world = make_translation(previous) *
                 mtx<float, 4>{{{x[0], y[0], z[0], 0},
                                {x[1], y[1], z[1], 0},
                                {x[2], y[2], z[2], 0},
                                {0, 0, 0, 1}}} *
                 make_scaling(vec<float, 3>{distance, 0.1, 0.1});

    auto view = camera * world;

    render(prepared, view, projection);
  } while (currentPos != lastPos);

  return true;
}

void main() {
  gl::Init("canvas");

  gl::Viewport(0, 0, 800, 800);

  if (EMSCRIPTEN_RESULT_SUCCESS != emscripten_set_mousemove_callback(
                                       "canvas", nullptr, false, mouse_move) ||
      EMSCRIPTEN_RESULT_SUCCESS != emscripten_set_mousedown_callback(
                                       "canvas", nullptr, false, mouse_move)) {
    printf("emscripten_set_mousemove_callback || "
           "emscripten_set_mousedown_callback\n");
    exit(1);
  }

  prepared = prepare_mesh(vertices, indices);

  emscripten_request_animation_frame_loop(animation_frame, nullptr);
}

} // namespace basic

int main() {
  printf("main()\n");

  basic::main();

  printf("main() -> 0\n");

  return 0;
}