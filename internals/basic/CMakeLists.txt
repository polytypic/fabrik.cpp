add_conventional_executable(basic)
target_link_libraries(basic PRIVATE fabrik_v1 gl_v1 emscripten_system_include)
target_link_options(basic PUBLIC
  "SHELL:-s USE_WEBGL2=1"
  "SHELL:-s MIN_WEBGL_VERSION=2"
  "SHELL:-s GL_EMULATE_GLES_VERSION_STRING_FORMAT=0"
  "SHELL:-s WASM=1")
add_custom_command(
  TARGET basic POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:basic>/basic.* ${CMAKE_SOURCE_DIR}/docs/)
