#pragma once

#include "data_v1/synopsis.hpp"
#include "gl_v1/synopsis.hpp"

#include "config.hpp"

namespace basic {

struct vertex_t {
  vec_t<GLfloat, 3> position;
  vec_t<GLfloat, 3> normal;
};

struct triangle_t {
  GLuint vertices[3];
};

struct prepared_mesh_t {
  gl::VertexArray vertex_array;
  gl::Buffer vertex_buffer;
  gl::Buffer index_buffer;
  size_t num_indices;
};

prepared_mesh_t prepare_mesh(const contiguous_t<const vertex_t> &vertices,
                             const contiguous_t<const triangle_t> &indices);

void render(const prepared_mesh_t &mesh,
            const mtx_t<float, 4> &view,
            const mtx_t<float, 4> &projection);

} // namespace basic
