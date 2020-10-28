#include "mesh.hpp"

#include "math3d_v1/mtx.hpp"

#include "gl_v1/gl.hpp"

namespace basic {

static gl::Program s_program;

static void compile_program() {
  if (s_program.name)
    return;

  auto vertex_shader = gl::CompileShader(gl::VERTEX_SHADER, R"(#version 300 es
    precision highp float;

    in vec3 position;
    in vec3 normal;

    uniform mat4 view;
    uniform mat4 view_inv_trn;
    uniform mat4 view_projection;

    out vec3 v_normal;

    void main() {
      gl_Position = view_projection * vec4(position, 1);

      v_normal = normalize((view_inv_trn * vec4(normal, 1)).xyz);
    })");
  auto fragment_shader =
      gl::CompileShader(gl::FRAGMENT_SHADER, R"(#version 300 es
    precision highp float;

    in vec3 v_normal;

    uniform vec3 color;

    out vec4 out_color;

    void main() {
      float intensity =
        clamp(dot(vec3(0.0, 0.0, 1.0), normalize(v_normal)), 0.15, 1.0);

      out_color = vec4(color * intensity, 1);
    })");

  s_program = gl::LinkProgram(vertex_shader, fragment_shader);
}

} // namespace basic

basic::prepared_mesh_t
basic::prepare_mesh(const contiguous_t<const vertex_t> &vertices,
                    const contiguous_t<const triangle_t> &indices) {
  compile_program();

  auto vertex_array = gl::GenVertexArray();
  gl::BindVertexArray(vertex_array);

  auto vertex_buffer = gl::GenBuffer();
  gl::BindBuffer(gl::ARRAY_BUFFER, vertex_buffer);
  gl::BufferData(gl::ARRAY_BUFFER, vertices, gl::STATIC_DRAW);

  auto index_buffer = gl::GenBuffer();
  gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, index_buffer);
  gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, indices, gl::STATIC_DRAW);

  gl::UseProgram(s_program);

  auto position_loc = gl::GetAttribLocation(s_program, "position");
  gl::EnableVertexAttribArray(position_loc);
  gl::VertexAttribPointer(position_loc, &vertex_t::position);

  auto normal_loc = gl::GetAttribLocation(s_program, "normal");
  gl::EnableVertexAttribArray(normal_loc);
  gl::VertexAttribPointer(normal_loc, &vertex_t::normal);

  auto num_indices = indices.size() * 3;

  return {std::move(vertex_array),
          std::move(vertex_buffer),
          std::move(index_buffer),
          num_indices};
}

void basic::render(const prepared_mesh_t &mesh,
                   const mtx_t<float, 4> &view,
                   const mtx_t<float, 4> &projection) {
  gl::Enable(gl::CULL_FACE);

  gl::BindVertexArray(mesh.vertex_array);

  gl::Uniform(gl::GetUniformLocation(s_program, "color"),
              vec_t<float, 3>{1, 1, 1});

  gl::Uniform(gl::GetUniformLocation(s_program, "view"), view);
  gl::Uniform(gl::GetUniformLocation(s_program, "view_inv_trn"),
              transpose(inverse(view)));

  auto view_projection = projection * view;
  gl::Uniform(gl::GetUniformLocation(s_program, "view_projection"),
              view_projection);

  gl::DrawElements(
      gl::TRIANGLES, mesh.num_indices, gl::TypeOf<GLuint>(), nullptr);
}
