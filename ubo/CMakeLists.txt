set(destdir "${CMAKE_CURRENT_BINARY_DIR}")

gen_spirv_headers(ubo_headers DESTINATION ${destdir} FILES shader.vert shader.frag)

add_executable(ubo main.cpp ${ubo_headers})
target_include_directories(ubo PRIVATE ${destdir})
target_link_libraries(ubo GLEW::GLEW OpenGL::GL glfw)
