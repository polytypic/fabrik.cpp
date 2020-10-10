#pragma once

#include "data_v1/synopsis.hpp"
#include "gl_v1/synopsis.hpp"

#include "config.hpp"

namespace basic {

struct vertex {
  vec<GLfloat, 3> position;
  vec<GLfloat, 3> normal;
};

struct triangle {
  GLuint vertices[3];
};

struct prepared_mesh {
  gl::VertexArray vertex_array;
  gl::Buffer vertex_buffer;
  gl::Buffer index_buffer;
  size_t num_indices;
};

prepared_mesh prepare_mesh(const contiguous<const vertex> &vertices,
                           const contiguous<const triangle> &indices);

void render(const prepared_mesh &mesh,
            const mtx<float, 4> &view,
            const mtx<float, 4> &projection);

} // namespace basic
